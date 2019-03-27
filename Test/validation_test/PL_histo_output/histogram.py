#!/usr/bin/env python3

import matplotlib.pyplot as plt
import sys
import numpy as np

with open(sys.argv[1], 'r') as histo_data:
    lines = histo_data.readlines()
    PL =[float(e.strip()) for e in lines]
    n, bins, patches = plt.hist(x=PL, bins='auto', color='#0504aa',
                                alpha=0.7, rwidth=0.85)
    plt.grid(axis='y', alpha=0.75)
    if ( sys.argv[1] == "PL_histo_Call.dat"):
        plt.plot([], [], ' ', label="Option Call")
        plt.plot([], [], ' ', label="Maturité 1 an")
        plt.plot([], [], ' ', label="Rebalancement quotidien")
        plt.plot([], [], ' ', label="Montecarlo samples: 50000")
        plt.plot([], [], ' ', label="Taille de l'échantillon: 1000")
    elif ( sys.argv[1] == "PL_histo_Asian.dat"):
        plt.plot([], [], ' ', label="Option Asian")
        plt.plot([], [], ' ', label="Maturité 1 an")
        plt.plot([], [], ' ', label="Constations mensuelles")
        plt.plot([], [], ' ', label="Rebalancement quotidien")
        plt.plot([], [], ' ', label="Montecarlo samples: 50000")
        plt.plot([], [], ' ', label="Taille de l'échantillon: 1000")
    elif ( sys.argv[1] == "PL_histo_Kozei.dat"):
        plt.plot([], [], ' ', label="Option Kozei")
        plt.plot([], [], ' ', label="Maturité 8 ans")
        plt.plot([], [], ' ', label="Rebalancement hebdomadaire")
        plt.plot([], [], ' ', label="Montecarlo samples: 50000")
        plt.plot([], [], ' ', label="Taille de l'échantillon: 100")
    plt.legend()
    plt.xlabel('Profit and Loss normalisé ( % ) ')
    plt.ylabel('Frequence')
    plt.title('Profit and Loss Histogram   (' + sys.argv[1] + ' )')
    plt.show()
