#!/bin/bash

#status=$(pwd)
#echo "$status"
echo "The first argument is: $1"
git add .
git commit -m "quick commit"
git push origin main
git status #sanity check
