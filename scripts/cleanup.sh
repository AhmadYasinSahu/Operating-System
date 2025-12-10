#!/bin/bash

# Compiler Simulator - Cleanup Script
# Removes all build artifacts and IPC resources

echo "Cleaning up Compiler Simulator..."

# Remove executables
if [ -d "bin" ]; then
    rm -f bin/*
    echo "✓ Removed executables"
fi

# Remove logs
if [ -d "logs" ]; then
    rm -f logs/*.log logs/*.txt
    echo "✓ Removed log files"
fi

# Remove output files
if [ -d "data/output" ]; then
    rm -f data/output/*
    echo "✓ Removed output files"
fi

# Clean up shared memory
echo "Cleaning up IPC resources..."
ipcs -m | grep "compiler" | awk '{print $2}' | xargs -r ipcrm -m 2>/dev/null
echo "✓ Shared memory cleaned"

# Clean up message queues
ipcs -q | grep "compiler" | awk '{print $2}' | xargs -r ipcrm -q 2>/dev/null
echo "✓ Message queues cleaned"

echo ""
echo "Cleanup complete!"
