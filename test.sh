
make test.out

./test.out na=5 Gax_row=3 id_Gax=11 Gaz_row=1 id_Gaz=3 debug=1

for i in {1..10..1}
do
    continue
    ./test.out $i 
#    sleep 0.01
    
done

wait

echo "done"
