#!/bin/bash
gcc question_1/question_1.c -o 1.out; ./1.out
python question_2/question_2.py
g++ question_3/question_3.cpp -o 3.out; ./3.out 

rm *.out