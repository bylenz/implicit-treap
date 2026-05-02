#!/bin/bash
set -e

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"

echo "Generando caso grande (seed=42, n=q=500000)..."
"$BUILD_DIR/gen_big" 42 >"$BUILD_DIR/big.in"

echo "Corriendo main..."
time "$BUILD_DIR/main" <"$BUILD_DIR/big.in" >"$BUILD_DIR/big.out"

echo "Lineas de salida:"
wc -l "$BUILD_DIR/big.out"
