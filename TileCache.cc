#include "TileCache.h"
#include <functional>

std::ostream& operator<<(std::ostream& os, const TileCacheKey& key) {
    fmt::print(os, "x={}, y={}", key.x, key.y);
    return os;
}

bool TileCache::Peek(std::vector<float>& tile_data, Key key) {
    // This is a read-only operation which it is safe to do in parallel.
    if (_map.find(key) == _map.end()) {
        return false;
    } else {
        auto tile = UnsafePeek(key);
        CopyTileData(tile_data, tile);
        return true;
    }
}

bool TileCache::Get(std::vector<float>& tile_data, Key key, std::shared_ptr<carta::FileLoader> loader, std::mutex& image_mutex) {
    // Will be loaded or retrieved from cache
    bool valid(1);

    std::unique_lock<std::mutex> guard(_tile_cache_mutex);

    if (_map.find(key) == _map.end()) { // Not in cache
        // Load 2x2 chunk of tiles from image
        valid = LoadChunk(ChunkKey(key), loader, image_mutex);
    } else {
        Touch(key);
    }

    if (valid) {
        CopyTileData(tile_data, UnsafePeek(key));
    }

    return valid;
}

bool TileCache::GetMultiple(
    std::unordered_map<Key, std::vector<float>>& tiles, std::shared_ptr<carta::FileLoader> loader, std::mutex& image_mutex) {
    std::vector<Key> found;
    std::vector<Key> not_found;
    bool valid(1);

    std::unique_lock<std::mutex> guard(_tile_cache_mutex);

    for (auto& kv : tiles) {
        auto& key = kv.first;
        if (_map.find(key) == _map.end()) {
            not_found.push_back(key);
        } else {
            found.push_back(key);
        }
    }

    // First get all the tiles which are in the cache, in parallel
#pragma omp parallel for
    for (auto& key : found) {
        CopyTileData(tiles[key], UnsafePeek(key));
    }

    for (auto& key : found) {
        Touch(key);
    }

    // Then process each new chunk serially
    std::unordered_map<Key, std::vector<Key>> chunk_tiles;

    for (auto& key : not_found) {
        chunk_tiles[ChunkKey(key)].push_back(key);
    }

    for (auto& kv : chunk_tiles) {
        // load the chunk
        valid = valid && LoadChunk(kv.first, loader, image_mutex);

        // get the tiles (up to 4, probably 2) in parallel
#pragma omp parallel for
        for (auto& key : kv.second) {
            CopyTileData(tiles[key], UnsafePeek(key));
        }
    }

    return valid;
}

void TileCache::Reset(int32_t channel, int32_t stokes, int capacity) {
    std::unique_lock<std::mutex> guard(_tile_cache_mutex);
    if (capacity > 0) {
        _capacity = capacity;
    }
    _map.clear();
    _queue.clear();
    _channel = channel;
    _stokes = stokes;
}

void TileCache::CopyTileData(std::vector<float>& tile_data, TilePtr tile) {
    tile_data.resize(tile->size());
    std::copy(tile->begin(), tile->end(), tile_data.begin());
}

TileCache::TilePtr TileCache::UnsafePeek(Key key) {
    // Assumes that the tile is in the cache
    return _map.find(key)->second->second;
}

void TileCache::Touch(Key key) {
    // Move tile to the front of the queue
    // Assumes that the tile is in the cache
    auto tile = _map.find(key)->second->second;
    _queue.erase(_map.find(key)->second);
    _queue.push_front(std::make_pair(key, tile));
    _map[key] = _queue.begin();
}

TileCache::Key TileCache::ChunkKey(Key tile_key) {
    return Key((tile_key.x / _CHUNK_SIZE) * _CHUNK_SIZE, (tile_key.y / _CHUNK_SIZE) * _CHUNK_SIZE);
}

bool TileCache::LoadChunk(Key chunk_key, std::shared_ptr<carta::FileLoader> loader, std::mutex& image_mutex) {
    // load a chunk from the file
    std::vector<float> chunk;
    int data_width;
    int data_height;

    if (!loader->GetChunk(chunk, data_width, data_height, chunk_key.x, chunk_key.y, _channel, _stokes, image_mutex)) {
        return false;
    };

    // split the chunk into four tiles
    std::vector<TilePtr> tiles;
    std::vector<int> tile_widths;
    std::vector<int> tile_heights;

    for (int tile_row = 0; tile_row < 2; tile_row++) {
        for (int tile_col = 0; tile_col < 2; tile_col++) {
            tile_widths.push_back(std::max(0, std::min(TILE_SIZE, data_width - tile_col * TILE_SIZE)));
            tile_heights.push_back(std::max(0, std::min(TILE_SIZE, data_height - tile_row * TILE_SIZE)));
            tiles.push_back(std::make_shared<std::vector<float>>(tile_widths.back() * tile_heights.back(), NAN));
        }
    }

    std::function<void(TileIter, int, TileIter&)> do_nothing = [&](TileIter start, int width, TileIter& destination) {};

    std::function<void(TileIter, int, TileIter&)> do_copy = [&](TileIter start, int width, TileIter& destination) {
        std::copy(start, start + width, destination);
        std::advance(destination, width);
    };

    auto left_copy = do_copy;
    auto right_copy = (data_width > TILE_SIZE) ? do_copy : do_nothing;

    auto pos = chunk.begin();
    auto row_read_end = pos;

    for (int tr : {0, 1}) {
        auto row_height = tile_heights[tr * 2];

        if (!row_height) {
            continue;
        }

        auto left = tiles[tr * 2]->begin();
        auto right = tiles[tr * 2 + 1]->begin();

        auto left_width = tile_widths[tr * 2];
        auto right_width = tile_widths[tr * 2 + 1];

        row_read_end += data_width * row_height;

        while (pos < row_read_end) {
            left_copy(pos, left_width, left);
            right_copy(pos + TILE_SIZE, right_width, right);
            std::advance(pos, data_width);
        }
    }

    // insert the 4 tiles into the cache
    std::vector<std::pair<int, int>> offsets = {{0, 0}, {TILE_SIZE, 0}, {0, TILE_SIZE}, {TILE_SIZE, TILE_SIZE}};
    auto tile_offset = offsets.begin();
    for (auto& t : tiles) {
        Key key(chunk_key.x + tile_offset->first, chunk_key.y + tile_offset->second);

        if (key.x < chunk_key.x + data_width && key.y < chunk_key.y + data_height) {
            // this tile is within the bounds of the image

            // If the tile is not in the map
            if (_map.find(key) == _map.end()) { // add if not found
                // Evict oldest tile if necessary
                if (_map.size() == _capacity) {
                    _map.erase(_queue.back().first);
                    _queue.pop_back();
                }

                // Insert the new tile
                _queue.push_front(std::make_pair(key, t));
                _map[key] = _queue.begin();

            } else { // touch the tile
                Touch(key);
            }
        }

        std::advance(tile_offset, 1);
    }

    return true;
}

const int TileCache::_TILE_SQ = TILE_SIZE * TILE_SIZE;
const int TileCache::_CHUNK_SIZE = TILE_SIZE * 2;
const int TileCache::_CHUNK_SQ = TILE_SIZE * TILE_SIZE * 4;