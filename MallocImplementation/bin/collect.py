#!/usr/bin/env python2.7

from subprocess import Popen, PIPE

libs = ["libmalloc-ff.so","libmalloc-nf.so", "libmalloc-bf.so", "libmalloc-wf.so"]

cmds = []
for lib in libs:
    cmds.append("env LD_PRELOAD=./lib/{} ./bin/Benchmark".format(lib))

mallocs = []
frees = []
reuses = []
grows = []
splits = []
coalesces = []
blocks = []
requested = []
maxHeap = []
time = []

for cmd in cmds:
    proc = Popen(cmd.split(), stdout=PIPE)
    for i, line in enumerate(proc.stdout):
        if i == 0: 
            mallocs.append(line.rstrip().split()[1])
        if i == 1: 
            frees.append(line.rstrip().split()[1])
        if i == 2: 
            reuses.append(line.rstrip().split()[1])
        if i == 3: 
            grows.append(line.rstrip().split()[1])
        if i == 4: 
            splits.append(line.rstrip().split()[1])
        if i == 5: 
            coalesces.append(line.rstrip().split()[1])
        if i == 6: 
            blocks.append(line.rstrip().split()[1])
        if i == 7: 
            requested.append(line.rstrip().split()[1])
        if i == 8: 
            maxHeap.append(line.rstrip().split()[2])

print "libraries: ff,     nf,      bf,      wf"
print ""
print "mallocs: ", mallocs
print "frees: ", frees
print "reuses: ", reuses
print "grows: ", grows
print "splits: ", splits
print "coalesces: ", coalesces
print "blocks: ", blocks
print "requested: ", requested
print "Max Heap: ", maxHeap
