#!/bin/sh

EXAMPLE_TO_BE_RUN="hello_world"
# EXAMPLE_TO_BE_RUN="demo"

INPUT_FILE_PATH="./examples/${EXAMPLE_TO_BE_RUN}.cfs"
OUTPUT_FILE_PATH="${EXAMPLE_TO_BE_RUN}_decompiled.cfs"

printf "\n"

if ./bin/cfs $INPUT_FILE_PATH $OUTPUT_FILE_PATH;
then
    mv "./${EXAMPLE_TO_BE_RUN}_decompiled.cfs" ./bin/
    mv ./out.s ./bin/

    mkdir -p ./examples/bin/

    gcc ./bin/out.s -o "./examples/bin/${EXAMPLE_TO_BE_RUN}"

    printf "\n"

    ./examples/bin/"${EXAMPLE_TO_BE_RUN}"
else
    printf "Couldn't run Script\n"
fi
