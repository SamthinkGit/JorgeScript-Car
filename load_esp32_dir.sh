#!/bin/sh

for file in *\.*; do
    if [ -f "$file" ] && grep -q "^//Arduino" "$file"; then
        mv "$file" ./arduino/
    fi
done

if [ -d ./esp32 ]; then
    mv ./esp32/* .
fi

echo "./esp32 -> root"
