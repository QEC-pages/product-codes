#!/bin/bash
index=405
# 250-266  for random code on cherenkov

max_trial=1000000

#index=221 #218-221 - for reduced code.
#index=200-217 for concatenation
#index=1-112 #for hypergraph

make counter_concatenation.out

#add index by 1 while rerun this script
#the number of simultaneous process is limited by max_process.

max_process=150

case `hostname` in 
    "Chenrenkov")
	folder=data/random2
	;;
    *)
	folder=data_hpcc/random3
	;;
esac


echo start job on `hostname` $index $max_process/$max_trial `date`

(( i = 0 ))
while (( i < max_trial ))
do
    num_process=`pgrep -c counter_con`
    for (( j = num_process ; j < max_process ; j++ ))
    do
	    title=$folder/trial$index-$i
#	    ./random_concatenation.out 1 $title    >>data/result/result$index-$i.log &
	    ./counter_concatenation.out mode=1 title=$title debug=0 &
	    #>>data/result/result$index-$i.log &	    
	    #1 for generate random code
	    #./random_concatenation.out  >>data/result/result$index-$i.log &
	    echo -ne ${max_trial} $title `date` \\r
	    (( i++ ))
    done

    sleep 0.1

done


wait

echo finish job on `hostname` $index $max_process/$max_trial `date`

