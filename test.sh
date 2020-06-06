
make test.out

for i in {1..30..1}
do
    ./test.out $i &
#    sleep 0.01
done

wait

echo "done"
