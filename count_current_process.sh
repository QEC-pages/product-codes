#get number of running process
max=10

num=`pgrep -c hypergraph`
num=`pgrep -c concatenation`

#echo current number of running process is $num

for ((i=0;i<200;i++));
#for i in {1.."$num"..1}
do

    num=`pgrep -c random_concat`
    echo current number of running process is $num
#    echo $i
    sleep 0.5
done
