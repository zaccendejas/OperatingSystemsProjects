#!/bin/bash

bs-fifo-output() {
    cat <<EOF
Addresses : 1024
Pages     : 16
VPNShift  : 6
VPNMask   : 0x3C0
OffsetMask: 0x3F
Hits      : 8407
Misses    : 1024
EOF
}

bs-random-output() {
    cat <<EOF
Addresses : 1024
Pages     : 16
VPNShift  : 6
VPNMask   : 0x3C0
OffsetMask: 0x3F
Hits      : 8407
Misses    : 1024
EOF
}

SCRATCH=$(mktemp -d)
trap "rm -fr $SCRATCH" INT QUIT TERM EXIT

echo -n "Testing bs with FIFO ... "
if diff -u <(./bin/bs 10 64 0 1) <(bs-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing bs with Random ... "
if diff -u <(./bin/bs 10 64 1 1) <(bs-random-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing bs with LRU ... "
if diff -u <(./bin/bs 10 64 2 1) <(bs-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing bs with Clock ... "
if diff -u <(./bin/bs 10 64 3 1) <(bs-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi
