#!/bin/sh

for file in *\.*; do
    if [ -f "$file" ] && grep -q "^//ESP32" "$file"; then
        mv "$file" ./esp32/
    fi
done

if [ -d ./arduino ]; then
    mv ./arduino/* ./
fi

echo "./arduino -> root"

