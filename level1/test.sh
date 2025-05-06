#!/bin/bash

STRINGS="$(strings level1)"
for STRING in $STRINGS; do
    cp level1 b.out
    RESULT="$(./b.out $STRING)"
    if [[ $RESULT == *"Access granted \o/"* ]]
    then
	printf "%s" $STRING
	break;
    fi
done