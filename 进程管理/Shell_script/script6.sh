#!/bin/bash
#删除在自变量列表中的文件
#如果设置了 MAXFILES 变量，则将它的值作为要删除文件最大数目
#如果要删除的文件超出了这个最大数目，则在删除文件以前要请求用户的允许
ask=0
if [ "$MAXFILES" != "" -a $MAXFILES -lt $# ];then
    ask=1
fi
for i in $@
do
    if [ $ask -eq 1 ];then
        rm -i $i
        continue
    fi
    echo remove $i
    rm -f $i
done