#!/bin/bash

prime-fifo-output() {
    cat <<EOF
Addresses : 1024
Pages     : 16
VPNShift  : 6
VPNMask   : 0x3C0
OffsetMask: 0x3F
Hits      : 2048
Misses    : 2048
EOF
}

prime-random-output() {
    cat <<EOF
Addresses : 1024
Pages     : 16
VPNShift  : 6
VPNMask   : 0x3C0
OffsetMask: 0x3F
Hits      : 2048
Misses    : 2048
EOF
}

SCRATCH=$(mktemp -d)
trap "rm -fr $SCRATCH" INT QUIT TERM EXIT

echo -n "Testing prime with FIFO ... "
if diff -u <(./bin/prime 10 64 0 1) <(prime-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing prime with Random ... "
if diff -u <(./bin/prime 10 64 1 1) <(prime-random-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing prime with LRU ... "
if diff -u <(./bin/prime 10 64 2 1) <(prime-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing prime with Clock ... "
if diff -u <(./bin/prime 10 64 3 1) <(prime-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi
