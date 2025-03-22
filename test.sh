#!/usr/bin/env bash

set -euo pipefail

#     The diff utility exits with one of the following values:
#
#           0       No differences were found.
#           1       Differences were found.
#           >1      An error occurred.

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
   cat /dev/urandom | head -n 10 > $tmp_file || true

   if diff <($binary "$tmp_file") <(xxd "$tmp_file") > /dev/null; then
       echo "TEST $i: PASS"
    else
       echo "TEST $i: FAIL"
       diff <($binary $tmp_file) <(xxd $tmp_file) >&2
       exit 1
   fi
done
