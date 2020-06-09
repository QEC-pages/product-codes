#!/bin/bash
index=222 #218 - for reduced code.
#index=200-217 for concatenation
#index=1-112 #for hypergraph

max_trial=500

#add index by 1 while rerun this script
#run hypergraph.out and save result into data/result/

#the number of simultaneous process is limited by max_process.
max_process=16
initial_process=5
#max_trial=16

echo start: max_process = ${max_process}, max_trial = ${max_trial}, now run: 
date
folder=data/random/pool


#for i in {1..800..1}
for (( i = 1; i < initial_process ; i++ ))
#    for (( i = 1; i < max_process ; i++ ))
do
    title=$folder/trail$index-$i
    ./concatenation.out $title    >>data/result/result$index-$i.log &
#    echo $title
    echo  max_process = ${max_process}, max_trial = ${max_trial}, now run: $title
done


(( i = initial_process ))
while (( i < max_trial ))
do
    num_process=`pgrep -c concatenation`
 #   echo ${num_process}
 #   echo $i

    for (( j = num_process ; j < max_process ; j++ ))
    do
	    title=$folder/trail$index-$i
	    ./concatenation.out $title    >>data/result/result$index-$i.log &
	    echo max_process = ${max_process}, max_trial = ${max_trial}, now run: $title
	    date
	    (( i++ ))
#	    echo $title

    done
    
    
    sleep 0.1
#    (( i = i + max_process - num_process ))
done


wait
echo finished $max_trial trials
date
