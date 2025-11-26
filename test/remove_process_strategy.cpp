// Slander: (System)Verilog minimiser and mutator
//
// Copyright (c) 2025 M. L. Young.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL
// was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#include "slander/strategy.hpp"
#include "slang/syntax/AllSyntax.h"
#include "slang/syntax/SyntaxPrinter.h"
#include "slang/syntax/SyntaxTree.h"
#include <algorithm>
#include <iostream>
#include <spdlog/spdlog.h>

using namespace slang;
using namespace slang::syntax;
using namespace slander;

#include <catch2/catch_test_macros.hpp>

constexpr std::string strip(const std::string &str) {
    std::string local;
    std::ranges::remove_if(local.begin(), local.end(), isspace);
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

    RemoveProcessMinimiser strat;
    auto count = strat.proposeActions(tree);
    REQUIRE(count == 1);

    auto result = strat.act(tree, 0);
    std::cout << strip(SyntaxPrinter::printFile(*result)) << std::endl;

    REQUIRE(strip(SyntaxPrinter::printFile(*result)) == strip(R"(
module top(
    input logic clk,
    input logic a,
    output logic b
);

endmodule
    )"));
}
