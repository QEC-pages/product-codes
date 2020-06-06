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
# size 9 , trial410-234473
# size 7,  trial411-572582
# size 11 , trial418-104413
#trial423-52069
# size 15 trial427-1531 trial427-1421
# size 10 , trial423-52068
# size 8, trial412-784951
for trial in trial486-36 trial485-3565 trial485-500 trial412-784951 trial423-52068 trial418-104413 trial410-323232 trial427-1421 trial427-1531 trial410-234473 trial411-572582
do
    title=$folder/$trial
    echo check $title
#    ./random_concatenation.out 2 $title >>data/result/random2check/check-$trial.log &
#    ./random_concatenation.out 2 $title
#    ./counter_concatenation.out 2 $title     
    ./counter_concatenation.out mode=2 title=$title debug=1 seed=999   
    break
done
wait
echo done
date
