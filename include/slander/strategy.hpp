// Slander: (System)Verilog minimiser and mutator
//
// Copyright (c) 2025 M. L. Young.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL
// was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#pragma once
#include "slang/syntax/SyntaxTree.h"
#include "slang/syntax/SyntaxVisitor.h"
#include "slang/syntax/AllSyntax.h"
#include <cstddef>
#include <memory>

namespace slander {

using namespace slang::syntax;

using SyntaxTreePtr = std::shared_ptr<SyntaxTree>;

/// A strategy that can be employed to minimise a SV/V document
struct MinimisationStrategy {
public:
    virtual ~MinimisationStrategy() = default;
    MinimisationStrategy() = default;
    MinimisationStrategy(const MinimisationStrategy &) = default;
    MinimisationStrategy(MinimisationStrategy &&) = default;
    MinimisationStrategy &operator=(const MinimisationStrategy &) = default;
    MinimisationStrategy &operator=(MinimisationStrategy &&) = default;

    /// Returns the number of actions that this minimisation strategy can take; e.g.
    /// RemoveProcessMinimisationStrategy would return the number of processes in the document
    virtual size_t proposeActions(const SyntaxTreePtr &tree) = 0;

    /// Performs the action on the syntax tree, e.g. RemoveProcessMinimisationStrategy should remove the Nth
    /// process from the document, where N = 'action'
    virtual SyntaxTreePtr act(SyntaxTreePtr &tree, size_t action) = 0;

    using Ptr = std::unique_ptr<MinimisationStrategy>;
};

/// Strategy that removes processes (always_ff, etc) from the document
struct RemoveProcessMinimisationStrategy : public MinimisationStrategy {
public:
    RemoveProcessMinimisationStrategy() = default;

    size_t proposeActions(const SyntaxTreePtr &tree) override;

    SyntaxTreePtr act(SyntaxTreePtr &tree, size_t action) override;

    struct Proposer : SyntaxRewriter<Proposer> {
        void handle(const ProceduralBlockSyntax &block);
        size_t count = 0;
    };

    struct Editor : SyntaxRewriter<Editor> {
        public:
            Editor(size_t which) : which(which) {}
            void handle(const ProceduralBlockSyntax &block);

        private:
            size_t which = 0;
            size_t count = 0;

    };
};

} // namespace slander
