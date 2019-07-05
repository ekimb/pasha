#!/bin/sh
touch ../bench/PASHAbench$1.txt
command time -v ./pasha $1 20 24 decyc$1.txt PASHA$1_20.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 30 24 decyc$1.txt PASHA$1_30.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 40 24 decyc$1.txt PASHA$1_40.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 50 24 decyc$1.txt PASHA$1_50.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 60 24 decyc$1.txt PASHA$1_60.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 70 24 decyc$1.txt PASHA$1_70.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 80 24 decyc$1.txt PASHA$1_80.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 90 24 decyc$1.txt PASHA$1_90.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 100 24 decyc$1.txt PASHA$1_100.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 110 24 decyc$1.txt PASHA$1_110.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 120 24 decyc$1.txt PASHA$1_120.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 130 24 decyc$1.txt PASHA$1_130.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 140 24 decyc$1.txt PASHA$1_140.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 150 24 decyc$1.txt PASHA$1_150.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 160 24 decyc$1.txt PASHA$1_160.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 170 24 decyc$1.txt PASHA$1_170.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 180 24 decyc$1.txt PASHA$1_180.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 190 24 decyc$1.txt PASHA$1_190.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt
command time -v ./pasha $1 200 24 decyc$1.txt PASHA$1_200.txt 2>&1 | tee -a ../bench/PASHAbench$1.txt