
make test.out

#trial501-na7-Gax_row2-id_Gax455-Gaz_row2-id_Gaz1008

./test.out na=7 Gax_row=2 id_Gax=455 Gaz_row=2 id_Gaz=1008 debug=0
./test.out na=7 Gax_row=2 id_Gax=455 Gaz_row=2 id_Gaz=527 debug=0
./test.out na=7 Gax_row=2 id_Gax=455 Gaz_row=2 id_Gaz=1007 debug=0

# 7 qubit code
./test.out na=7 Gax_row=3 id_Gax=3511 Gaz_row=3 id_Gaz=2657 debug=1

#trial508-na7-Gax_row3-id_Gax3767-Gaz_row3-id_Gaz2386

for i in {1..10..1}
do
    continue
    ./test.out $i 
#    sleep 0.01
    
done

wait

echo "done"
