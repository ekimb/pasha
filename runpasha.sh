for k in {5..9}
do
for L in {20..200}
do
./pasha $k $L 8 decyc$k.txt PASHA$k_$L.txt
done
done