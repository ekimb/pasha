#!/bin/sh
touch ../bench/DOCKSbench$1.txt
command time -v ./docks $1 20 decyc$1.txt DOCKS$1_20.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 30 decyc$1.txt DOCKS$1_30.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 40 decyc$1.txt DOCKS$1_40.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 50 decyc$1.txt DOCKS$1_50.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 60 decyc$1.txt DOCKS$1_60.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 70 decyc$1.txt DOCKS$1_70.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 80 decyc$1.txt DOCKS$1_80.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 90 decyc$1.txt DOCKS$1_90.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 100 decyc$1.txt DOCKS$1_100.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 110 decyc$1.txt DOCKS$1_110.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 120 decyc$1.txt DOCKS$1_120.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 130 decyc$1.txt DOCKS$1_130.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 140 decyc$1.txt DOCKS$1_140.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 150 decyc$1.txt DOCKS$1_150.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 160 decyc$1.txt DOCKS$1_160.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 170 decyc$1.txt DOCKS$1_170.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 180 decyc$1.txt DOCKS$1_180.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 190 decyc$1.txt DOCKS$1_190.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt
command time -v ./docks $1 200 decyc$1.txt DOCKS$1_200.txt 2>&1 | tee -a ../bench/DOCKSbench$1.txt