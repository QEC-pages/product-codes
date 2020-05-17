#Weilei Zeng, Nov 22, 2018
make concatenation.out
#check result of some P matrix

folder=data/random/pool
#trial=trail42-7

test={trail207-14}
#test=trail207-14 trail207-45 trail207-6 trail208-16 trail209-8
#test={trail207-14,trail207-45,trail207-6,trail208-16,trail209-8}
#for i in {1..10..1}
#trail207-14 trail207-45 trail207-6 trail208-16 trail209-8
#trail207-6

#trail213-375 trail213-451 trail213-54 trail213-
#trail211-106 trail211-150 trail211-37 trail211-79
for trial in trail217-1
do
#    trial=trail
    echo $trial
    folderCm=data/random/temp #not in use
    
    filename_P1=$folder/$trial-P1.mm
    filename_P2=$folder/$trial-P2.mm
    filename_P3=$folder/$trial-P3.mm
    filename_P4=$folder/$trial-P4.mm

#    ./hypergraph.out trail_test
    more emoji/happyface >>data/result/temp1/check_4_${trial}.log 
    echo $trial >>data/result/temp1/check_4_${trial}.log 
#    ./concatenation.out $filename_P1 $filename_P2 $filename_P3 $filename_P4 $folderCm    >>data/result/temp1/check_4_${trial}.log &
    ./concatenation.out $filename_P1 $filename_P2 $filename_P3 $filename_P4 $folderCm
    
done
wait
echo $trial
date
echo done
