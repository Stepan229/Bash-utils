#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""
RED="$(tput setaf 1)"
GREEN="$(tput setaf 2)"
BASE="$(tput setaf 7)"
declare -a tests=(
    "s test_0_grep.txt test_1_grep.txt VAR"
    "for ../s21_grep.c VAR"
    "s test_2_grep.txt test_3_grep.txt VAR"
    "int test_4_grep.txt VAR"
    "regex ../s21_grep.h  VAR"
    "s test_6_grep.txt test_5_grep.txt VAR"
    "print ../s21_grep.c VAR"
    
)

declare -a extra=(
    "-n for test_1_grep.txt test_2_grep.txt"
    "-n for test_1_grep.txt"
    "-n -e ^\} test_1_grep.txt"
    "-ce ^int test_1_grep.txt test_2_grep.txt"
    "-ne = -e out test_5_grep.txt"
    "-e ^int test_1_grep.txt"
    "-nivh = test_1_grep.txt test_2_grep.txt"
    "-ie INT test_5_grep.txt"
    "-echar test_1_grep.txt test_2_grep.txt"
    "-ne = -e out test_5_grep.txt"
    "-iv int test_5_grep.txt"
    "-in int test_5_grep.txt"
    "-c -l aboba test_1_grep.txt test_5_grep.txt"
    "-v test_1_grep.txt -e ank"
    "-in -l int test_5_grep.txt"
    "-l for test_1_grep.txt test_2_grep.txt"
    "-v test_1_grep.txt -e ank"
    "-e = -e out test_5_grep.txt"
    "-e = -e out test_5_grep.txt -s"
    "-e ing -e as -e the -e not -e is test_6_grep.txt"
    "-l for no_file.txt test_2_grep.txt"
    "-f test_ptrn_grep.txt test_1_grep.txt test_3_grep.txt test_4_grep.txt"
)


testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./../s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    RESULT="SUCCESS"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
        RESULT="SUCCESS"
    else
      (( FAIL++ ))
        RESULT="FAIL"
    fi
    echo "[${GREEN}${SUCCESS}${BASE}/${RED}${FAIL}${BASE}] ${GREEN}${RESULT}${BASE} grep $t"
    rm test_s21_grep.log test_sys_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in v c l n h
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in v c l n h
do
    for var2 in v c l n h
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 3 параметра
for var1 in v c l n h
do
    for var2 in v c l n h
    do
        for var3 in v c l n h
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

# 2 сдвоенных параметра
for var1 in v c l n h
do
    for var2 in v c l n h
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

# 3 строенных параметра
for var1 in v c l n h
do
    for var2 in v c l n h
    do
        for var3 in v c l n h
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    testing $i
                done
            fi
        done
    done
done

echo "${RED}FAIL: ${FAIL}${BASE}"
echo "${GREEN}SUCCESS: ${SUCCESS}${BASE}"
echo "ALL: $COUNTER"
