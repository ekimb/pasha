#!/bin/sh
touch ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 20 $2 decyc$1.txt DOCKSany$1_20.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 30 $2 decyc$1.txt DOCKSany$1_30.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 40 $2 decyc$1.txt DOCKSany$1_40.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 50 $2 decyc$1.txt DOCKSany$1_50.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 60 $2 decyc$1.txt DOCKSany$1_60.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 70 $2 decyc$1.txt DOCKSany$1_70.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 80 $2 decyc$1.txt DOCKSany$1_80.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 90 $2 decyc$1.txt DOCKSany$1_90.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 100 $2 decyc$1.txt DOCKSany$1_100.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 110 $2 decyc$1.txt DOCKSany$1_110.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 120 $2 decyc$1.txt DOCKSany$1_120.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 130 $2 decyc$1.txt DOCKSany$1_130.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 140 $2 decyc$1.txt DOCKSany$1_140.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 150 $2 decyc$1.txt DOCKSany$1_150.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 160 $2 decyc$1.txt DOCKSany$1_160.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 170 $2 decyc$1.txt DOCKSany$1_170.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 180 $2 decyc$1.txt DOCKSany$1_180.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 190 $2 decyc$1.txt DOCKSany$1_190.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt
command time -v ./docksany $1 200 $2 decyc$1.txt DOCKSany$1_200.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2.txt