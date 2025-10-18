#!/bin/bash

#TODO: automation for branches.

if [ $# -ge 1 ]; then
    mess=$1
else 
    mess="Quick commit on $(date '+%Y-%m-%d %H:%M:%S')"
fi

git add .
git commit -m "$mess"
git push origin main

# verify
git status
