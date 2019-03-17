directory <- "C:/Users/lione/Desktop/Ensimag/PEPS/NEWPEPS/DataFeeds/Daily/Csv/"

file <- paste(directory,"AWK", sep= "")
file <- paste(file,".csv", sep= "")
MyData <- read.csv(file, header=TRUE, sep=",")
MyData <- MyData[,c(1,5)]
colnames(MyData)[colnames(MyData)=="close"] <- "AWK"

NameList <- c("A","DHR","ECL","FLS","PNR","UUGRY","AGCO","CNI","CF","K","MOS","MDLZ","DE","SU","CSX","NSC","6326.TOK","9020.TOK","SVT.LON","WEIR.LON","SEV.PAR","DSM.AMS","SGO.PAR","AA9.BER","LR.PAR","PHIA.AMS","SIE.FRK","ABBN.VSX","BRFS3.SAO")
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
MyData$timestamp <- as.numeric(as.POSIXct(MyData$timestamp, format="%Y-%m-%d"))
#MyData <- MyData[which(as.Date(MyData$timestamp) > as.Date.character("2009-12-31")),]
MyData <-MyData[order(MyData[,1],decreasing=F),]
rownames(MyData) <- seq( 1,nrow(MyData) ,by=1)

for ( i in seq(nrow(MyData), 1, by=-1)){
  counter = 0
  for(j in MyData[i, ]){
    if (is.na(j) | (as.numeric(j) == 0.00)) {
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
      if ((is.na(dataFeedYoung[j])) | (as.numeric(dataFeedYoung[j])) == 0.000){
        k = as.numeric(i)
        print(k)
        while ((is.na(as.numeric(MyData[k,j]))) | (as.numeric(MyData[k,j]) == 0.000) ) {
          k = k +1
        }
        dataFeedYoung[j] = MyData[k,j]
      }
      MyData[i,j] = as.numeric(dataFeedYoung[j]) +0.0
    }
  }
  dataFeedYoung = as.vector(c(dataFeedOld))
}

for (j in seq(2, 31, by=1)){
  for ( i in seq(nrow(MyData)- 1, 1, by=-1)){
    data = MyData[i,j]
    if (as.numeric(data) == 0) {
      print(i)
      MyData[i,j] = MyData[i + 1,j]
    }
  }
}


write.table(MyData[,-1], file = "C:/Users/lione/Desktop/Ensimag/PEPS/NEWPEPS/DataFeeds/kozei_dataFeed.dat", sep = " ",row.names = FALSE,col.names = FALSE)
write.table(as.numeric(format(as.Date(as.POSIXct(MyData[,1], origin="1970-01-01")),"%Y%m%d")), file = "C:/Users/lione/Desktop/Ensimag/PEPS/NEWPEPS/DataFeeds/kozei_IndexdataFeed.dat", sep = " ",row.names = FALSE,col.names = FALSE)

### Write the Data into a json
library(rjson)
write(toJSON(unname(split(MyData, 1:nrow(MyData))))[1][1], file = "C:/Users/lione/Desktop/Ensimag/PEPS/NEWPEPS/DataFeeds/kozei_dataFeed.json")

