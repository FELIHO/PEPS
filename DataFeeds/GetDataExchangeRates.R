directory <- "C:/Users/lione/Desktop/Ensimag/PEPS/PepsBeta/p.e.p.s/DataFeeds/ExchangeRates/"

file <- paste(directory,"USD", sep= "")
file <- paste(file,".csv", sep= "")
MyData <- read.csv(file, header=TRUE, sep=",")
MyData <- MyData[,c(1,5)]
colnames(MyData)[colnames(MyData)=="close"] <- "USD"

NameList <- c("JPY", "GBP", "CHF", "BRL")
for (name in NameList) {
  file <- paste(directory,name, sep= "")
  file <- paste(file,".csv", sep= "")
  dataCurrent <- read.csv(file, header=TRUE, sep=",")
  dataCurrent <- dataCurrent[,c(1,5)]
  colnames(dataCurrent)[colnames(dataCurrent)=="close"] <- name
  MyData <- merge(MyData, dataCurrent, by="timestamp", all=TRUE)
}

rm(dataCurrent)


#################################
######### MAKE A GOOD DATA ######
#################################
#Reorder Data
MyData$timestamp <- (as.Date(MyData$timestamp))
MyData <-MyData[order(MyData[,1],decreasing=F),]
rownames(MyData) <- seq( 1,nrow(MyData) ,by=1)

for ( i in seq(nrow(MyData), 1, by=-1)){
  counter = 0
  for(j in MyData[i, ]){
    if (is.na(j)) {
      counter = counter +1
    }
  }
  if (counter > 0) {
    MyData <- MyData[-i, ]
  }
  else {
    break
  }
  
}

########################################
#### FILL DATA WHEN DATA IS MISSING ####
########################################

dataFeedYoung = c(MyData[nrow(MyData),])
for ( i in seq(nrow(MyData)-1, 1, by=-1)){
  dataFeedOld = as.vector(c(MyData[i,]))
  for(j in seq(1, ncol(MyData), by=1)){
    if (is.na(dataFeedOld[j])) {
      if (is.na(dataFeedYoung[j])){
        k = as.numeric(i)
        print(k)
        while (is.na(as.numeric(MyData[k,j]))) {
          k = k +1
        }
        dataFeedYoung[j] = MyData[k,j]
      }
      MyData[i,j] = as.numeric(dataFeedYoung[j]) +0.0
    }
  }
  dataFeedYoung = as.vector(c(dataFeedOld))
}

MyData <- MyData[which(as.Date(MyData$timestamp) > as.Date.character("2009-12-31")),]

#prix_dates_constation <-  x_remplie[ which(x$timestamp %in% c(as.Date.character("2014-04-11"),as.Date.character("2014-04-14"),as.Date.character("2014-04-15"),as.Date.character("2014-10-13"),as.Date.character("2015-04-13"),as.Date.character("2015-10-12"),as.Date.character("2016-04-11"),as.Date.character("2016-10-11"),as.Date.character("2017-04-11"),as.Date.character("2017-10-11"),as.Date.character("2018-04-11"),as.Date.character("2018-10-11"))), ]


write.table(MyData[,-1], file = "C:/Users/lione/Desktop/Ensimag/PEPS/PepsBeta/p.e.p.s/DataFeeds/kozei_dataExchangeRates.dat", sep = " ",row.names = FALSE,col.names = FALSE)
write.table(as.numeric(format(MyData[,1],"%Y%m%d")), file = "C:/Users/lione/Desktop/Ensimag/PEPS/PepsBeta/p.e.p.s/DataFeeds/kozei_IndexdataExchangeRates.dat", sep = " ",row.names = FALSE,col.names = FALSE)
