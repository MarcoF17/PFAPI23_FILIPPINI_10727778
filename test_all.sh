#!/bin/bash

# Estrai la cartella se non esiste
if [ ! -d "open_tests" ]; then
    echo "Estrazione open_tests.zip..."
    # mkdir -p open_tests
    unzip -q open_tests.zip -d open_tests
fi

# Compila il programma
gcc -Wall -Werror -std=gnu11 -O2 -lm main.c -o main

# Esegui i test da 1 a 111 senza zeri iniziali
for i in $(seq 1 111); do
    echo "test $i"
    ./main < "open_tests/open_${i}.txt" > out.txt
    diff out.txt "open_tests/open_${i}.output.txt" || echo "❌ Test $i fallito"
done

rm out.txt
echo "DONE"
