#!/usr/bin/env python3
import matplotlib.pyplot as plt
import sys
import numpy as np

with open(sys.argv[1], 'r') as rebal_data:
    lines = rebal_data.readlines()
    x = []
    y = []
    for l in lines:
        values = l.split()
        x.append(int(values[0]))
        values = [abs(float(e)) for e in values[1:]]
        y.append(np.mean(values))
    plt.plot(x,y, 'r--')
    plt.plot(x,y, 'b.')
    if ( sys.argv[1] == "PL_rebal_Call.dat"):
        plt.plot([], [], ' ', label="Option Call")
        plt.plot([], [], ' ', label="Maturité 1 an")
        plt.plot([], [], ' ', label="Strike = 100")
        plt.plot([], [], ' ', label="Spot = 100")
    elif ( sys.argv[1] == "PL_rebal_Asian.dat"):
        plt.plot([], [], ' ', label="Option Asian")
        plt.plot([], [], ' ', label="Maturité 1 an")
        plt.plot([], [], ' ', label="Strike = 100")
        plt.plot([], [], ' ', label="Spot = 100")
        plt.plot([], [], ' ', label="Constations mensuelles")
    elif ( sys.argv[1] == "PL_rebal_Kozei.dat"):
        plt.plot([], [], ' ', label="Option Kozei")
        plt.plot([], [], ' ', label="Maturité 8 ans")
        plt.plot([], [], ' ', label="Niveaux initiaux ~ 100")
    plt.legend()
    plt.title('Profit and Loss en fonction du nombre de rebalancement   (' + sys.argv[1] + ' )')
    plt.ylabel('Profit and Loss normalisé ( % )')
    plt.xlabel('Nombre de rebalancement')
    plt.show()
