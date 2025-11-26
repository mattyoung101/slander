# Slander - (System)Verilog Minimiser and Mutator
**Slander** is a tool that aims to both _minimise_ and _mutate_ SystemVerilog and Verilog code for the
purposes of fuzzing. This is helpful for reporting, reproducing and exploring crashes generated in EDA tools
through programs such as [Verismith](https://github.com/ymherklotz/verismith).

Minimisation is a non-trivial problem with a large state-space, but Slander aims to make a best-effort basis
at comprehensive and performant minimisation.

Slander is based on Mike Popoloski's excellent [Slang](https://github.com/MikePopoloski/slang) SystemVerilog
frontend.

It is possible, although not _currently_ planned, that Slander may one day also be able to generate random
(System)Verilog code by doing random walks over the Slang AST.

### Current state
Slander is work in progress.

### Timeline

## Building and running
### Building and environment
**TODO**

### Running
TODO

## Licence
Copyright (c) 2025 M. L. Young. Available under the Mozilla Public License v2.0

> This Source Code Form is subject to the terms of the Mozilla Public
> License, v. 2.0. If a copy of the MPL was not distributed with this
> file, You can obtain one at https://mozilla.org/MPL/2.0/.
