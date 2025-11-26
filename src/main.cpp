// Slander: (System)Verilog minimiser and mutator
//
// Copyright (c) 2025 M. L. Young.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL
// was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#include "CLI/CLI.hpp"
#include "slander/slander.hpp"
#include "slander/strategy.hpp"
#include "spdlog/spdlog.h"
#include <csignal>
#include <cstdlib>
#include <memory>
#include <random>
#include <slang/syntax/SyntaxTree.h>
#include <slang/util/Util.h>
#include <slang/util/VersionInfo.h>
#include <spdlog/sinks/ansicolor_sink.h>
#include <unistd.h>

constexpr std::string SLANDER_VERSION = "0.0.1";

namespace {

void sigIntHandler(int signal) {
    SPDLOG_INFO("Caught SIGINT");

    // TODO flush index and so on

    exit(0);
}

using namespace slang::syntax;
using namespace slander;

std::unordered_map<std::string, SearchStrategy> SEARCH_STRATEGY_VALUE = {
    { "exhaustive", SearchStrategy::Exhaustive },
    { "random-timeout", SearchStrategy::RandomTimeout },
    { "random-attempts", SearchStrategy::RandomAttempts },
};

std::vector<MinimisationStrategy::Ptr> registerStrategies() {
    std::vector<MinimisationStrategy::Ptr> out;
    out.push_back(std::make_unique<RemoveAssignMinimiser>());
    out.push_back(std::make_unique<RemoveProcessMinimiser>());
    return out;
}

} // namespace

int main(int argc, char *argv[]) {
    using namespace slang;

    signal(SIGINT, sigIntHandler);

    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::trace);

    spdlog::default_logger()->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%# %!] %v");
    for (auto &sink : spdlog::default_logger()->sinks()) {
        sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%# %!] %v");
    }

    CLI::App app { "Slander v" + SLANDER_VERSION + ": (System)Verilog minimiser and mutator" };
    argv = app.ensure_utf8(argv);

    std::filesystem::path input;
    std::string programCmdLine;
    SearchStrategy searchStrat {};

    std::random_device rd;
    uint64_t seed = rd();

    app.add_option("file,-f,--file", input, "Input SystemVerilog/Verilog file")
        ->required()
        ->check(CLI::ExistingFile);
    app.add_option("-p,--program", programCmdLine,
           "Program command line to check if the minimised document still causes the intended bug. A "
           "temporary "
           "path to the minimised document will be appended to the command line. The program should return 0 "
           "if no bug exists. Anything else is treated as the bug still existing.")
        ->required();
    app.add_option("-s,--strategy", searchStrat,
           "Search strategy to use. 'exhausitive' will try all possible minimisations. 'random-attempts' and "
           "'random-timeout' will randomly try for a given number of attempts or a timeout in seconds, "
           "respectively.")
        ->required()
        ->transform(CLI::CheckedTransformer(SEARCH_STRATEGY_VALUE, CLI::ignore_case));
    app.add_option(
        "--seed", seed, "Fixed RNG seed for reproducibility. If not specified, a random seed is used.");
    app.set_version_flag("-v,--version",
        fmt::format("Slander v{} - (c) 2025 M. L. Young. Licenced under the MPL 2.0.\nUsing Slang v{}.{}",
            SLANDER_VERSION, VersionInfo::getMajor(), VersionInfo::getMinor()));

    CLI11_PARSE(app, argc, argv);

    auto maybeTree = SyntaxTree::fromFile(input.string());
    if (!maybeTree.has_value()) {
        SPDLOG_ERROR("Failed to parse document: {}", maybeTree.error().first.message());
        return 1;
    }
    // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
    SyntaxTreePtr originalTree = maybeTree.value();

    switch (searchStrat) {
        case SearchStrategy::Exhaustive:
            // NOTE this isn't really exhaustive, we're just applying each strategy. we don't consider
            // applying multiple strategies one after the other, which we should probably do, but that's a
            // hard-ish problem to solve, because all permutations of N passes would be N factorial.
            for (const MinimisationStrategy::Ptr &strat : registerStrategies()) {
                auto actions = strat->proposeActions(originalTree);
                SPDLOG_INFO("{} has {} actions", strat->name(), actions);

                for (size_t i = 0; i < actions; i++) {
                    // create a copy of the tree
                    // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
                    SyntaxTreePtr tree = originalTree;

                    // apply the action
                    auto result = strat->act(tree, i);
                    SPDLOG_DEBUG("Apply action: {}", i);
                }
            }
            break;

        default:
            SPDLOG_ERROR("Unhandled search strategy: {}", static_cast<int>(searchStrat));
            return 1;
    }

    return 0;
}
