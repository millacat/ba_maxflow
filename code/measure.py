#!/usr/bin/python3
import sys, os
import subprocess

PATH_graphs = os.path.join("generator", "graphs")

graph_files = list(filter(lambda x: x.startswith('V'), os.listdir(PATH_graphs)))
graph_files = [os.path.join(PATH_graphs, f) for f in graph_files]
graph_files.sort()

# run bash script 'runtests.sh' on all the graphs. The graphs are in DIMACS
# format in text files in the directory 'generator/graphs/'
for f in graph_files:
    subprocess.check_call(["./runtests.sh", f])

