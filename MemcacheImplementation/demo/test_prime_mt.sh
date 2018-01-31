#!/bin/bash

SCRATCH=$(mktemp -d)
trap "rm -fr $SCRATCH" INT QUIT TERM EXIT

echo -n "Testing prime (mt) with FIFO ... "
if ./bin/prime 10 64 0 8 > /dev/null; then
    echo Success
else
    echo Failure
fi

echo -n "Testing prime (mt) with Random ... "
if ./bin/prime 10 64 1 8 > /dev/null; then
    echo Success
else
    echo Failure
fi

echo -n "Testing prime (mt) with LRU ... "
if ./bin/prime 10 64 2 8 > /dev/null; then
    echo Success
else
    echo Failure
fi

echo -n "Testing prime (mt) with Clock ... "
if ./bin/prime 10 64 3 8 > /dev/null; then
    echo Success
else
    echo Failure
fi
