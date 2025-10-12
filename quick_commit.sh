#!/bin/bash

if [ $# -ge 1 ]; then
    mess=$1
else 
    mess="quick commit"
fi

git add .
git commit -m "$mess"
git push origin main
git status #sanity check!
