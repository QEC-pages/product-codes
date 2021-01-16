#This script check specific cases according to their file prefix/title

##make random_concatenation.out
#make counter_concatenation.out
make product

date
folder=data/random2
folder=data_hpcc/random3
folder=data/random4
# folder to save counter examples
report_folder=report/matrices

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


# trial516/trial516-na8-Gax_row4-id_Gax61111-Gaz_row3-id_Gaz2386
# size 8 d=7 when identical, which is a bit unexpected
# d=8 when reverse symmetrical, that is proved

cases_size7="trial555/trial555-A-7-3-3511-3-2657-B-7-3-3511-3-2657 \
trial555/trial555-A-7-3-3799-3-2371-B-7-3-3511-3-2657 \
trial560/trial560-A-7-3-3511-3-2657-B-7-3-3511-3-2657 \
trial560/trial560-A-7-3-3799-3-2371-B-7-3-3511-3-2657 \
trial560/trial560-A-7-3-3799-3-2371-B-7-3-3799-3-2371 \
trial560/trial560-A-7-3-3803-3-2131-B-7-3-3767-3-2386 \
trial560/trial560-A-7-3-3767-3-2386-B-7-3-3511-3-2657 \
trial560/trial560-A-7-3-3767-3-2386-B-7-3-3799-3-2371 \
trial560/trial560-A-7-3-3767-3-2386-B-7-3-3767-3-2386"


for trial in ${cases_size7}
#trial550/trial550-971 trial550/trial550-106 trial548/trial548-5 trial516/trial516-na8-Gax_row4-id_Gax61111-Gaz_row3-id_Gaz2386 trial508/trial508-na7-Gax_row3-id_Gax3511-Gaz_row3-id_Gaz2657 trial508/trial508-na7-Gax_row3-id_Gax3767-Gaz_row3-id_Gaz2386 trial508/trial508-na7-Gax_row3-id_Gax3803-Gaz_row3-id_Gaz2131 trial508/trial508-na7-Gax_row3-id_Gax3799-Gaz_row3-id_Gaz2371
#trial501-na7-Gax_row2-id_Gax455-Gaz_row2-id_Gaz1008 trial501-na7-Gax_row2-id_Gax455-Gaz_row2-id_Gaz527 trial493-16816 trial493-17128 trial486-36 trial485-3565 trial485-500 trial412-784951 trial423-52068 trial418-104413 trial410-323232 trial427-1421 trial427-1531 trial410-234473 trial411-572582
do
    title=$folder/$trial
    echo check $title
    #copies files
    cp $folder/${trial}Gax.mm $folder/${trial}Gaz.mm $folder/${trial}Gbx.mm $folder/${trial}Gbz.mm $report_folder

#    ./random_concatenation.out 2 $title >>data/result/random2check/check-$trial.log &
#    ./random_concatenation.out 2 $title
#    ./counter_concatenation.out 2 $title 


#    ./counter_concatenation.out mode=2 title=$title debug=0 seed=999 &
#    mkdir -p report/trial516
    ./product.out mode=2 title=$title debug=0 seed=999 
#>> report/${trial}.log &
#    break
done
wait
echo done
date
