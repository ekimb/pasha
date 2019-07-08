touch ../bench/PASHAbench$1$2.txt
command time -v ./pasha $1 $2 24 decyc$1.txt PASHA$1_$2.txt 2>&1 | tee -a ../bench/PASHAbench$1$2.txt