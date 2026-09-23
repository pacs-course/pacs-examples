#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
cd "$SCRIPT_DIR"

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release >/dev/null
cmake --build build >/dev/null

echo "Running forward-mode demo..."
./build/autodiff_forward_demo

echo

echo "Running reverse-mode demo..."
./build/autodiff_reverse_demo
