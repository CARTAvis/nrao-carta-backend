/* This file is part of the CARTA Image Viewer: https://github.com/CARTAvis/carta-backend
   Copyright 2018, 2019, 2020 Academia Sinica Institute of Astronomy and Astrophysics (ASIAA),
   Associated Universities, Inc. (AUI) and the Inter-University Institute for Data Intensive Astronomy (IDIA)
   SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "Tile.h"

std::ostream& operator<<(std::ostream& os, const Tile& tile) {
    fmt::print(os, "x={}, y={}, layer={}", tile.x, tile.y, tile.layer);
    return os;
}
