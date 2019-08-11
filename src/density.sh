touch ../bench/densityBench$1.txt
echo "PASHA: " 2>&1 | tee -a ../bench/densityBench$1.txt
./density rawgenome.txt $1 100 ~/pasha/src/PASHA$1_100.txt ~/pasha/src/decyc$1.txt 2>&1 | tee -a ../bench/densityBench$1.txt
echo "PDOCKS: " 2>&1 | tee -a ../bench/densityBench$1.txt
./density rawgenome.txt $1 100 ~/pasha/src/PDOCKS$1_100.txt ~/pasha/src/decyc$1.txt 2>&1 | tee -a ../bench/densityBench$1.txt
echo "DOCKSany: " 2>&1 | tee -a ../bench/densityBench$1.txt
./density rawgenome.txt $1 100 ~/pasha/src/DOCKSany$1_100*.txt ~/pasha/src/decyc$1.txt 2>&1 | tee -a ../bench/densityBench$1.txt