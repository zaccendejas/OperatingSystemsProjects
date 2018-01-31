#!/bin/bash

fibonacci-fifo-output() {
    cat <<EOF
 0. Fibonacci( 0) = 0
 0. Fibonacci( 1) = 1
 0. Fibonacci( 2) = 1
 0. Fibonacci( 3) = 2
 0. Fibonacci( 4) = 3
 0. Fibonacci( 5) = 5
 0. Fibonacci( 6) = 8
 0. Fibonacci( 7) = 13
 0. Fibonacci( 8) = 21
 0. Fibonacci( 9) = 34
 0. Fibonacci(10) = 55
 0. Fibonacci(11) = 89
 0. Fibonacci(12) = 144
 0. Fibonacci(13) = 233
 0. Fibonacci(14) = 377
 0. Fibonacci(15) = 610
 0. Fibonacci(16) = 987
 0. Fibonacci(17) = 1597
 0. Fibonacci(18) = 2584
 0. Fibonacci(19) = 4181
 0. Fibonacci(20) = 6765
 0. Fibonacci(21) = 10946
 0. Fibonacci(22) = 17711
 0. Fibonacci(23) = 28657
 0. Fibonacci(24) = 46368
 0. Fibonacci(25) = 75025
 0. Fibonacci(26) = 121393
 0. Fibonacci(27) = 196418
 0. Fibonacci(28) = 317811
 0. Fibonacci(29) = 514229
 0. Fibonacci(30) = 832040
 0. Fibonacci(31) = 1346269
 0. Fibonacci(32) = 2178309
 0. Fibonacci(33) = 3524578
 0. Fibonacci(34) = 5702887
 0. Fibonacci(35) = 9227465
 0. Fibonacci(36) = 14930352
 0. Fibonacci(37) = 24157817
 0. Fibonacci(38) = 39088169
 0. Fibonacci(39) = 63245986
 0. Fibonacci(40) = 102334155
 0. Fibonacci(41) = 165580141
 0. Fibonacci(42) = 267914296
 0. Fibonacci(43) = 433494437
 0. Fibonacci(44) = 701408733
 0. Fibonacci(45) = 1134903170
 0. Fibonacci(46) = 1836311903
 0. Fibonacci(47) = 2971215073
 0. Fibonacci(48) = 4807526976
 0. Fibonacci(49) = 7778742049
 0. Fibonacci(50) = 12586269025
 0. Fibonacci(51) = 20365011074
 0. Fibonacci(52) = 32951280099
 0. Fibonacci(53) = 53316291173
 0. Fibonacci(54) = 86267571272
 0. Fibonacci(55) = 139583862445
 0. Fibonacci(56) = 225851433717
 0. Fibonacci(57) = 365435296162
 0. Fibonacci(58) = 591286729879
 0. Fibonacci(59) = 956722026041
 0. Fibonacci(60) = 1548008755920
 0. Fibonacci(61) = 2504730781961
 0. Fibonacci(62) = 4052739537881
 0. Fibonacci(63) = 6557470319842
 0. Fibonacci(64) = 10610209857723
 0. Fibonacci(65) = 17167680177565
 0. Fibonacci(66) = 27777890035288
 0. Fibonacci(67) = 44945570212853
 0. Fibonacci(68) = 72723460248141
 0. Fibonacci(69) = 117669030460994
 0. Fibonacci(70) = 190392490709135
 0. Fibonacci(71) = 308061521170129
 0. Fibonacci(72) = 498454011879264
 0. Fibonacci(73) = 806515533049393
 0. Fibonacci(74) = 1304969544928657
 0. Fibonacci(75) = 2111485077978050
 0. Fibonacci(76) = 3416454622906707
 0. Fibonacci(77) = 5527939700884757
 0. Fibonacci(78) = 8944394323791464
 0. Fibonacci(79) = 14472334024676221
 0. Fibonacci(80) = 23416728348467685
 0. Fibonacci(81) = 37889062373143906
 0. Fibonacci(82) = 61305790721611591
 0. Fibonacci(83) = 99194853094755497
 0. Fibonacci(84) = 160500643816367088
 0. Fibonacci(85) = 259695496911122585
 0. Fibonacci(86) = 420196140727489673
 0. Fibonacci(87) = 679891637638612258
 0. Fibonacci(88) = 1100087778366101931
 0. Fibonacci(89) = 1779979416004714189
 0. Fibonacci(90) = 2880067194370816120
 0. Fibonacci(91) = 4660046610375530309
 0. Fibonacci(92) = 7540113804746346429
 0. Fibonacci(93) = 12200160415121876738
 0. Fibonacci(94) = 1293530146158671551
 0. Fibonacci(95) = 13493690561280548289
 0. Fibonacci(96) = 14787220707439219840
 0. Fibonacci(97) = 9834167195010216513
 0. Fibonacci(98) = 6174643828739884737
 0. Fibonacci(99) = 16008811023750101250
Addresses : 64
Pages     : 4
VPNShift  : 4
VPNMask   : 0x30
OffsetMask: 0xF
Hits      : 194
Misses    : 100
EOF
}

fibonacci-random-output() {
    cat <<EOF
 0. Fibonacci( 0) = 0
 0. Fibonacci( 1) = 1
 0. Fibonacci( 2) = 1
 0. Fibonacci( 3) = 2
 0. Fibonacci( 4) = 3
 0. Fibonacci( 5) = 5
 0. Fibonacci( 6) = 8
 0. Fibonacci( 7) = 13
 0. Fibonacci( 8) = 21
 0. Fibonacci( 9) = 34
 0. Fibonacci(10) = 55
 0. Fibonacci(11) = 89
 0. Fibonacci(12) = 144
 0. Fibonacci(13) = 233
 0. Fibonacci(14) = 377
 0. Fibonacci(15) = 610
 0. Fibonacci(16) = 987
 0. Fibonacci(17) = 1597
 0. Fibonacci(18) = 2584
 0. Fibonacci(19) = 4181
 0. Fibonacci(20) = 6765
 0. Fibonacci(21) = 10946
 0. Fibonacci(22) = 17711
 0. Fibonacci(23) = 28657
 0. Fibonacci(24) = 46368
 0. Fibonacci(25) = 75025
 0. Fibonacci(26) = 121393
 0. Fibonacci(27) = 196418
 0. Fibonacci(28) = 317811
 0. Fibonacci(29) = 514229
 0. Fibonacci(30) = 832040
 0. Fibonacci(31) = 1346269
 0. Fibonacci(32) = 2178309
 0. Fibonacci(33) = 3524578
 0. Fibonacci(34) = 5702887
 0. Fibonacci(35) = 9227465
 0. Fibonacci(36) = 14930352
 0. Fibonacci(37) = 24157817
 0. Fibonacci(38) = 39088169
 0. Fibonacci(39) = 63245986
 0. Fibonacci(40) = 102334155
 0. Fibonacci(41) = 165580141
 0. Fibonacci(42) = 267914296
 0. Fibonacci(43) = 433494437
 0. Fibonacci(44) = 701408733
 0. Fibonacci(45) = 1134903170
 0. Fibonacci(46) = 1836311903
 0. Fibonacci(47) = 2971215073
 0. Fibonacci(48) = 4807526976
 0. Fibonacci(49) = 7778742049
 0. Fibonacci(50) = 12586269025
 0. Fibonacci(51) = 20365011074
 0. Fibonacci(52) = 32951280099
 0. Fibonacci(53) = 53316291173
 0. Fibonacci(54) = 86267571272
 0. Fibonacci(55) = 139583862445
 0. Fibonacci(56) = 225851433717
 0. Fibonacci(57) = 365435296162
 0. Fibonacci(58) = 591286729879
 0. Fibonacci(59) = 956722026041
 0. Fibonacci(60) = 1548008755920
 0. Fibonacci(61) = 2504730781961
 0. Fibonacci(62) = 4052739537881
 0. Fibonacci(63) = 6557470319842
 0. Fibonacci(64) = 10610209857723
 0. Fibonacci(65) = 17167680177565
 0. Fibonacci(66) = 27777890035288
 0. Fibonacci(67) = 44945570212853
 0. Fibonacci(68) = 72723460248141
 0. Fibonacci(69) = 117669030460994
 0. Fibonacci(70) = 190392490709135
 0. Fibonacci(71) = 308061521170129
 0. Fibonacci(72) = 498454011879264
 0. Fibonacci(73) = 806515533049393
 0. Fibonacci(74) = 1304969544928657
 0. Fibonacci(75) = 2111485077978050
 0. Fibonacci(76) = 3416454622906707
 0. Fibonacci(77) = 5527939700884757
 0. Fibonacci(78) = 8944394323791464
 0. Fibonacci(79) = 14472334024676221
 0. Fibonacci(80) = 23416728348467685
 0. Fibonacci(81) = 37889062373143906
 0. Fibonacci(82) = 61305790721611591
 0. Fibonacci(83) = 99194853094755497
 0. Fibonacci(84) = 160500643816367088
 0. Fibonacci(85) = 259695496911122585
 0. Fibonacci(86) = 420196140727489673
 0. Fibonacci(87) = 679891637638612258
 0. Fibonacci(88) = 1100087778366101931
 0. Fibonacci(89) = 1779979416004714189
 0. Fibonacci(90) = 2880067194370816120
 0. Fibonacci(91) = 4660046610375530309
 0. Fibonacci(92) = 7540113804746346429
 0. Fibonacci(93) = 12200160415121876738
 0. Fibonacci(94) = 1293530146158671551
 0. Fibonacci(95) = 13493690561280548289
 0. Fibonacci(96) = 14787220707439219840
 0. Fibonacci(97) = 9834167195010216513
 0. Fibonacci(98) = 6174643828739884737
 0. Fibonacci(99) = 16008811023750101250
Addresses : 64
Pages     : 4
VPNShift  : 4
VPNMask   : 0x30
OffsetMask: 0xF
Hits      : 198
Misses    : 104
EOF
}

fibonacci-lru-output() {
    cat <<EOF
 0. Fibonacci( 0) = 0
 0. Fibonacci( 1) = 1
 0. Fibonacci( 2) = 1
 0. Fibonacci( 3) = 2
 0. Fibonacci( 4) = 3
 0. Fibonacci( 5) = 5
 0. Fibonacci( 6) = 8
 0. Fibonacci( 7) = 13
 0. Fibonacci( 8) = 21
 0. Fibonacci( 9) = 34
 0. Fibonacci(10) = 55
 0. Fibonacci(11) = 89
 0. Fibonacci(12) = 144
 0. Fibonacci(13) = 233
 0. Fibonacci(14) = 377
 0. Fibonacci(15) = 610
 0. Fibonacci(16) = 987
 0. Fibonacci(17) = 1597
 0. Fibonacci(18) = 2584
 0. Fibonacci(19) = 4181
 0. Fibonacci(20) = 6765
 0. Fibonacci(21) = 10946
 0. Fibonacci(22) = 17711
 0. Fibonacci(23) = 28657
 0. Fibonacci(24) = 46368
 0. Fibonacci(25) = 75025
 0. Fibonacci(26) = 121393
 0. Fibonacci(27) = 196418
 0. Fibonacci(28) = 317811
 0. Fibonacci(29) = 514229
 0. Fibonacci(30) = 832040
 0. Fibonacci(31) = 1346269
 0. Fibonacci(32) = 2178309
 0. Fibonacci(33) = 3524578
 0. Fibonacci(34) = 5702887
 0. Fibonacci(35) = 9227465
 0. Fibonacci(36) = 14930352
 0. Fibonacci(37) = 24157817
 0. Fibonacci(38) = 39088169
 0. Fibonacci(39) = 63245986
 0. Fibonacci(40) = 102334155
 0. Fibonacci(41) = 165580141
 0. Fibonacci(42) = 267914296
 0. Fibonacci(43) = 433494437
 0. Fibonacci(44) = 701408733
 0. Fibonacci(45) = 1134903170
 0. Fibonacci(46) = 1836311903
 0. Fibonacci(47) = 2971215073
 0. Fibonacci(48) = 4807526976
 0. Fibonacci(49) = 7778742049
 0. Fibonacci(50) = 12586269025
 0. Fibonacci(51) = 20365011074
 0. Fibonacci(52) = 32951280099
 0. Fibonacci(53) = 53316291173
 0. Fibonacci(54) = 86267571272
 0. Fibonacci(55) = 139583862445
 0. Fibonacci(56) = 225851433717
 0. Fibonacci(57) = 365435296162
 0. Fibonacci(58) = 591286729879
 0. Fibonacci(59) = 956722026041
 0. Fibonacci(60) = 1548008755920
 0. Fibonacci(61) = 2504730781961
 0. Fibonacci(62) = 4052739537881
 0. Fibonacci(63) = 6557470319842
 0. Fibonacci(64) = 10610209857723
 0. Fibonacci(65) = 17167680177565
 0. Fibonacci(66) = 27777890035288
 0. Fibonacci(67) = 44945570212853
 0. Fibonacci(68) = 72723460248141
 0. Fibonacci(69) = 117669030460994
 0. Fibonacci(70) = 190392490709135
 0. Fibonacci(71) = 308061521170129
 0. Fibonacci(72) = 498454011879264
 0. Fibonacci(73) = 806515533049393
 0. Fibonacci(74) = 1304969544928657
 0. Fibonacci(75) = 2111485077978050
 0. Fibonacci(76) = 3416454622906707
 0. Fibonacci(77) = 5527939700884757
 0. Fibonacci(78) = 8944394323791464
 0. Fibonacci(79) = 14472334024676221
 0. Fibonacci(80) = 23416728348467685
 0. Fibonacci(81) = 37889062373143906
 0. Fibonacci(82) = 61305790721611591
 0. Fibonacci(83) = 99194853094755497
 0. Fibonacci(84) = 160500643816367088
 0. Fibonacci(85) = 259695496911122585
 0. Fibonacci(86) = 420196140727489673
 0. Fibonacci(87) = 679891637638612258
 0. Fibonacci(88) = 1100087778366101931
 0. Fibonacci(89) = 1779979416004714189
 0. Fibonacci(90) = 2880067194370816120
 0. Fibonacci(91) = 4660046610375530309
 0. Fibonacci(92) = 7540113804746346429
 0. Fibonacci(93) = 12200160415121876738
 0. Fibonacci(94) = 1293530146158671551
 0. Fibonacci(95) = 13493690561280548289
 0. Fibonacci(96) = 14787220707439219840
 0. Fibonacci(97) = 9834167195010216513
 0. Fibonacci(98) = 6174643828739884737
 0. Fibonacci(99) = 16008811023750101250
Addresses : 64
Pages     : 4
VPNShift  : 4
VPNMask   : 0x30
OffsetMask: 0xF
Hits      : 194
Misses    : 100
EOF
}

fibonacci-clock-output() {
    cat <<EOF
 0. Fibonacci( 0) = 0
 0. Fibonacci( 1) = 1
 0. Fibonacci( 2) = 1
 0. Fibonacci( 3) = 2
 0. Fibonacci( 4) = 3
 0. Fibonacci( 5) = 5
 0. Fibonacci( 6) = 8
 0. Fibonacci( 7) = 13
 0. Fibonacci( 8) = 21
 0. Fibonacci( 9) = 34
 0. Fibonacci(10) = 55
 0. Fibonacci(11) = 89
 0. Fibonacci(12) = 144
 0. Fibonacci(13) = 233
 0. Fibonacci(14) = 377
 0. Fibonacci(15) = 610
 0. Fibonacci(16) = 987
 0. Fibonacci(17) = 1597
 0. Fibonacci(18) = 2584
 0. Fibonacci(19) = 4181
 0. Fibonacci(20) = 6765
 0. Fibonacci(21) = 10946
 0. Fibonacci(22) = 17711
 0. Fibonacci(23) = 28657
 0. Fibonacci(24) = 46368
 0. Fibonacci(25) = 75025
 0. Fibonacci(26) = 121393
 0. Fibonacci(27) = 196418
 0. Fibonacci(28) = 317811
 0. Fibonacci(29) = 514229
 0. Fibonacci(30) = 832040
 0. Fibonacci(31) = 1346269
 0. Fibonacci(32) = 2178309
 0. Fibonacci(33) = 3524578
 0. Fibonacci(34) = 5702887
 0. Fibonacci(35) = 9227465
 0. Fibonacci(36) = 14930352
 0. Fibonacci(37) = 24157817
 0. Fibonacci(38) = 39088169
 0. Fibonacci(39) = 63245986
 0. Fibonacci(40) = 102334155
 0. Fibonacci(41) = 165580141
 0. Fibonacci(42) = 267914296
 0. Fibonacci(43) = 433494437
 0. Fibonacci(44) = 701408733
 0. Fibonacci(45) = 1134903170
 0. Fibonacci(46) = 1836311903
 0. Fibonacci(47) = 2971215073
 0. Fibonacci(48) = 4807526976
 0. Fibonacci(49) = 7778742049
 0. Fibonacci(50) = 12586269025
 0. Fibonacci(51) = 20365011074
 0. Fibonacci(52) = 32951280099
 0. Fibonacci(53) = 53316291173
 0. Fibonacci(54) = 86267571272
 0. Fibonacci(55) = 139583862445
 0. Fibonacci(56) = 225851433717
 0. Fibonacci(57) = 365435296162
 0. Fibonacci(58) = 591286729879
 0. Fibonacci(59) = 956722026041
 0. Fibonacci(60) = 1548008755920
 0. Fibonacci(61) = 2504730781961
 0. Fibonacci(62) = 4052739537881
 0. Fibonacci(63) = 6557470319842
 0. Fibonacci(64) = 10610209857723
 0. Fibonacci(65) = 17167680177565
 0. Fibonacci(66) = 27777890035288
 0. Fibonacci(67) = 44945570212853
 0. Fibonacci(68) = 72723460248141
 0. Fibonacci(69) = 117669030460994
 0. Fibonacci(70) = 190392490709135
 0. Fibonacci(71) = 308061521170129
 0. Fibonacci(72) = 498454011879264
 0. Fibonacci(73) = 806515533049393
 0. Fibonacci(74) = 1304969544928657
 0. Fibonacci(75) = 2111485077978050
 0. Fibonacci(76) = 3416454622906707
 0. Fibonacci(77) = 5527939700884757
 0. Fibonacci(78) = 8944394323791464
 0. Fibonacci(79) = 14472334024676221
 0. Fibonacci(80) = 23416728348467685
 0. Fibonacci(81) = 37889062373143906
 0. Fibonacci(82) = 61305790721611591
 0. Fibonacci(83) = 99194853094755497
 0. Fibonacci(84) = 160500643816367088
 0. Fibonacci(85) = 259695496911122585
 0. Fibonacci(86) = 420196140727489673
 0. Fibonacci(87) = 679891637638612258
 0. Fibonacci(88) = 1100087778366101931
 0. Fibonacci(89) = 1779979416004714189
 0. Fibonacci(90) = 2880067194370816120
 0. Fibonacci(91) = 4660046610375530309
 0. Fibonacci(92) = 7540113804746346429
 0. Fibonacci(93) = 12200160415121876738
 0. Fibonacci(94) = 1293530146158671551
 0. Fibonacci(95) = 13493690561280548289
 0. Fibonacci(96) = 14787220707439219840
 0. Fibonacci(97) = 9834167195010216513
 0. Fibonacci(98) = 6174643828739884737
 0. Fibonacci(99) = 16008811023750101250
Addresses : 64
Pages     : 4
VPNShift  : 4
VPNMask   : 0x30
OffsetMask: 0xF
Hits      : 194
Misses    : 100
EOF
}

SCRATCH=$(mktemp -d)
trap "rm -fr $SCRATCH" INT QUIT TERM EXIT

echo -n "Testing fibonacci with FIFO ... "
if diff -u <(./bin/fibonacci 6 16 0 1) <(fibonacci-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing fibonacci with Random ... "
if diff -u <(./bin/fibonacci 6 16 1 1) <(fibonacci-random-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing fibonacci with LRU ... "
if diff -u <(./bin/fibonacci 6 16 2 1) <(fibonacci-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi

echo -n "Testing fibonacci with Clock ... "
if diff -u <(./bin/fibonacci 6 16 3 1) <(fibonacci-fifo-output) > $SCRATCH/test.log; then
    echo Success
else
    echo Failure
    cat $SCRATCH/test.log
fi
