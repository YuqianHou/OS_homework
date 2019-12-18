#!/bin/bash
#用until循环编写一个 shell 程序，它将自变量按其逆序的形式打印出来

n=$#
until test $n -le 0
do
# let j=$#-i+1
# eval echo \$$j
# eval i='$'$n
# echo $i
# n=' expr $n - 1 '
# eval echo \$$n
# n=' expr $n - 1 '
echo -n ${@:$n:1}" "
n=$[n-1]
done
echo