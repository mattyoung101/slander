// Slander: (System)Verilog minimiser and mutator
//
// Copyright (c) 2025 M. L. Young.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL
// was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#include "slander/strategy.hpp"
#include <slang/syntax/SyntaxTree.h>

using namespace slander;
using namespace slang::syntax;

size_t RemoveProcessMinimisationStrategy::proposeActions(const SyntaxTree &tree) {
    return 0;
}

void RemoveProcessMinimisationStrategy::act(SyntaxTree &tree, size_t action) {
}
