#!/bin/bash

for file in `ls /unix/muons/g-2/omegaEU/60h_5036B/gm2*.root`; do

    echo $file
    # ignore truncated file 
    #[[ $file == *30.root ]] && continue ; 

    ./Plotter $file

done

hadd merged_uncorrected.root plots*.root
