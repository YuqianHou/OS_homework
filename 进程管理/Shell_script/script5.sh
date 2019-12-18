#! /bin/bash
#根据输入的自变量的类型而采取不同的动作
#是目录时则显示目录中的文件
#是可执行文件则在 shell 中执行它
#是可读文件时则显示它的内容。
dir=$1
if [ -d $dir ];then
ls $dir
elif [ -x $dir ];then
sh $dir
elif [ -r $dir ];then
more $dir
else
ll $dir
fi