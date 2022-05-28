#!/bin/bash
gcc question_1/question_1.c -o 1.out; ./1.out
python question_2/question_2.py
g++ question_3/question_3.cpp -o 3.out; ./3.out 
g++ question_4/question_4.cpp -o 4.out; ./4.out
g++ question_5/question_5.cpp -o 5.out; ./5.out
rm *.out