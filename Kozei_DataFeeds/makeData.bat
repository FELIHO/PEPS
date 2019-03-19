python C:\Users\lione\Desktop\Ensimag\PEPS\NEWPEPS\DataFeeds\GetDataFeed.py %*
"C:\Program Files\R\R-3.4.4\bin\x64\Rscript.exe" GetDataFeed.R
"C:\Program Files\R\R-3.4.4\bin\x64\Rscript.exe" GetDataExchangeRates.R
cd C:\Users\lione\Desktop\appliweb\notes-app-api
serverless invoke local --function createAllData --path C:/Users/lione/Desktop/Ensimag/PEPS/NEWPEPS/DataFeeds/kozei_dataFeed.json
serverless invoke local --function createAllExchangeRate --path C:/Users/lione/Desktop/Ensimag/PEPS/NEWPEPS/DataFeeds/kozei_dataExchangeRates.json
pause