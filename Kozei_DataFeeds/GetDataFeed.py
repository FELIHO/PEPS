# -*- coding: utf-8 -*-
"""
Created on Sat Dec 29 19:40:02 2018

@author: lione
"""

import urllib.request
import time

Namelist = ["AWK","A","DHR","ECL","FLS","PNR","SEV.PAR","SVT.LON","UUGRY","WEIR.LON","AGCO","CNI","CF","BRFS3.SAO","K","MOS","MDLZ","DSM.AMS","DE","6326.TOK","9020.TOK","SGO.PAR","SU","ABBN.VSX","AA9.BER","CSX","LR.PAR","NSC","PHIA.AMS","SIE.FRK"]



api_key= "FIF2L2BXUMM7RYWV"

'''
USD	JPY	GBP	EUR	CHF	BRL
17	2	2	7	1	1
'''



for nameStock in Namelist:
    url = 'https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol={stock}&apikey={key}&datatype=csv&outputsize=full'.format(stock=nameStock, key=api_key)
    urllib.request.urlretrieve(url, "./Stocks/{}.csv".format(nameStock))
    print(nameStock+", done.")
    time.sleep(21)


ExchangesRateList = ["USD", "JPY", "GBP", "CHF", "BRL"]

for nameExchangeRate in ExchangesRateList:
    url = 'https://www.alphavantage.co/query?function=FX_DAILY&from_symbol={exchangeRate}&to_symbol=EUR&apikey={key}&outputsize=full&datatype=csv'.format(exchangeRate=nameExchangeRate, key=api_key)
    urllib.request.urlretrieve(url, "./ExchangeRates/{}.csv".format(nameExchangeRate))
    print(nameExchangeRate+", done.")
    time.sleep(21)

