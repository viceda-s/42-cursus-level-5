#!/bin/bash
# Test script to verify graceful shutdown works quickly

echo "Starting webserv in background..."
./webserv config/default.conf &
SERVER_PID=$!

echo "Server PID: $SERVER_PID"
sleep 1

echo "Sending SIGINT (Ctrl+C) to server..."
START=$(date +%s%N)
kill -INT $SERVER_PID

# Wait for server to exit
wait $SERVER_PID 2>/dev/null
EXIT_CODE=$?

END=$(date +%s%N)
ELAPSED=$(( ($END - $START) / 1000000 ))

echo ""
echo "Server exit code: $EXIT_CODE"
echo "Shutdown time: ${ELAPSED}ms"

if [ $ELAPSED -lt 2000 ]; then
    echo "✓ Shutdown completed quickly (< 2 seconds)"
    exit 0
else
    echo "✗ Shutdown took too long (>= 2 seconds)"
    exit 1
fi

