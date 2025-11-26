// Slander: (System)Verilog minimiser and mutator
//
// Copyright (c) 2025 M. L. Young.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL
// was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

namespace slander {

enum class SearchStrategy : uint8_t {
    /// Evaluate all possible minimisations
    Exhaustive,

    /// Search randomly for a given number of attempts
    RandomAttempts,

    /// Search randomly for a given time in seconds approximately
    RandomTimeout,
};

} // namespace slander
