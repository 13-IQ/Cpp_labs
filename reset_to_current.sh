#!/bin/bash
set -e

echo "=== Возврат к текущей ревизии ==="
echo "ВНИМАНИЕ: Все несохранённые изменения будут потеряны!"

CURRENT_BRANCH=$(git branch --show-current)
echo "Текущая ветка: $CURRENT_BRANCH"

git reset --hard HEAD
git clean -fd

echo "=== Возврат завершён ==="
