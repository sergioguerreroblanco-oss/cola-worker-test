#!/usr/bin/env bash
set -e

echo "Sanitizing source files (replace curly quotes with ASCII)..."
find include src -type f -exec sed -i \
    -e "s/’/'/g" \
    -e "s/‘/'/g" \
    -e 's/“/"/g' \
    -e 's/”/"/g' \
    {} +

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

# Compile PDF (LaTeX -> PDF)
if [ -d "latex" ]; then
  echo "Compiling LaTeX to PDF..."
  make -C latex
fi

echo "Documentation generated in docs/html and docs/latex/refman.pdf"
