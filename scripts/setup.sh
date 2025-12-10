#!/bin/bash

# Compiler Simulator - Setup and Build Script
# This script sets up the environment and builds the project

echo "==============================================="
echo "Compiler Process Interaction Simulator"
echo "Setup & Build Script"
echo "==============================================="
echo ""

# Check if we're in the right directory
if [ ! -f "Makefile" ]; then
    echo "Error: Makefile not found. Please run this script from the project root directory."
    exit 1
fi

# Create directories if they don't exist
echo "[1/5] Creating directories..."
mkdir -p bin data/input data/output logs
echo "✓ Directories created"

# Clean previous builds
echo "[2/5] Cleaning previous builds..."
make clean > /dev/null 2>&1
echo "✓ Clean complete"

# Build the project
echo "[3/5] Building project..."
make all
if [ $? -ne 0 ]; then
    echo "✗ Build failed"
    exit 1
fi
echo "✓ Build successful"

# Set execute permissions
echo "[4/5] Setting permissions..."
chmod +x bin/* 2>/dev/null
echo "✓ Permissions set"

# Display build summary
echo "[5/5] Build Summary..."
echo "==============================================="
echo "Executables built:"
ls -lh bin/
echo ""
echo "Project structure:"
find . -type d | head -10
echo ""
echo "==============================================="
echo "Setup complete! Run './bin/supervisor' to start the simulator"
echo "Or use: make run"
echo "==============================================="
