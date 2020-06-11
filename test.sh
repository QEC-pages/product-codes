
make test.out

#trial501-na7-Gax_row2-id_Gax455-Gaz_row2-id_Gaz1008
#./test.out na=5 Gax_row=3 id_Gax=11 Gaz_row=1 id_Gaz=3 debug=1
./test.out na=7 Gax_row=2 id_Gax=455 Gaz_row=2 id_Gaz=1008 debug=0
./test.out na=7 Gax_row=2 id_Gax=455 Gaz_row=2 id_Gaz=527 debug=0
./test.out na=7 Gax_row=2 id_Gax=455 Gaz_row=2 id_Gaz=1007 debug=0

for i in {1..10..1}
do
    continue
    ./test.out $i 
#    sleep 0.01
    
done

wait

echo "done"
