#!/usr/bin/env bash

cd "$(dirname "$0")" || exit

CC="g++"
CFLAGS="-Wall -Wextra -pedantic -std=c++17 -O2"

set -x
mkdir -p build
$CC $CFLAGS quick_sort.cpp -o build/fixed_base
$CC $CFLAGS quick_sort.cpp -DRAND -o build/rand_base
$CC $CFLAGS quick_sort.cpp -DSELECT3 -o build/select3

$CC $CFLAGS quick_sort.cpp -DUSE_STD_SORT -o build/std_sort
$CC $CFLAGS quick_sort.cpp -DUSE_STD_QSORT -o build/std_qsort
set +x

for n in $(seq 0 3); do
    printf "Test %d\n" $n

    [ ! -r "data/$n.in" ] && exit

    cp "data/$n.in" data.txt

    for f in build/*; do
        printf "%s:\t" $(basename $f) >&2
        "./$f"

        diff "data/$n.ans" sorted.txt > /dev/null 2>&1
        [ $? -ne 0 ] && printf "Test Failed!\n"
    done
    printf "\n"
done

rm -rf build data.txt sorted.txt
