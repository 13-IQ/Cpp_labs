#!/bin/bash
set -e

echo "=== Возврат к текущей ревизии в ветке prd ==="

git checkout prd
git reset --hard HEAD
git clean -fd
git checkout dev

echo "=== Возврат завершён. ==="
