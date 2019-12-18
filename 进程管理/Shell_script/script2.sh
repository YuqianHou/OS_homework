#!/bin/bash
#对自变量为“yes”时返回零出口状态，否则就返回1出口状态。
#将 y,yes,Yes,YES 和 Y 都看成是合法的“yes”自变量。
#read ANS
if [ $1 = "y" ]||[ $1 = "yes" ]||[ $1 = "Yes" ]||[ $1 = "YES" ]||[ $1 = "Y" ]
then
    #echo "$?"
    exit 0
else
    #echo "$?"
    exit 1
fi