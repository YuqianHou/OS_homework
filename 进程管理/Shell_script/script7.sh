#!/bin/bash
#设计一个函数并递归调用之，打印出用户指定目录或当前目录中的所有文件和目录
function echo_name(){
    for file in `ls $1`
    do
        echo $1"/"$file
        if [ -d $1"/"$file ]  
        then
            echo_name $1"/"$file
        fi
    done
}
 
echo_name $1