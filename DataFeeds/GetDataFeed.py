# -*- coding: utf-8 -*-
"""
Created on Sat Dec 29 19:40:02 2018

@author: lione
"""

import requests
import urllib.request
from matplotlib import style
import time

Namelist = ["AWK","A","DHR","ECL","FLS","PNR","SEV.PAR","SVT.LON","UUGRY","WEIR.LON","AGCO","CNI","CF","BRFS3.SAO","K","MOS","MDLZ","DSM.AMS","DE","6326.TOK","9020.TOK","SGO.PAR","SU","ABBN.VSX","AA9.BER","CSX","LR.PAR","NSC","PHIA.AMS","SIE.FRK"]


style.use('fivethirtyeight')
api_key= "FIF2L2BXUMM7RYWV"






for nameStock in Namelist:
    url = 'https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol={stock}&apikey={key}&datatype=csv&outputsize=full'.format(stock=nameStock, key=api_key)
    urllib.request.urlretrieve(url, "./Daily/Csv/{}.csv".format(nameStock))
    print(nameStock+", done.")
    time.sleep(21)

'''

model type                  <string>  bs
option size                 <int>     30
spot                        <vector>  89.11 66.98 100.82 147.81 39.29 38.805 11.45 1850.5 19.28 1355.5 60.02 77.25 44.99 21.53 57.59 30.89 41.27 72.5 155.19 1603.5 10030 29.14 29.81 18.8 18.745 62.92 49.49 151.189 29.65 97.89
maturity                    <float>   8
volatility                  <vector>  0.2
interest rate               <float>   0.02
trend                       <vector>  0.04

option type                 <string>  kozei
payoff coefficients         <vector>  0.03333333333

timestep number             <int>     16
sample number               <long>    50000
hedging dates number        <int>     450
'''

