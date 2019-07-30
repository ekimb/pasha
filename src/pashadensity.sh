touch ../bench/PASHAdensity$2$1.txt
command time -v ./density $1 $2 $3 PASHA$2_$3.txt decyc$2.txt  2>&1 | tee -a ../bench/PASHAdensity$2$1.txt