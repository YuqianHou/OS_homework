#! /bin/bash
#将自变量形式给出的目录下的全部普通文件名和权限列出
k=`echo $1|sed 's/\/$//'`
for i in $k/*
do
if [ -f "$i" ]
then
j=`ls -l "$i"|cut -c 1-10`
echo -e "$j\t$i"
fi
done