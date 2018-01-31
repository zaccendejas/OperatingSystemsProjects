#!/bin/bash

test-library() {
    library=$1
    echo -n "Testing $library ... "
    if diff -q <(env LD_PRELOAD=./lib/$library cat README.md) <(cat README.md); then
        echo "success"
    else
        echo "failure"
    diff -u <(env LD_PRELOAD=./lib/$library cat README.md) <(cat README.md)
    fi
}

test-library libmalloc-ff.so
test-library libmalloc-nf.so
test-library libmalloc-bf.so
test-library libmalloc-wf.so
