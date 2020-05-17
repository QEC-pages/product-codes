make hypergraph.out
#check result of some P matrix

folder=data/random/pool
trial=trail42-7
trial=trail44-13
#trial=trail44-9
#trial=trail49-10
#trial=trail49-12

trial=trail53-6  #1,3,4,6

#trial=trail35-1  #4x4 for quick result
#{trail53-1,trail53-4,trail53-3,trail53-6}
trial=trail54-31
#{trail56-101,trail56-107,trail56-117,trail56-153,trail56-155}
#for i in {1..10..1}
for trial in test
do
    trial=trail207-6
    echo $trial
    folderCm=data/random/trial54-31
    
    filename_P1=$folder/$trial-P1.mm
    filename_P2=$folder/$trial-P2.mm
    filename_P3=$folder/$trial-P3.mm
    filename_P4=$folder/$trial-P4.mm

#    ./hypergraph.out trail_test
   ./hypergraph.out $filename_P1 $filename_P2 $filename_P3 $filename_P4 $folderCm    >>data/result/temp1/check_4_${trial}.log &
#    ./hypergraph.out $filename_P1 $filename_P2 $filename_P3 $filename_P4 $folderCm
    
done
wait
echo $trial
date
echo done
