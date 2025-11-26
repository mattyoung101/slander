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

size_t RemoveProcessMinimisationStrategy::proposeActions(const SyntaxTreePtr &tree) {
    Proposer proposer;
    proposer.transform(tree);
    return proposer.count;
}

SyntaxTreePtr RemoveProcessMinimisationStrategy::act(SyntaxTreePtr &tree, size_t action) {
    Editor editor(action);
    auto newTree = editor.transform(tree);

    return newTree;
}

void RemoveProcessMinimisationStrategy::Proposer::handle(const ProceduralBlockSyntax &block) {
    count++;
}

void RemoveProcessMinimisationStrategy::Editor::handle(const ProceduralBlockSyntax &block) {
    if (count == which) {
        SPDLOG_DEBUG("Removing block at idx {}: {}", count, block.toString());
        remove(block);
        return;
    }
    count++;
}
