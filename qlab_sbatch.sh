#!/bin/bash -l

#SBATCH --job-name=test-job
#SBATCH -N 1
#SBATCH -n 16
#SBATCH --time=60:00
#SBATCH -p debug
#SBATCH --output=/sharedata01/weileizeng/tmp.stdout

# module load vasp/intel

# srun -n 64$(which vasp_std)> vasp.out

# Change directory to where you submitted the job from, so that relative paths resolve properly
# cd $SLURM_SUBMIT_DIR

# cd /sharedata01/weileizeng
# Print name of node
echo start job on `hostname` `date` `pwd`

#sleep 2
#echo start job on `hostname` `date` `pwd`>> weilei.log


# ./test.out
