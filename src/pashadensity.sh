touch ../bench/PASHAdensity$2$1.txt
command time -v ./density $1 $2 $3 PASHA$1_$2.txt decyc$1.txt  2>&1 | tee -a ../bench/PASHAdensity$2$1.txt