#!/bin/bash

bubble-fifo-output() {
    cat <<EOF
Addresses : 1024
Pages     : 8
VPNShift  : 7
VPNMask   : 0x380
OffsetMask: 0x7F
Hits      : 2073620
Misses    : 1024
EOF
}

bubble-random-output() {
    cat <<EOF
Addresses : 1024
Pages     : 8
VPNShift  : 7
VPNMask   : 0x380
OffsetMask: 0x7F
Hits      : 2073620
Misses    : 1024
EOF
}

SCRATCH=$(mktemp -d)
trap "rm -fr $SCRATCH" INT QUIT TERM EXIT

echo -n "Testing bubble with FIFO ... "
if diff -u <(./bin/bubble 10 128 0 1) <(bubble-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing bubble with Random ... "
if diff -u <(./bin/bubble 10 128 1 1) <(bubble-random-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing bubble with LRU ... "
if diff -u <(./bin/bubble 10 128 2 1) <(bubble-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing bubble with Clock ... "
if diff -u <(./bin/bubble 10 128 3 1) <(bubble-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi
