#!/bin/bash

PROJECT="$1"
FOLDER="$2"
BLANK='\033[0m'
RED='\033[0;31m'
GREEN='\033[0;32m'

for f in "${FOLDER}"/*.in
do
    NAME=${f%.in}
    NAME=${NAME#"${FOLDER}"/}
    OUT=$(mktemp)
    ERR=$(mktemp)
    
    valgrind --error-exitcode=15 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all -q\
    "${PROJECT}" <$f >$OUT 2>$ERR
    
    if diff $OUT ${f%.in}.out > /dev/null || diff $ERR ${f%.in}.err > /dev/null
    then
        echo -e "$GREEN$NAME - OK!$BLANK"
    else
        echo -e "$RED$NAME - WRONG!$BLANK"
    fi
    rm $OUT $ERR
done
