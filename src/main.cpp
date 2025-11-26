// Slander: (System)Verilog minimiser and mutator
//
// Copyright (c) 2025 M. L. Young.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL
// was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#include "CLI/CLI.hpp"
#include "slander/slander.hpp"
#include "slander/strategy.hpp"
#include "slang/util/VersionInfo.h"
#include "spdlog/spdlog.h"
#include <csignal>
#include <spdlog/sinks/ansicolor_sink.h>
#include <unistd.h>
#include "slang/syntax/SyntaxVisitor.h"

constexpr std::string SLANDER_VERSION = "1.0.0";

namespace {

void sigIntHandler(int signal) {
    SPDLOG_INFO("Caught SIGINT");

    // TODO flush index and so on

    exit(0);
}

using namespace slang::syntax;

// FIXME we need to figure out how to have like an array of multiple templates

// void registerEditors(std::vector<SyntaxVisitor> &out)

} // namespace

int main(int argc, char *argv[]) {
    using namespace slang;

    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::trace);

    signal(SIGINT, sigIntHandler);

    spdlog::default_logger()->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%# %!] %v");
    for (auto &sink : spdlog::default_logger()->sinks()) {
        sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%# %!] %v");
    }

    CLI::App app { "(System)Verilog minimiser and mutator" };
    argv = app.ensure_utf8(argv);

    std::filesystem::path input;
    std::string programCmdLine;
    bool shouldPrintVersion = false;

    app.add_option("-f,--file", input, "Input SystemVerilog/Verilog file")
        ->required()
        ->check(CLI::ExistingFile);
    app.add_option("-p,--program", programCmdLine,
           "Program command line to check if the minimised document still causes the intended bug. A "
           "temporary "
           "path to the minimised document will be appended to the command line. The program should return 0 "
           "if no bug exists. Anything else is treated as the bug still existing.")
        ->required();
    app.add_flag("-v,--version", shouldPrintVersion, "Print version and exit.");

    CLI11_PARSE(app, argc, argv);

    if (shouldPrintVersion) {
        SPDLOG_INFO("Slander v{} - (c) 2025 M. L. Young. Licenced under the MPL 2.0.", SLANDER_VERSION);
        SPDLOG_INFO("Using Slang version: {}.{}", VersionInfo::getMajor(), VersionInfo::getMinor());
        return 0;
    }

    slander::RemoveProcessMinimisationStrategy::Editor editor;

    return 0;
}
