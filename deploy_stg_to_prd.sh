#!/bin/bash
set -e

echo "=== Начало деплоя stg -> prd ==="

# Переключаемся на ветку prd
git checkout prd

# Вливаем изменения из stg в prd
git merge stg -m "Auto-merge: stg -> prd"

# Создаём тег с меткой времени (например: prd-20260626-183050)
TAG="prd-$(date +%Y%m%d-%H%M%S)"
git tag -a "$TAG" -m "Deployment to prd at $(date)"

# Отправляем ветку и тег на GitHub
git push origin prd
git push origin "$TAG"

# Возвращаемся в dev
git checkout dev

echo "=== Деплой завершён. Создан тег: $TAG ==="
