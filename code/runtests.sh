#!/bin/bash
#make clean
#make

#./max.out $1
#mv generator/graphs/*_res generator/graphs/results/

OUTPUT="$1_mem"
touch .temp

regex='([0-9]+,)*[0-9]+ bytes a'
#regex='[0-9]+,[0-9]+ bytes a|[0-9]+ bytes a'
ALGORITHMS=(dfs bfs rtf)

for algo in ${ALGORITHMS[*]}
do
    # Write info to the file .temp about bytes used in memory measured with
    # valgrind when running maxflow alogirthm
    valgrind --leak-check=full ./max.out $1 $algo > .temp 2>&1
    [[ `cat .temp` =~ $regex ]]    # Use regex to extract bytes used
    MATCH=$BASH_REMATCH            # Array of matches. Only matches one instance.
    BYTES=`echo ${MATCH:0:-8}`     # Remove 8 last characters " bytes a".
    printf "%s\n" $algo >> $1_mem  # Write append name of algorithm to file.
    echo $BYTES >> $1_mem          # Append bytes used to file.
done

mv "$OUTPUT" generator/graphs/results/
rm .temp

