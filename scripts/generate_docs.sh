#!/usr/bin/env bash
set -e

PROJECT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"

DOXYGEN_CMD=$(command -v doxygen || true)

if [ -z "$DOXYGEN_CMD" ]; then
  echo "Error: doxygen not found. Please install it first."
  echo "On Ubuntu/Debian: sudo apt install doxygen graphviz"
  exit 1
fi

echo "Generating documentation..."
cd "$PROJECT_ROOT/docs"
doxygen Doxyfile

echo "Documentation generated in docs/html/index.html"
