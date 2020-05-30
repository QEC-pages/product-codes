#!/bin/zsh -l

#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=32
#SBATCH --mem-per-cpu=1G
#SBATCH --time=0-05:00:00     
#SBATCH --output=log/size8.stdout
#SBATCH --mail-user=wzeng002@ucr.edu
#SBATCH --mail-type=ALL
#SBATCH --job-name="concatenation"
#SBATCH -p intel # This is the default partition, you can use any of the following; intel, batch, highmem, gpu


# module itpp already load on zsh and bash

# Load samtools
# module load samtools


# Change directory to where you submitted the job from, so that relative paths resolve properly
case $SLURM_SUBMIT_DIR in
    "/rhome/wzeng002/Documents/GitHub/workspace")
	echo "test run"
	;;
    *)
	echo "run in sbatch"
	;;
esac


# Concatenate BAMs
# samtools cat -h header.sam -o out.bam in1.bam in2.bam

# Print name of node
echo start job on `hostname` `date`


#./run_counter_concatenation.sh


#!/bin/zsh
# record
# 408 size 8
# 407 size 7

index=409
# 250-266  for random code on cherenkov

max_trial=1000000
na_input=9

#index=221 #218-221 - for reduced code.
#index=200-217 for concatenation
#index=1-112 #for hypergraph

make counter_concatenation.out
cp counter_concatenation.out .run$index.sh

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


echo start job on `hostname` size$na_input run$index $max_process/$max_trial `date`
echo one dot means 1000 test
(( i = 0 ))
while (( i < max_trial ))
do
    num_process=`pgrep -c counter_con`
    for (( j = num_process ; j < max_process ; j++ ))
    do
	    title=$folder/trial$index-$i
#	    ./random_concatenation.out 1 $title    >>data/result/result$index-$i.log &
	    #./counter_concatenation.out mode=1 title=$title debug=0 &
	    ./.run$index.sh  mode=1 title=$title debug=0 na_input=$na_input &
	    #>>data/result/result$index-$i.log &	    
	    #1 for generate random code
	    #./random_concatenation.out  >>data/result/result$index-$i.log &
	    if (( i % 1000 == 0 )) then 
		echo -n "." 
		#the following is a bit strange, and show different output using less and cat
		#echo -ne ${max_trial} $title `date` \\r
	    fi
	    (( i++ ))
    done

    sleep 0.1

done


wait
echo 
echo finish job on `hostname` $index $max_process/$max_trial `date`
