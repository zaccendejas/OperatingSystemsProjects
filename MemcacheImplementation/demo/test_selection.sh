#!/bin/bash

selection-fifo-output() {
    cat <<EOF
Addresses : 256
Pages     : 4
VPNShift  : 6
VPNMask   : 0xC0
OffsetMask: 0x3F
Hits      : 581307
Misses    : 468291
EOF
}

selection-random-output() {
    cat <<EOF
Addresses : 256
Pages     : 4
VPNShift  : 6
VPNMask   : 0xC0
OffsetMask: 0x3F
Hits      : 626063
Misses    : 423535
EOF
}

selection-lru-output() {
    cat <<EOF
Addresses : 256
Pages     : 4
VPNShift  : 6
VPNMask   : 0xC0
OffsetMask: 0x3F
Hits      : 582444
Misses    : 467154
EOF
}

selection-clock-output() {
    cat <<EOF
Addresses : 256
Pages     : 4
VPNShift  : 6
VPNMask   : 0xC0
OffsetMask: 0x3F
Hits      : 582730
Misses    : 466868
EOF
}

SCRATCH=$(mktemp -d)
trap "rm -fr $SCRATCH" INT QUIT TERM EXIT

echo -n "Testing selection with FIFO ... "
if diff -u <(./bin/selection 8 64 0 1) <(selection-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing selection with Random ... "
if diff -u <(./bin/selection 8 64 1 1) <(selection-random-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing selection with LRU ... "
if diff -u <(./bin/selection 8 64 2 1) <(selection-lru-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing selection with Clock ... "
if diff -u <(./bin/selection 8 64 3 1) <(selection-clock-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi
