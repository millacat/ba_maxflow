# Execution of the code
This is an overview on how to run the different files which is used to build
graphs, to conducting experiments with and create resulting plots. The order
they are described in is the order they should be run in.

F#, python 3, C, valgrind and bash are required to run it all.

## Run full experiment
Go to directory `code/generator`

```
./rungen
```

Go to directory `code/`

```
make
python3 measure.py
find generator/graphs/ -maxdepth 1 -name "V100-max*" -print | wc -l | xargs python3 plot.py
```

## Generate graphs
### generator.fsx

Generates a directed, weighted graph saved in a text file in DIMACS format. The
resulting file is placed in the directory `code/generator/`.

To run:

Go to the directory `code/generator/`.
```
fsharpc buildDIMACS.fs generator.fsx
mono generator.exe <Edges' max capacity> <bound> <n> <c>
```
E.g.:
```
fsharpc buildDIMACS.fs generator.fsx
mono generator.exe 10 max 250 2
```

- < Edges' max capacity > is the upper bound for possible capacities of edges.
The capacity of an edge will be an integer between 1 and max capacity.

- < bound > is either 'max' or 'min':
    * 'max' - there can maximum be made c*n edges.
    * 'min' - there are minimum made c*n^2 edges.
- < n > is |V|, being the number of vertices.
- < c > is the hidden constant c - either floating point or integer.

Many graphs are made if executing the script `rungen.sh`. See top of the
`rungen.sh` file for more information. The graph files are placed in directory
`code/generator/graphs`.

To run:

Go to the directory `code/generator/`.

```
./rungen.sh
```

## Conduct experiments on graphs

### main.c
This file contains the main function. The three maximum flow algorithms
implemented is run from here. Time measuring of the algorithms is done if only
the path to the graph file is given as argument.
If a specific algorithm also is given as argument this algorithm is run once
and time measurement is not done.

If only the path is given it produces a text file with the time measurements in
for each algorithm. This file is placed in `generator/graphs/results`.

To run:
```
    make
    ./max.out <path to graph>
    ./max.out <path to graph> <dfs or bfs or rtf>
```
E.g.:
```
    make
    ./max.out generator/graphs/V10-max-c2_E20_04-15-2020_12:02:32 dfs
```


### runtests.sh
Firstly you should compile the C code using `make` and then run
`runtests.sh` which runs the executable four times. First time
is to do the time measuring on the different maximum flow algorithms.
The next three times is one time for each algorithm where the memory allocated
is measured with the program `valgrind`.

To run:
```
make
./runtests.sh <path to graph>
```

### measure.py

This python script makes the call `./runtests.sh <path to graph>` one time for
each DIMACS graph file in directory `generator/graphs/`.

To run:
```
make
python3 measure.py
```


## Generate plots from result files
### plot.py
Generate plots from files containing memory- and time measurements from running
maximum flow algorithms on graphs in DIMACS format.

The files are present in `generator/graphs/results/`.

The plots made are placed in `plots/` in .pdf format.

To run:

```
python3 plot.py <number of files with specific |V|>
```
E.g.:
```
python3 plot.py 10
```

< number of files with specific |V| / 2 > is how many graph files exists for
each |V| but divided by 2. If |V| = 10 then it is 5 files for exploring m = O(n)
and 5 for exploring m = O(n^2).
|V| is decided when the graph files are build using
`generator/generator.fsx`.
