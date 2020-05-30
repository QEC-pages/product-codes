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
cd $SLURM_SUBMIT_DIR

# Concatenate BAMs
# samtools cat -h header.sam -o out.bam in1.bam in2.bam

# Print name of node
echo start job on `hostname` `date`


./run_counter_concatenation.sh
