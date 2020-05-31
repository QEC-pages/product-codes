#make random_concatenation.out
make counter_concatenation.out

date
folder=data/random2
folder=data_hpcc/random3
#trial254-13 trial254-27 trial255-11
#trial256-106 trial256-111 trial256-134 trial256-141
#trial264-12296 trial264-14671 trial264-14740 trial264-15340 trial264-8369
# new cases
#trial403-278 
#trial410-234473
#trial411-572582
for trial in trial423-52069
do
    title=$folder/$trial
    echo check $title
#    ./random_concatenation.out 2 $title >>data/result/random2check/check-$trial.log &
#    ./random_concatenation.out 2 $title
#    ./counter_concatenation.out 2 $title     
    ./counter_concatenation.out mode=2 title=$title debug=1    
done
wait
echo done
date
