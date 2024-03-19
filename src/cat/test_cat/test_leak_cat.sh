#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
RESULT=0
DIFF_RES=""

declare -a tests=(
"VAR test_case_cat.txt"
"VAR no_file.txt"
)

declare -a extra=(
"-s test_1_cat.txt"
"-b -e -n -s -t -v test_1_cat.txt"
"-t test_3_cat.txt"
"-n test_2_cat.txt"
"no_file.txt"
"-n -b test_1_cat.txt"
"-s -n -e test_4_cat.txt"
"test_1_cat.txt -n"
"-n test_1_cat.txt"
"-n test_1_cat.txt test_2_cat.txt"
"-v test_5_cat.txt"
)
s21_command=(
    "./s21_cat"
    )

testing() {
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose -q --log-file="${s21_command[@]}".log ./"${s21_command[@]}" $param > /dev/null
    leak=$(grep -ic -A10000 "LEAK SUMMARY:" "${s21_command[@]}".log || true)
    leak2=$(grep -ic -A10000 "ERROR SUMMARY: [^0]" "${s21_command[@]}".log || true)
    let "COUNTER++"
    if [ "$leak" -eq "0" ] && [ "$leak2" -eq "0" ]
    then
        let "SUCCESS++"
        echo "$COUNTER - Success $param"
    else
        let "FAIL++"
        echo "$COUNTER - Fail $param"
        exit 1
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}
# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in b e n s t v
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in b e n s t v
do
    for var2 in b e n s t v
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
for var1 in b e n s t v
do
    for var2 in b e n s t v
    do
        for var3 in b e n s t v
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

# 4 параметра
for var1 in b e n s t v
do
    for var2 in b e n s t v
    do
        for var3 in b e n s t v
        do
            for var4 in b e n s t v
            do
                if [ $var1 != $var2 ] && [ $var2 != $var3 ] \
                && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
                && [ $var2 != $var4 ] && [ $var3 != $var4 ]
                then
                    for i in "${tests[@]}"
                    do
                        var="-$var1 -$var2 -$var3 -$var4"
                        testing $i
                    done
                fi
            done
        done
    done
done

echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"
