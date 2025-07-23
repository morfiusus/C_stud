#!/bin/bash

# Создаем временные файлы
test_file="test_cat.txt"
out_cat="out_cat.txt"
out_s21cat="out_s21cat.txt"

# Создаем тестовый файл
cat > $test_file <<EOF
	test line 1
TEST line 2
another test line
no match here
EOF

# Функция для сравнения вывода
compare() {
    local flags=$1
    echo "Testing flags: $flags"
    
    # Запускаем оригинальный cat
    cat $flags $test_file > $out_cat
    
    # Запускаем нашу реализацию
    ./s21_cat $flags $test_file > $out_s21cat
    
    # Сравниваем вывод
    if diff -q $out_cat $out_s21cat >/dev/null; then
        echo "  OK"
    else
        echo "  FAIL"
        echo "cat output:"
        cat $out_cat
        echo "s21_cat output:"
        cat $out_s21cat
    fi
    echo "----------------------------------"
}

# Тестируем основные флаги по одному
compare_f() {
    local flags=$1
    echo "Testing flags: $flags"
    
    # Запускаем оригинальный cat
    cat $flags $test_file > $out_cat
    
    # Запускаем нашу реализацию
    ./s21_cat $flags $test_file > $out_s21cat
    
    # Сравниваем вывод
    if diff -q $out_cat $out_s21cat >/dev/null; then
        echo "  OK"
    else
        echo "  FAIL"
        echo "cat output:"
        cat $out_cat
        echo "s21_cat output:"
        cat $out_s21cat
    fi
    echo "----------------------------------"
}

compare "-b"
compare "-e"
compare "-E"
compare "-s"
compare "-t"
compare "-T"
compare "-n"
compare "--number-nonblank"
compare "--number"
compare "--squeeze-blank"
# Тестируем комбинации флагов
compare "-beEstTn"

# Удаляем временные файлы
rm -f $test_file $out_cat $out_s21cat
echo "Testing completed"