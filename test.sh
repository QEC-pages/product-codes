
make test.out

for i in {1..300..1}
do
    ./test.out $i &
#    sleep 0.01
done

wait

echo "done"
