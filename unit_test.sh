echo make sure these tests have been passed


#make random_concatenation.out
make counter_concatenation.out

echo expected results: find case for all of them.
echo dcx and dcz for size 9, and size 7

date
folder=data/random2
folder=data_hpcc/random3
#trial254-13 trial254-27 trial255-11
#trial256-106 trial256-111 trial256-134 trial256-141
#trial264-12296 trial264-14671 trial264-14740 trial264-15340 trial264-8369
# new cases
#trial403-278 
# size 9 , trial410-234473
# size 7, trial411-572582
#trial423-52069
for trial in trial410-234473 trial411-572582
do
    title=$folder/$trial
    echo check $title
#    ./random_concatenation.out 2 $title >>data/result/random2check/check-$trial.log &
#    ./random_concatenation.out 2 $title
#    ./counter_concatenation.out 2 $title     
    ./counter_concatenation.out mode=2 title=$title debug=0 seed=999   
done
wait

echo done

date
