// Slander: (System)Verilog minimiser and mutator
//
// Copyright (c) 2025 M. L. Young.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL
// was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#include "slander/strategy.hpp"
#include <slang/syntax/AllSyntax.h>
#include <slang/syntax/SyntaxTree.h>
#include <spdlog/spdlog.h>

using namespace slander;
using namespace slang::syntax;

size_t RemoveProcessMinimisationStrategy::proposeActions(const SyntaxTree &tree) {
    // TODO use the proposer
    return 0;
}

void RemoveProcessMinimisationStrategy::act(SyntaxTree &tree, size_t action) {
    // TODO use the editor and then the repairer
}

void RemoveProcessMinimisationStrategy::Proposer::visit(const ProceduralBlockSyntax &block) {
    count++;
}

void RemoveProcessMinimisationStrategy::Editor::visit(const ProceduralBlockSyntax &block) {
    if (count == which) {
        SPDLOG_DEBUG("Removing block at idx {}: {}", count, block.toString());
        remove(block);
    }
    count++;
}
