#!/bin/bash

# Process Monitor Script
# Displays system resource usage during compilation

echo "==============================================="
echo "Compiler Simulator - Process Monitor"
echo "==============================================="
echo ""
echo "Monitoring system resources..."
echo ""

# Display current processes
echo "Active Processes:"
ps aux | grep -E "supervisor|lexical|syntax|semantic|code_gen" | grep -v grep

echo ""
echo "Memory Usage:"
free -h

echo ""
echo "Shared Memory Segments:"
ipcs -m

echo ""
echo "Message Queues:"
ipcs -q

echo ""
echo "CPU Information:"
nproc
echo "Cores available"

echo ""
echo "==============================================="
