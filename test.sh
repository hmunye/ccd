#!/usr/bin/env bash

set -euo pipefail

if [[ "$#" -ne 1  ]]; then
    echo "USAGE: $0 <num_tests>" >&2
    exit 1
fi

num_tests=$1
binary="./ccd"

if [ ! -f $binary ]; then
    echo "ERROR: $binary not found" >&2
    exit 1
fi

if ! [ -x "$(command -v xxd)" ]; then
    echo "ERROR: xxd not installed or not in PATH" >&2
    exit 1
fi

tmp_file=$(mktemp)

trap "rm -f $tmp_file" EXIT

for ((i = 1; i <= $num_tests; ++i)); do
   random_num=$(( RANDOM % (64 - 2 + 1) + 2 ))
   cat /dev/urandom | head -n "$random_num" > $tmp_file || true

   echo -n "TEST $i: "

   if diff <($binary "$tmp_file") <(xxd "$tmp_file") > /dev/null; then
       echo "PASS"
    else
       echo "FAIL"
       diff <($binary $tmp_file) <(xxd $tmp_file) >&2
       exit 1
   fi
done
