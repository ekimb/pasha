touch ../bench/DOCKSbench$1$2$3.txt
command time -v ./docksany $1 $2 $3 decyc$1.txt DOCKSany$1_$2_$3.txt 2>&1 | tee -a ../bench/DOCKSanybench$1$2$3.txt