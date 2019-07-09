touch ../bench/DOCKSbench$1$2.txt
command time -v ./docks $1 $2 decyc$1.txt DOCKS$1_$2.txt 2>&1 | tee -a ../bench/DOCKSbench$1$2.txt