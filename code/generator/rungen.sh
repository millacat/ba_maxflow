#!/bin/bash

buildAmount=30               # How many of each size graph there is made.
#VSizes=(100 150 200 250 300 350 400 450) # |V| sizes
VSizes=`echo {100..1000..100}`
maxCapacity=10              # Edges' max capacity
constantMax=2               # Hidden constant to use when examining Theta(n)
constantMin=0.4             # Same, but Theta(n^2)

fsharpc dimacs.fs generator.fsx

max() {
    mono generator.exe $maxCapacity max $size $constantMax
}

min() {
    mono generator.exe $maxCapacity min $size $constantMin
}


# "min" is given when generating dense graphs
# "max" is given when generating sparse graphs
# When wanting to generate
for (( i=1; i <= $buildAmount; i++ ))
#for i in {1..${buildAmount}}
do
    for size in ${VSizes[*]}
    do
       # printf "\n=============== $i $size ==============\n"
        max
        while [ `echo $?` == 1 ]; do
            max
        done

        min
        while [ `echo $?` == 1 ]; do
            min
        done
      #  printf "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
    done
#    sleep 1
done

