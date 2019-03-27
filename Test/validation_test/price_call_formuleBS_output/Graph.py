#!/usr/bin/env python3
import matplotlib.pyplot as plt
import sys
import numpy as np

with open(sys.argv[1], 'r') as rebal_data:
    lines = rebal_data.readlines()
    x = []
    y_pricer = []
    y_BS = []
    for l in lines:
        values = l.split()
        x.append(float(values[0]))
        y_pricer.append(float(values[1]))
        y_BS.append(float(values[2]))
    plt.subplot(2, 1, 1)
    plt.title('Comparaison entre "pricer" et Black&Scholes (' + sys.argv[1] + ' )')
    plt.plot(x,y_pricer)
    plt.ylabel('Prix calculé par le pricer')
    plt.subplot(2, 1, 2)
    plt.plot(x,y_BS, color='r')
    plt.ylabel('Prix de la formule de B&S')
    if ( sys.argv[1] == "delta.dat"):
        plt.xlabel('Valeur du Spot')
    elif ( sys.argv[1] == "vega.dat"):
        plt.xlabel("Valeur de Sigma (volatilité)")
    elif ( sys.argv[1] == "rho.dat"):
        plt.xlabel("Taux sans risque r")
    elif ( sys.argv[1] == "spread.dat"):
        plt.xlabel("Strike k")
    elif ( sys.argv[1] == "Theta.dat"):
        plt.xlabel("date t de 0 à maturité")
        #plt.plot([], [], ' ', label="Niveaux initiaux ~ 100")
    plt.show()
