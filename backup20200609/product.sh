#!/bin/zsh -l

#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=60
#SBATCH --mem-per-cpu=1G
#SBATCH --time=7-00:00:00     
#SBATCH --output=log/product.stdout --open-mode=append
#SBATCH --error=log/product.stderror --open-mode=append
#SBATCH --mail-user=wzeng002@ucr.edu
#SBATCH --mail-type=ALL
#SBATCH --job-name="product"
#SBATCH -p batch,intel # This is the default partition, you can use any of the following; intel, batch, highmem, gpu, short
#SBATCH --export=ALL,ON_SBATCH=TRUE #add environment variable

# module itpp already load on zsh and bash
# Load samtools
# module load samtools

# Change directory to where you submitted the job from, so that relative paths resolve properly
# no need. the default folder is $SLURM_SUBMIT_DIR

WORK_STATION=HEAD
case $ON_SRUN in
    "TRUE")
	case $ON_SBATCH in
	    "TRUE")
#		echo "on sbatch"
		WORK_STATION=SBATCH
		;;
	    *)		
#		echo "on srun"
		WORK_STATION=SRUN
		pkill .product
		;;
	esac
	;;
    *)
	WORK_STATION=HEAD
#	echo "not on srun or sbatch"
	;;
esac

# Concatenate BAMs
# samtools cat -h header.sam -o out.bam in1.bam in2.bam

# Print name of node
echo
echo "============= The World is Unitary ============"
echo
echo start job on `hostname` `date`


#./run_counter_concatenation.sh

#!/bin/zsh
# record
# 408 size 8
# 407 size 7

# job name should be short, for search reason
job_name=product
index=499
# 250-266  for random code on cherenkov

max_trial=1000000
# 1: two random code; 2: identical reverse A B; 3: identical A B
sub_mode=1
#na_input=7
n_low=9
n_high=9
k_low=1
k_high=1
# note for run info
note="[n_low=$n_low, n_high=$n_high, k_low=$k_low, k_high=$k_high sub_mode=$sub_mode]"

#echo "==========   new history file " `date` >> log/history.stdout
#cat log/product.stdout >> log/history.stdout

logfile=log/${job_name}${index}-size${na_input}.log
statusfile=log/status-${job_name}${index}-size${na_input}.log
echo check logfile: $logfile statusfile:$statusfile

#index=221 #218-221 - for reduced code.
#index=200-217 for concatenation
#index=1-112 #for hypergraph

make counter_concatenation.out
cp counter_concatenation.out .${job_name}$index.out

#add index by 1 while rerun this script
#the number of simultaneous process is limited by max_process.



case `hostname` in 
    "Chenrenkov")
	folder=data/random2
	;;
    *)
	folder=data_hpcc/random3
	;;
esac

# TODO: remove above case when job finished
folder=data/random3


#echo start job on `hostname` size$na_input run$index $max_process/$max_trial `date`
#echo $SLURM_JOB_ID $SLURM_JOB_NAME $SLURM_SUBMIT_DIR 
#echo data folder $folder, log file: $logfile 


echo start job on $WORK_STATION:`hostname` size$na_input run$index max_process:$max_process/max_trial:$max_trial `date` > $logfile
echo SLURM_JOB_ID:$SLURM_JOB_ID SLURM_JOB_NAME:$SLURM_JOB_NAME SLURM_JOB_DIR:$SLURM_SUBMIT_DIR >> $logfile
echo note:$note, sub_mode:$sub_mode, n_low:$n_low, n_high=$n_high, data_folder:$folder, log_file:$logfile status_file:$statusfile >> $logfile

# duplicate info to stdout
cat $logfile
cat $logfile > $statusfile

# size 9 32768

(( i = 1 ))
(( bi = 2 ))

# it is actually 60.
(( num_cores = 60 ))
(( max_process = num_cores + 10 ))
while (( i < max_trial ))
do
    # control number of processes according to the speed and number of cores
    num_process=`pgrep -c ${job_name}`
#    echo -n num_process: $num_process , 
#    echo max_process: $max_process
    if (( num_process < num_cores  )) then
	(( max_process = max_process + max_process / 10 ))
    fi
    if (( num_process > num_cores * 2 )) then
	(( max_process = max_process - max_process / 10 ))
    fi


    for (( j = num_process ; j < max_process ; j++ ))
    do	
	    title=$folder/trial$index-$i
#	    ./random_concatenation.out 1 $title    >>data/result/result$index-$i.log &
	    #./counter_concatenation.out mode=1 title=$title debug=0 &
	    #./.product$index.out  mode=1 title=$title debug=0 na_input=$na_input seed=$i  >> $logfile &
	    ./.product$index.out  mode=1 sub_mode=$sub_mode title=$title debug=0 n_low=$n_low n_high=$n_high k_low=$k_low k_high=$k_high seed=$i  note=$note >> $logfile &
	    #>>data/result/result$index-$i.log &	    
	    #1 for generate random code
	    #./random_concatenation.out  >>data/result/result$index-$i.log &
	    if (( i  == bi )) then

		#echo -n "[$bi]" >> $logfile
		#the following is a bit strange, and show different output using less and cat
		#echo -ne ${max_trial} $title `date` \\r
		echo $num_process `date` $na_input $title  
		echo $num_process `date` $na_input $title >> $statusfile
		#echo ${max_trial} $title `date` >> $logfile
		# add 20 % for next check point
		(( bi = bi + bi / 3 + 2 ))
	    fi
	    (( i++ ))

    done

    sleep 0.2
done


wait
echo >> $logfile
echo finish job on `hostname` $index $max_process/$max_trial `date` >> $logfile
echo finish job on `hostname` $index $max_process/$max_trial `date` 

