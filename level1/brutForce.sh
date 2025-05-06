#!/bin/bash

# Extract strings from the binary file `level1`
STRINGS="$(strings level1)"

# Loop through each extracted string
for STRING in $STRINGS; do
    # Try the string as input to the level1 binary
    RESULT="$(echo "$STRING" | ./level1)"

    # Check if the output contains the success message
    if [[ "$RESULT" != *"Nope."* ]]; then
        echo "Correct key found: $STRING"
        break
    fi
done
