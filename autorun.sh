#!/bin/bash

clear
cd $(dirname $0)

gcc -Wall security_audit.c auditdir.c utilities.c -o main.out

./main.out

rm *.out
