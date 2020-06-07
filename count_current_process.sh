#!/bin/bash
#get number of running process
max=10

num=`pgrep -c hypergraph`
num=`pgrep -c concatenation`

#echo current number of running process is $num

job_name=.product
for (( i=0;i<200;i++ ));
#for i in {1.."$num"..1}
do

    num=`pgrep -c $job_name`
    echo current number of running process of $job_name is $num
#    echo $i
    sleep 0.5
done
