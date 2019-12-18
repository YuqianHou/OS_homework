#!/bin/bash
#用while循环编写一个 shell 程序，它将自变量按其逆序的形式打印出来

n=$#
#for i in `seq 1 $#`
while
test $n -gt 0
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