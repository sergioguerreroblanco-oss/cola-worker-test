# Documentation

This project uses **Doxygen** to generate API documentation from the annotated source code.

## Requirements
- Doxygen
- (Optional) Graphviz for diagrams

## On Windows:

1. Install Doxygen for Windows (`https://www.doxygen.nl/download.html`)
	
	During setup, check “Add doxygen to system PATH

2. (Optional) Install Graphviz (`https://graphviz.org/download/`) and add its bin/ folder to PATH.

3.Open PowerShell in the project root and run:

```PowerShell
cd docs
doxygen Doxyfile
```

4. Open documentation in your browser:
```PowerShell
start docs\html\index.html
```

## On Debian/Ubuntu:

```bash
sudo apt install doxygen graphviz
```

Generate

```bash
./scripts/generate_docs.sh
```

## Output

HTML: docs/html/index.html

(Optional) LaTeX: docs/latex/ (if enabled in Doxyfile)