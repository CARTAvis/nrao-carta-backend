/* This file is part of the CARTA Image Viewer: https://github.com/CARTAvis/carta-backend
   Copyright 2018, 2019, 2020, 2021 Academia Sinica Institute of Astronomy and Astrophysics (ASIAA),
   Associated Universities, Inc. (AUI) and the Inter-University Institute for Data Intensive Astronomy (IDIA)
   SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef CARTA_BACKEND_FILELIST_FITSHDULIST_H_
#define CARTA_BACKEND_FILELIST_FITSHDULIST_H_

#include <casacore/fits/FITS/FITSError.h>
#include <casacore/fits/FITS/hdu.h>

#include <carta-protobuf/defs.pb.h>

#include "../Logger/Logger.h"

inline void FitsInfoErrHandler(const char* err_message, casacore::FITSError::ErrorLevel severity) {
    if (severity > casacore::FITSError::WARN) {
        spdlog::error(err_message);
    }
}

class FitsHduList {
public:
    FitsHduList(const std::string& filename);
    void GetHduList(std::vector<std::string>& hdu_list);

private:
    bool IsImageHdu(casacore::FITS::HDUType hdu_type);
    void GetFitsHduInfo(casacore::FitsInput& fits_input, int& ndim, std::string& ext_name);

    std::string _filename;
};

#endif // CARTA_BACKEND_FILELIST_FITSHDULIST_H_
