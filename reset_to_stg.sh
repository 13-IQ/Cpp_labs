#!/bin/bash
set -e

echo "=== Возврат к текущей ревизии в ветке stg ==="

git checkout stg
git reset --hard HEAD
git clean -fd
git checkout dev

echo "=== Возврат завершён. ==="
