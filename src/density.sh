touch ../bench/densityBench$1$2.txt
echo "PASHA: " 2>&1 | tee -a ../bench/densityBench$1$2.txt
./density $2 $1 100 ~/Desktop/sets_july24/k$1/PASHA$1_100.txt ~/Desktop/sets_july24/k$1/decyc$1.txt 2>&1 | tee -a ../bench/densityBench$1$2.txt
echo "PDOCKS: " 2>&1 | tee -a ../bench/densityBench$1$2.txt
./density $2 $1 100 ~/Desktop/sets_july24/k$1/PDOCKS$1_100.txt ~/Desktop/sets_july24/k$1/decyc$1.txt 2>&1 | tee -a ../bench/densityBench$1$2.txt
echo "DOCKSany: " 2>&1 | tee -a ../bench/densityBench$1$2.txt
./density $2 $1 100 ~/Desktop/sets_july24/k$1/DOCKSany$1_100*.txt ~/Desktop/sets_july24/k$1/decyc$1.txt 2>&1 | tee -a ../bench/densityBench$1$2.txt


