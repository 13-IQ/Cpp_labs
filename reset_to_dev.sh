#!/bin/bash
set -e

echo "=== Возврат к текущей ревизии в ветке dev ==="

# Переключаемся на dev
git checkout dev

# Отменяем все изменения вtracked файлах
git reset --hard HEAD

# Удаляем все untracked файлы и папки (кроме игнорируемых)
git clean -fd

echo "=== Возврат завершён. Рабочая директория очищена. ==="
