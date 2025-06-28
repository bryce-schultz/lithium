#!/bin/bash

echo "Setting up development environment for Lithium..."

# Check if python3 is available
if ! command -v python3 &> /dev/null; then
    echo "Error: python3 is required but not installed."
    exit 1
fi

# Check if pip is available
if ! command -v pip3 &> /dev/null; then
    echo "Error: pip3 is required but not installed."
    exit 1
fi

# Install Python dependencies
echo "Installing Python testing dependencies..."
pip3 install -r requirements-dev.txt

# Build the interpreter
echo "Building Lithium interpreter..."
make -C src

# Verify setup
echo "Verifying setup..."
if [ -x "src/li" ]; then
    echo "✓ Interpreter built successfully"
else
    echo "✗ Failed to build interpreter"
    exit 1
fi

if python3 -c "import pytest" 2>/dev/null; then
    echo "✓ Pytest installed successfully"
else
    echo "✗ Failed to install pytest"
    exit 1
fi

echo ""
echo "Setup complete! You can now:"
echo "1. Open VSCode and reload the window"
echo "2. Use Ctrl+Shift+P → 'Python: Refresh Tests'"
echo "3. Open the Test Explorer panel"
echo "4. Run tests from the VSCode Test Explorer"
