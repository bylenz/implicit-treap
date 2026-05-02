#!/bin/bash
# Stress test: corre N casos aleatorios y compara main vs brute.
# Uso: ./tests/stress.sh [N]   (default: 200)

set -e

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"
TESTS_DIR="$ROOT_DIR/tests"

MAIN="$BUILD_DIR/main"
BRUTE="$BUILD_DIR/brute"
GEN="$BUILD_DIR/gen"

if [[ ! -x "$MAIN" || ! -x "$BRUTE" || ! -x "$GEN" ]]; then
  echo "Error: faltan binarios. Compila primero con: cmake --build build"
  exit 1
fi

N=${1:-200}
TMP=$(mktemp -d)
trap "rm -rf $TMP" EXIT

echo "Corriendo $N casos aleatorios..."
for i in $(seq 1 $N); do
  "$GEN" $i >"$TMP/test.in"
  "$MAIN" <"$TMP/test.in" >"$TMP/main.got"
  "$BRUTE" <"$TMP/test.in" >"$TMP/brute.got"
  if ! diff -q "$TMP/main.got" "$TMP/brute.got" >/dev/null; then
    echo "✗ FAIL en seed $i"
    echo "--- Input ---"
    cat "$TMP/test.in"
    echo "--- Main output ---"
    cat "$TMP/main.got"
    echo "--- Brute output (correcto) ---"
    cat "$TMP/brute.got"
    exit 1
  fi
  if ((i % 20 == 0)); then
    echo "  ... $i casos OK"
  fi
done

echo "✓ Todos los $N casos pasaron"
