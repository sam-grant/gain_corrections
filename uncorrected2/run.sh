#!/bin/bash
for file in `ls ../../60hr_v9_17_01/gm2*.root`; do
    echo $file
     ./plotter_time_normalised_xtal $file
done

hadd energy_and_momentum_xtal2.root plots*.root
