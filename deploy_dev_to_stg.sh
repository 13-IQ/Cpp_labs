#!/bin/bash
set -e

echo "=== Начало деплоя dev -> stg ==="

# Переключаемся на ветку stg
git checkout stg

# Вливаем изменения из dev в stg
git merge dev -m "Auto-merge: dev -> stg"

# Создаём тег с меткой времени (например: stg-20260626-183045)
TAG="stg-$(date +%Y%m%d-%H%M%S)"
git tag -a "$TAG" -m "Deployment to stg at $(date)"

# Отправляем ветку и тег на GitHub
git push origin stg
git push origin "$TAG"

# Возвращаемся в dev
git checkout dev

echo "=== Деплой завершён. Создан тег: $TAG ==="
