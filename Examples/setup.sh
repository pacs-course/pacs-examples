#!/bin/bash
#
# Tool to compile the basic stuff using CMake
#
set -euo pipefail

build_and_install() {
  local dir="$1"
  (cd "$dir" && cmake -S . -B build && cmake --build build && cmake --install build)
}

build_and_install src/Utilities
build_and_install src/LinearAlgebraUtil
