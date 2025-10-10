#!/bin/bash


status=$(git status)

echo status


git add .
git commit -m "quick commit"
git push origin main
git status #sanity check
