#!/bin/bash
# 打印出1至20之间的平方和立方数

echo -e "数\t平方\t立方"
for i in `seq 1 20`
do
echo -e "$i\t$(($i*$i))\t$(($i*$i*$i))"
done