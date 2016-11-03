#!/bin/bash

# ./grading-script.sh <test-dir> <grading-scheme.txt>

test_dir="$1"
scheme="$2"

# Set up scratch space for grading
dir="grading"
mkdir -p $dir
cp *.h *.cpp Makefile $dir

# Check for cheating
token=`mktemp XXXXXXXXXXXXXXXXXXXXXXXX`
rm $token
sed -i "s/error:/$token/" $dir/main.cpp

# Compile project
{
    pushd $dir
    if ! make -j ; then
        echo "FAIL: Did not compile"
        exit 1
    fi
    popd >/dev/null
}

# Check results against grading criteria
./grading-helper.pl $dir "$test_dir" "$scheme" $token
