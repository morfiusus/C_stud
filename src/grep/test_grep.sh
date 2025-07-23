#!/bin/bash

# Создаем временные файлы
test_file="test_grep.txt"
pattern="test"
pattern_f="pattern_f.txt"
out_grep="out_grep.txt"
out_s21grep="out_s21grep.txt"

# Создаем тестовый файл
cat > $test_file <<EOF
test line 1
TEST line 2
another test line
no match here
\x01\x02
EOF

cat > $pattern_f <<EOF
test
EOF

# Функция для сравнения вывода
compare() {
    local flags=$1
    echo "Testing flags: $flags"
    
    # Запускаем оригинальный grep
    grep $flags "$pattern" $test_file > $out_grep
    
    # Запускаем нашу реализацию
    ./s21_grep $flags "$pattern" $test_file > $out_s21grep
    
    # Сравниваем вывод
    if diff -q $out_grep $out_s21grep >/dev/null; then
        echo "  OK"
    else
        echo "  FAIL"
        echo "grep output:"
        cat $out_grep
        echo "s21_grep output:"
        cat $out_s21grep
    fi
    echo "----------------------------------"
}

# Тестируем основные флаги по одному
compare_f() {
    local flags=$1
    echo "Testing flags: $flags"
    
    # Запускаем оригинальный grep
    grep $flags "$pattern_f" $test_file > $out_grep
    
    # Запускаем нашу реализацию
    ./s21_grep $flags "$pattern_f" $test_file > $out_s21grep
    
    # Сравниваем вывод
    if diff -q $out_grep $out_s21grep >/dev/null; then
        echo "  OK"
    else
        echo "  FAIL"
        echo "grep output:"
        cat $out_grep
        echo "s21_grep output:"
        cat $out_s21grep
    fi
    echo "----------------------------------"
}

compare "-o"
compare "-h"
compare "-s"
compare_f "-f"
compare "-e"
compare "-i"
compare "-v"
compare "-c"
compare "-n"
compare "-l"

# Тестируем комбинации флагов
compare "-oh"
compare "-os"
compare_f "-of"
compare "-oe"
compare "-oi"
compare "-ov"
compare "-oc"
compare "-on"
compare "-ol"
compare "-hs"
compare_f "-hf"
compare "-he"
compare "-hi"
compare "-hv"
compare "-hc"
compare "-hn"
compare "-hl"
compare_f "-sf"
compare "-se"
compare "-si"
compare "-sv"
compare "-sc"
compare "-sn"
compare "-sl"
compare "-iv"
compare "-ic"
compare "-in"
compare "-il"
compare "-vc"
compare "-vn"
compare "-vl"
compare "-cn"
compare "-cl"
compare "-nl"


# Удаляем временные файлы
rm -f $test_file $out_grep $out_s21grep $pattern_f

echo "Testing completed"