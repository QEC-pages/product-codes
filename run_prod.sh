#!/bin/zsh -l

#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=64
#SBATCH --mem-per-cpu=1G
#SBATCH --time=0-02:00:00     
#SBATCH --output=log/product.stdout --open-mode=append
#SBATCH --error=log/product.stderror --open-mode=append
#SBATCH --mail-user=wzeng002@ucr.edu
#SBATCH --mail-type=ALL
#SBATCH --job-name="product"
#SBATCH -p short,batch,intel # This is the default partition, you can use any of the following; intel, batch, highmem, gpu, short
#SBATCH --export=ALL,ON_SBATCH=TRUE #add environment variable

# module itpp already load on zsh and bash
# module load itpp

# Change directory to where you submitted the job from, so that relative paths resolve properly
# no need. the default folder is $SLURM_SUBMIT_DIR

WORK_STATION=HEAD
(( num_cores = 15 ))
# ON_SRUN and ON_SBATCH are manually defined environment variable when requesting the resource.
case $ON_SRUN in
    "TRUE")
	case $ON_SBATCH in
	    "TRUE")
		echo "on sbatch"
		WORK_STATION=SBATCH
		(( num_cores = 60 ))
		;;
	    *)		
		echo "on srun"
		WORK_STATION=SRUN		
		if (( `pgrep -c .product` > 10 )); then		    
		    echo "one should kill `pgrep -c .product` old jobs"
#		    pkill .product
		    # this will kill ./run_product.sh as well, so I change the name to run_prod.sh
		fi
		;;
	esac
	;;
    *)
	WORK_STATION=HEAD
#	echo "not on srun or sbatch"
	;;
esac

# add this when running srun on partition short
#(( num_cores = 60 ))

# Print name of node
echo
echo "============= The World is Unitary ============"
echo start job on `hostname` `date`
echo


# job name should be short, for search reason
job_name=product
index=552
# 250-266  for random code on cherenkov

max_trial=1000 #1000000
debug=0
mode=3
# mode:: 1:random codes; 2: check case; 3: enumerate all cases
#sub_mode=3 #not used when mode=3
sub_mode_A=1
#sub_mode_A:: 1: random; 2: enumerate all
sub_mode_B=4 #4 #enumerate code B as well
#sub_mode_B:: 1: random; 2: reverse symmetry; 3: symmetry/identical; 4: enumerate all cases
na_input=6

n_low=7
n_high=7
k_low=1
k_high=1
# note for run info
note="[n_low=$n_low, n_high=$n_high, k_low=$k_low, k_high=$k_high sub_mode=$sub_mode]"


logfile=log/${job_name}${index}-size${na_input}.log
#statusfile=log/status-${job_name}${index}-size${na_input}.log
echo check logfile: $logfile 
#statusfile:$statusfile

#index=221 #218-221 - for reduced code.
#index=200-217 for concatenation
#index=1-112 #for hypergraph

#make counter_concatenation.out
#cp counter_concatenation.out .${job_name}$index.out
#make product.out
make product
cp product.out .${job_name}$index.out

#add index by 1 while rerun this script
#the number of simultaneous process is limited by max_process.


#folder=data/random3
#change to new folder, only save code with cases, add sub folder for each run
folder=data/random4
mkdir -p $folder/trial$index
folder=$folder/trial$index

echo start job on $WORK_STATION:`hostname` size$na_input run$index max_process:$max_process/max_trial:$max_trial `date` > $logfile
echo SLURM_JOB_ID:$SLURM_JOB_ID SLURM_JOB_NAME:$SLURM_JOB_NAME SLURM_JOB_DIR:$SLURM_SUBMIT_DIR >> $logfile
echo note:$note, sub_mode_A:$sub_mode_A, sub_mode_B:$sub_mode_B, na_input:$na_input, \
    n_low:$n_low, n_high=$n_high, data_folder:$folder, \
    num_core=$num_core, log_file:$logfile status_file:$statusfile >> $logfile

# duplicate info to stdout
cat $logfile
#cat $logfile > $statusfile


#(( i = 1 ))

# it is actually 60.
#(( num_cores = 15 ))
#(( max_process = num_cores + 10 ))


title=$folder/trial$index

cmd () {
#for mode=3
#    ./.product$index.out  mode=$mode   sub_mode_B=$sub_mode_B title=$title debug=$debug na_input=$na_input seed=$i  num_cores=$num_cores note=$note 
# for mode=1
#    ./.product$index.out  mode=1 sub_mode_A=$sub_mode_A sub_mode_B=$sub_mode_B title=$title debug=0 n_low=$n_low n_high=$n_high k_low=$k_low k_high=$k_high seed=$i  note=$note max_trial=$max_trial
# for all
    ./.product$index.out  mode=$mode sub_mode_A=$sub_mode_A sub_mode_B=$sub_mode_B \
	title=$title debug=0 seed=1  note=$note num_cores=$num_cores \
	n_low=$n_low n_high=$n_high k_low=$k_low k_high=$k_high max_trial=$max_trial \
	na_input=$na_input 
}
declare -f cmd

case $WORK_STATION in
    "SRUN")
	cmd
#	./.product$index.out  mode=3  title=$title debug=0 na_input=$na_input seed=$i  num_cores=$num_cores note=$note
	;;
    "SBATCH")
	cmd >> $logfile
#	./.product$index.out  mode=3  title=$title debug=0 na_input=$na_input seed=$i  num_cores=$num_cores note=$note >>$logfile
	;;
esac
#./.product$index.out  mode=1 sub_mode_A=$sub_mode_A sub_mode_B=$sub_mode title=$title debug=1 n_low=$n_low n_high=$n_high k_low=$k_low k_high=$k_high seed=$i  note=$note 
#>> $logfile
date

wait
echo >> $logfile
echo finish job on `hostname` $index $max_process/$max_trial `date` >> $logfile
echo finish job on `hostname` $index $max_process/$max_trial `date` 

