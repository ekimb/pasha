for k in 9
do
for L in {42..200}
do
./src/pdocks $k $L 8 decyc$k.txt PDOCKS$k$L.txt
done
done
