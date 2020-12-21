/* This file is part of the CARTA Image Viewer: https://github.com/CARTAvis/carta-backend
   Copyright 2018, 2019, 2020 Academia Sinica Institute of Astronomy and Astrophysics (ASIAA),
   Associated Universities, Inc. (AUI) and the Inter-University Institute for Data Intensive Astronomy (IDIA)
   SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef CARTA_BACKEND__IMAGEDATA_CONCATSTOKESFILES_H_
#define CARTA_BACKEND__IMAGEDATA_CONCATSTOKESFILES_H_

#include <carta-protobuf/concat_stokes_files.pb.h>
#include <casacore/images/Images/ExtendImage.h>
#include <casacore/images/Images/ImageConcat.h>

#include "FileLoader.h"

namespace carta {

class ConcatStokesFiles {
public:
    ConcatStokesFiles(const std::string& root_folder);
    ~ConcatStokesFiles();

    bool DoConcat(const CARTA::ConcatStokesFiles& message, CARTA::ConcatStokesFilesAck& response,
        std::shared_ptr<casacore::ImageConcat<float>>& image, std::string& file_name);

private:
    bool OpenStokesFiles(const CARTA::ConcatStokesFiles& message, std::string& err);
    bool StokesFilesValid(std::string& err);
    bool GetStokesType(const CARTA::StokesType& stokes_type, casacore::Stokes::StokesTypes& result);
    void ClearCache();

    std::string _root_folder;
    std::string _file_name; // name of the new concatenate file
    std::unordered_map<CARTA::StokesType, std::unique_ptr<carta::FileLoader>> _loaders;
    std::unordered_map<CARTA::StokesType, std::shared_ptr<casacore::ExtendImage<float>>> _extent_images;
    std::unordered_map<CARTA::StokesType, casacore::CoordinateSystem> _coord_sys;
};

} // namespace carta

#endif // CARTA_BACKEND__IMAGEDATA_CONCATSTOKESFILES_H_