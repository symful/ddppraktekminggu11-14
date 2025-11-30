#!/bin/bash

echo "=================================="
echo "  Building and Running Tests"
echo "=================================="
echo ""

mkdir -p users

echo "Building test_category..."
gcc -o tests/test_category tests/test_category.c types/transaction_category.c -I. -lm
if [ $? -ne 0 ]; then
    echo "❌ Failed to compile test_category"
    exit 1
fi

echo "Building test_transaction_group..."
gcc -o tests/test_transaction_group tests/test_transaction_group.c db/transaction_group.c types/transaction_category.c -I. -lm
if [ $? -ne 0 ]; then
    echo "❌ Failed to compile test_transaction_group"
    exit 1
fi

echo ""
echo "=================================="
echo "  Running Tests"
echo "=================================="

./tests/test_category
CATEGORY_RESULT=$?

./tests/test_transaction_group
GROUP_RESULT=$?

echo ""
echo "=================================="
echo "  Test Results"
echo "=================================="

if [ $CATEGORY_RESULT -eq 0 ]; then
    echo "✓ Category tests: PASSED"
else
    echo "❌ Category tests: FAILED"
fi

if [ $GROUP_RESULT -eq 0 ]; then
    echo "✓ Transaction Group tests: PASSED"
else
    echo "❌ Transaction Group tests: FAILED"
fi

echo ""

rm -rf users

if [ $CATEGORY_RESULT -eq 0 ] && [ $GROUP_RESULT -eq 0 ]; then
    echo "✓ All tests passed!"
    exit 0
else
    echo "❌ Some tests failed!"
    exit 1
fi
