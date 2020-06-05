#!/usr/bin/python3

###############################################################################
### Generate plots from files containing memory- and time measurements from ###
### running maximum flow algorithms on graphs in DIMACS format              ###
###############################################################################
## To run:         python3 plot.py <number of files with specific |V| / 2>   ##
## For each |V| = n there was generated a specific number of files that is   ##
## explored here. E.g. there is 10 files with |V| = 50, 100, 150 etc.        ##
## Then it is 5 for exploring m = O(n) and 5 for exploring m = O(n^2)        ##
## Then run as follows:                                                      ##
##                 python3 plot.py 5                                         ##
###############################################################################

import os, sys
from matplotlib import pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import re
import math
import numpy as np

def load_file(file):
    ## Get content/data of file as string
    ## Arg:     Path to file
    ## Returns: Data in file
    fp = open(file, 'r')
    data = fp.read()
    fp.close()
    return data

# The number of files with the same n = |V|
file_amount = int(sys.argv[1])
PATH_results = os.path.join("generator", "graphs", "results")

def get_files(ends_with, path):
    ## Get all files which names ends with 'ends_with' and then sort them into
    ## two lists: files with 'max': m = O(n) and 'min': m = O(n^2)
    ## Args:    - ends_with: String specifying wanted end of filenames
    ##          - path:      Path to find files
    ## Returns: Two lists of files
    files = list(filter(lambda x: x.endswith(ends_with), os.listdir(path)))
    max_files = []
    min_files = []
    for f in files:
        if "max" in f:
            max_files.append(f)
        else:
            min_files.append(f)
    return max_files, min_files

def get_data(files):
    ## Load files data and remove everything but the measurements/numbers. Save
    ## info in list of relevant algorithm
    ## Arg:     A list of files
    ## Returns: A list of three lists, first for Ford-Fulkerson, second for
    ##          Edmonds-Karp and thrid for Relabel-to-Front. Each containing
    ##          sorted tuples of (n, memory measure)

    #       dfs, bfs, rtf
    algos = [[], [], []]

    for f in files:
        # get |V| from filename
        V = int(re.search(r'\d+', f).group())
        data = load_file(os.path.join(PATH_results, f))
        # strip away non numbers and remove comma from number: 24,333 = 24333
        res = [int(l.replace(',', '')) for l in data.splitlines()
                if l != 'bfs' and l != 'dfs' and l != 'rtf' and l != '']

        # Append tuple consisting of (x,y) == (|V|, mem measure)
        for algo in range (0, len(algos)):
            algos[algo].append((V, res[algo]))

    for algo in algos:
        algo.sort()

    return algos

def calc_medians(files):
    ## Load files data and remove everything but the measurements/numbers. Save
    ## info in list of relevant algorithm
    ## Arg:     A list of files
    ## Returns: A list of three lists, first for Ford-Fulkerson, second for
    ##          Edmonds-Karp and thrid for Relabel-to-Front. Each containing
    ##          sorted tuples of (n, memory measure)

    #       dfs, bfs, rtf
    algos = [[], [], []]
    for f in files:
        V = int(re.search(r'\d+', f).group())
        data = load_file(os.path.join(PATH_results, f))
        # strip away non numbers
        res = [int(l) for l in data.splitlines() if l != 'bfs' and l != 'dfs' and
                                                    l != 'rtf' and l != '']
        ## Sort the time measures out into relevant lists ##
        # find how many measurements there is for each algo
        measures = int(len(res) / 3)
        dfs = sorted(res[:measures])           # The first share belongs to dfs
        bfs = sorted(res[measures:2*measures]) # The next to bfs
        rtf = sorted(res[2*measures:])         # The last to rtf
        median_index = int(measures / 2)

        # Append tuple consisting of (x,y) == (|V|, median time measure)
        algos[0].append((V, dfs[median_index]))
        algos[1].append((V, bfs[median_index]))
        algos[2].append((V, rtf[median_index]))

    for algo in algos:
        algo.sort()

    return algos

def get_min_avg_max(coords):
    ## Find the maximum-, minimum- and average measurement for each n = |V| for
    ## one maxflow algorithm
    ## Arg: A sorted list of tuples of (x,y) = (V, time measurement) for all
    ##      measurement/results of one maxflow algorithm
    ## Returns: Three lists of tuples of (V, time measurement)
    #print(coords, "\n")
    min_c, avg, max_c = [], [], []

    for i in range(0, len(coords), file_amount):
        subset = coords[i : i + file_amount]

        min_c.append(subset[0])
        max_c.append(subset[file_amount - 1])
        average = sum(v[1] for v in subset) / file_amount
        x = coords[i][0]
        avg.append((x, average))

    return min_c, avg, max_c

def plot(coords, caption, yLabel, constant, time):
    ## Generate a plot for a maxflow algorithm where for each |V| (x-axis) there
    ## are three points on the y-axis: - the maximum measurement,
    ##                                 - the minimum measurement,
    ##                                 - the average of all measurements
    ## Args:    - coords: A sorted list of points/coordinates
    ##          - caption:  The headline for the plot
    ##          - yLabel:   Text for y-axis' label
    ##          - constant: The hidden constant
    ##          - time:     If True the plot for time- is made otherwise
    ##                      plot for memory measurement is made
    ## Returns: The figure containing the plot
    min_c, avg, max_c = get_min_avg_max(coords)
    print(caption)
    print("min, avg, max:\n", min_c, "\n", avg, "\n", max_c)

    if time:
        c1 = 'plum'
        c2 = 'teal' #'mediumorchid'
        c3 = 'magenta'
    else:
        c1 = 'darkred'
        c2 = 'darkorange'
        c3 = 'goldenrod'

    fig = plt.figure()
    # *zip is unzip. Puts the first entry of tuples in one list and the second
    # entry in the other
    plt.scatter(*zip(*max_c), color=c1, label='max')
    plt.scatter(*zip(*avg),   color=c2, label='avg')
    plt.scatter(*zip(*min_c), color=c3, label='min')
    plt.legend()
    plt.title(caption, size=20)
    plt.xlabel('n, number of vertices', size=16)
    plt.xticks(np.arange(100, 100*(len(coords) / file_amount) + 1, 100.))
    plt.ylabel(yLabel, size=16)
    txt = "c: " + constant
    plt.figtext(0.5, 0.01, txt, wrap=True, horizontalalignment='left',
                fontsize=13)
    plt.show() # Has to show() the plot, otherwise the xlabel and the 'txt' is
               # shown the same place
    return fig

def generate_plots(titles, pre_caption, captions, results, yLab, constant, time):
    ## For each algorithm in 'results' it makes a plot and saves it as .pdf
    ## Args:    - titles:       List of names for .pdf's
    ##          - pre_caption:  First part of the headline in plots
    ##          - captions:     List of second part of headline in plots
    ##          - yLab:         Text for y-axis' label
    ##          - constant:     The hidden constant
    ##          - time:         If True the plots for time- is made otherwise
    ##                          plots for memory measurement is made
    ## Returns: Nothing
    for i in range (0, len(results)):
        p = plot(results[i], pre_caption + captions[i], yLab, constant, time)
        pdf = PdfPages('plots/' + titles[i] + '.pdf')
        pdf.savefig(p, bbox_inches='tight')
        pdf.close()

def get_constant(file):
    ## Arg:     A filename as a string
    ## Returns: The hidden constant from the asymptotic notation as a string
    c = re.findall('c\d.\d+|c\d', file)
    return c[0][1:]

def main_time(captions_max, captions_min):
    ## Binds all together about the time consuption measurements.
    ## Results in plots showing the results for the three algorithms explored:
    ## 1) Ford-Fulkerson, 2) Edmonds-karp, 3) Relabel-to-Front
    ## Args:    The captions/headlines for the O(n) (max) and O(n^2) (min) time
    ##          measurements
    ## Returns: Nothing
    max_files, min_files = get_files('_res', PATH_results)
    pre_caption = 'Time consumption\n'

    c2 = get_constant(max_files[0])
    max_res = calc_medians(max_files)
    titles_max = ['timeDfsOn', 'timeBfsOn', 'timeRtfOn']
    generate_plots(titles_max, pre_caption, captions_max, max_res, 'time', c2,
                   True)

    c1 = get_constant(min_files[0])
    min_res = calc_medians(min_files)
    titles_min = ['timeDfsOn2', 'timeBfsOn2', 'timeRtfOn2']
    generate_plots(titles_min, pre_caption, captions_min, min_res, 'time', c1,
                   True)

def main_memory(captions_max, captions_min):
    ## Binds all together about the memory consuption measurements.
    ## Results in plots showing the results for the three algorithms explored:
    ## 1) Ford-Fulkerson, 2) Edmonds-karp, 3) Relabel-to-Front
    ## Args:    The captions/headlines for the O(n) (max) and O(n^2) (min)
    ##          memory measurements
    ## Returns: Nothing
    max_files, min_files = get_files('_mem', PATH_results)
    pre_caption = 'Memory consumption'

    max_res = get_data(max_files)
    c2 = get_constant(max_files[0])
    titles_max = ['memDfsOn', 'memBfsOn', 'memRtfOn']
    generate_plots(titles_max, pre_caption, captions_max, max_res, 'bytes', c2,
                   False)

    min_res = get_data(min_files)
    c1 = get_constant(min_files[0])
    titles_min = ['memDfsOn2', 'memBfsOn2', 'memRtfOn2']
    generate_plots(titles_min, pre_caption, captions_min, min_res, 'bytes', c1,
                   False)

def main():
    captions_max = ['m = O(n) : Ford-Fulkerson', 'm = O(n) : Edmonds-Karp',
                    'm = O(n) : Relabel-to-Front']

    captions_min = ['m = O(n^2) : Ford-Fulkerson', 'm = O(n^2) : Edmonds-Karp',
                    'm = O(n^2) : Relabel-to-Front']

    main_time(captions_max, captions_min)
    main_memory(captions_max, captions_min)

main()

