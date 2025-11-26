// Slander: (System)Verilog minimiser and mutator
//
// Copyright (c) 2025 M. L. Young.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL
// was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#pragma once
#include "slang/syntax/SyntaxVisitor.h"

namespace slander {

using namespace slang::syntax;

class MinimisationStrategy { };

/// Strategy that removes processes
class RemoveProcessMinimisationStrategy : public MinimisationStrategy {
public:
    class Editor : SyntaxRewriter<Editor> {

    };

    class Repairer : SyntaxRewriter<Editor> {  };
};

} // namespace slander
