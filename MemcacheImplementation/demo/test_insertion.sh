#!/bin/bash

insertion-fifo-output() {
    cat <<EOF
Addresses : 256
Pages     : 4
VPNShift  : 6
VPNMask   : 0xC0
OffsetMask: 0x3F
Hits      : 866232
Misses    : 176142
EOF
}

insertion-random-output() {
    cat <<EOF
Addresses : 256
Pages     : 4
VPNShift  : 6
VPNMask   : 0xC0
OffsetMask: 0x3F
Hits      : 875162
Misses    : 167212
EOF
}

insertion-lru-output() {
    cat <<EOF
Addresses : 256
Pages     : 4
VPNShift  : 6
VPNMask   : 0xC0
OffsetMask: 0x3F
Hits      : 864506
Misses    : 177868
EOF
}

insertion-clock-output() {
    cat <<EOF
Addresses : 256
Pages     : 4
VPNShift  : 6
VPNMask   : 0xC0
OffsetMask: 0x3F
Hits      : 866461
Misses    : 175913
EOF
}

SCRATCH=$(mktemp -d)
trap "rm -fr $SCRATCH" INT QUIT TERM EXIT

echo -n "Testing insertion with FIFO ... "
if diff -u <(./bin/insertion 8 64 0 1) <(insertion-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing insertion with Random ... "
if diff -u <(./bin/insertion 8 64 1 1) <(insertion-random-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing insertion with LRU ... "
if diff -u <(./bin/insertion 8 64 2 1) <(insertion-lru-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing insertion with Clock ... "
if diff -u <(./bin/insertion 8 64 3 1) <(insertion-clock-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi
