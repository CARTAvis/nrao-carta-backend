/* This file is part of the CARTA Image Viewer: https://github.com/CARTAvis/carta-backend
   Copyright 2018, 2019, 2020, 2021 Academia Sinica Institute of Astronomy and Astrophysics (ASIAA),
   Associated Universities, Inc. (AUI) and the Inter-University Institute for Data Intensive Astronomy (IDIA)
   SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef CARTA_BACKEND_SRC_SESSIONMANAGER_PROGRAMSETTINGS_H_
#define CARTA_BACKEND_SRC_SESSIONMANAGER_PROGRAMSETTINGS_H_

#include <string>
#include <tuple>
#include <vector>

#include "Constants.h"

namespace carta {
struct ProgramSettings {
    bool version = false;
    bool help = false;
    int port = -1;
    int grpc_port = -1;
    int omp_thread_count = OMP_THREAD_COUNT;
    std::string top_level_folder = "/";
    std::string starting_folder = ".";
    std::string host = "0.0.0.0";
    std::vector<std::string> files;
    std::string frontend_folder;
    bool no_http = false;
    bool debug_no_auth = false;
    bool no_browser = false;
    bool no_log = false;
    bool log_performance = false;
    bool log_protocol_messages = false;
    int verbosity = 4;
    int wait_time = -1;
    int init_wait_time = -1;
    int idle_session_wait_time = -1;

    ProgramSettings() = default;
    ProgramSettings(int argc, char** argv);

    auto GetTuple() const {
        return std::tie(help, version, port, grpc_port, omp_thread_count, top_level_folder, starting_folder, host, files, frontend_folder,
            no_http, no_browser, no_log, log_performance, log_protocol_messages, debug_no_auth, verbosity, wait_time, init_wait_time,
            idle_session_wait_time);
    }
    bool operator!=(const ProgramSettings& rhs) const;
    bool operator==(const ProgramSettings& rhs) const;
};
} // namespace carta
#endif // CARTA_BACKEND_SRC_SESSIONMANAGER_PROGRAMSETTINGS_H_
