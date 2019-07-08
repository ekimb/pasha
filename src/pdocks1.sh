touch ../bench/PDOCKSbench$1$2.txt
command time -v ./pdocks $1 $2 24 decyc$1.txt PDOCKS$1_$2.txt 2>&1 | tee -a ../bench/PDOCKSbench$1$2.txt