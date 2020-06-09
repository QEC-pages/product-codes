index=207 #200 for concatenation


#add index by 1 while rerun this script
#run hypergraph.out and save result into data/result/

#assertation fail while kappa=0, it is not a quantum code. we haven't consider this case yet

echo It may take 2 mins to finish
date
folder=data/random/pool

for i in {1..16..1}
#	 for i in {1..16..1}
do
    title=$folder/trail$index-$i
    ./concatenation.out $title    >>data/result/result$index-$i.log &
#    ./concatenation.out $title
done
wait
echo finished
date
