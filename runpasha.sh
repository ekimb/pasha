for k in {5..9}
do
for L in {20..200}
do
./src/pdocks $k $L 8 decyc$k.txt PDOCKS$k_$L.txt
done
done
