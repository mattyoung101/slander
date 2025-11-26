// Slander: (System)Verilog minimiser and mutator
//
// Copyright (c) 2025 M. L. Young.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL
// was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#include "slander/strategy.hpp"
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_test_run_info.hpp>
#include <catch2/catch_test_case_info.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <cctype>
#include <slang/syntax/SyntaxPrinter.h>
#include <slang/syntax/SyntaxTree.h>
#include <spdlog/sinks/sink.h>
#include <spdlog/spdlog.h>

using namespace slang;
using namespace slang::syntax;
using namespace slander;

class TestRunListener : public Catch::EventListenerBase {
public:
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(Catch::TestRunInfo const & /*testRunInfo*/) override {
        spdlog::set_level(spdlog::level::trace);
        spdlog::flush_on(spdlog::level::trace);

        spdlog::default_logger()->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%# %!] %v");
        for (auto &sink : spdlog::default_logger()->sinks()) {
            sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%# %!] %v");
        }
    }

    void testCaseStarting(Catch::TestCaseInfo const &testInfo) override {
        SPDLOG_INFO("===== BEGIN: {} =====", testInfo.name);
    }

    void testCaseEnded(Catch::TestCaseStats const &testCaseStats) override {
        SPDLOG_INFO("===== END: {} =====", testCaseStats.testInfo->name);
    }
};

CATCH_REGISTER_LISTENER(TestRunListener);

constexpr std::string strip(const std::string &str) {
    std::string local = str;
    // NOLINTNEXTLINE don't care about fuckin std::ranges man
    local.erase(std::remove_if(local.begin(), local.end(), isspace), local.end());
    return local;
}

TEST_CASE("Single process block is removed", "[RemoveProcessMinimiser]") {
    auto tree = SyntaxTree::fromText(R"(
module top(
    input logic clk,
    input logic a,
    output logic b
);

always_ff @(posedge clk) begin
    b <= a;
end

endmodule
)");
    REQUIRE(tree->diagnostics().empty());

    RemoveProcessMinimiser strat;
    auto count = strat.proposeActions(tree);
    REQUIRE(count == 1);

    auto result = strat.act(tree, 0);
    REQUIRE(result->diagnostics().empty());

    REQUIRE(strip(SyntaxPrinter::printFile(*result)) == strip(R"(
module top(
    input logic clk,
    input logic a,
    output logic b
);

endmodule
    )"));
}

TEST_CASE("Second process block is removed", "[RemoveProcessMinimiser]") {
    auto tree = SyntaxTree::fromText(R"(
module top(
    input logic clk,
    input logic a,
    output logic b
);

always_ff @(posedge clk) begin: block1
    b <= a;
end

always_ff @(posedge clk) begin: block2
    b <= a;
end

endmodule
)");
    REQUIRE(tree->diagnostics().empty());

    RemoveProcessMinimiser strat;
    auto count = strat.proposeActions(tree);
    REQUIRE(count == 2);

    auto result = strat.act(tree, 0);
    REQUIRE(result->diagnostics().empty());

    REQUIRE(strip(SyntaxPrinter::printFile(*result)) == strip(R"(
module top(
    input logic clk,
    input logic a,
    output logic b
);

always_ff @(posedge clk) begin: block2
    b <= a;
end

endmodule
    )"));
}

TEST_CASE("Single blocking assign is removed", "[RemoveAssignMinimiser]") {
    auto tree = SyntaxTree::fromText(R"(
module top(
    input logic clk,
    input logic a,
    output logic out1
);

always_comb begin
    out1 = a;
end

endmodule
)");
    REQUIRE(tree->diagnostics().empty());

    RemoveAssignMinimiser strat;
    auto count = strat.proposeActions(tree);
    REQUIRE(count == 1);

    auto result = strat.act(tree, 0);
    REQUIRE(result->diagnostics().empty());

    REQUIRE(strip(SyntaxPrinter::printFile(*result)) == strip(R"(
module top(
    input logic clk,
    input logic a,
    output logic out1
);

always_comb begin
end

endmodule
    )"));
}

TEST_CASE("Second blocking assign is removed", "[RemoveAssignMinimiser]") {
    auto tree = SyntaxTree::fromText(R"(
module top(
    input logic clk,
    input logic a,
    output logic out1,
    output logic out2
);

always_comb begin
    out1 = a;
end

always_ff @(posedge clk) begin
    out2 <= a;
end

endmodule
)");
    REQUIRE(tree->diagnostics().empty());

    RemoveAssignMinimiser strat;
    auto count = strat.proposeActions(tree);
    REQUIRE(count == 2);

    auto result = strat.act(tree, 0);
    REQUIRE(result->diagnostics().empty());

    REQUIRE(strip(SyntaxPrinter::printFile(*result)) == strip(R"(
module top(
    input logic clk,
    input logic a,
    output logic out1,
    output logic out2
);

always_comb begin
end

always_ff @(posedge clk) begin
    out2 <= a;
end

endmodule
    )"));
}
