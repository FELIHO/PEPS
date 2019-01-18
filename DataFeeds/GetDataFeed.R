Namelist = c("A","DHR","ECL","FLS","PNR","SEV.PAR","SVT.LON","UUGRY","WEIR.LON","AGCO","CNI","CF","BRFS3.SAO","K","MOS","MDLZ","DSM.AMS","DE","6326.TOK","9020.TOK","SGO.PAR","SU","ABBN.VSX","AA9.BER","CSX","LR.PAR","NSC","PHIA.AMS","SIE.FRK")


directory = "C:/Users/Mouhcine/Desktop/ENSIMAG/IF 3A/Peps Projet/p.e.p.s/DataFeeds/Daily/Csv/";
getwd()
file <- paste (directory, "AWK", sep = "");
file <- paste (file, ".csv", sep = "");
x <- read.table(file,header=TRUE,sep=",");
x <- x[,c(1, 5)]

for (name in Namelist){
  file <- paste (directory, name, sep = "");
  file <- paste (file, ".csv", sep = "");
  x_current <- read.table(file,header=TRUE,sep=",");
  x_current <- x_current[,c(1, 5)]
  x <- merge(x, x_current, by="timestamp", all=TRUE)
}
x$timestamp<-as.Date.character(x$timestamp)
x<-x[order(x$timestamp),]
x<- x[which(x$timestamp > as.Date.character("2014-04-10") ),]

#création d'une matrice sans NA
x_remplie <-x

for(i in 1:nrow(x)){
  for(j in 2:ncol(x)){
    if(is.na(x_remplie[i,j] )){
      compt<-0
      while( is.na(x_remplie[i+compt,j])){
        compt<-compt+1
        
      }
      x_remplie[i,j] <-x_remplie[i+compt,j]
    }
  } 
} 

prix_dates_constation <-  x_remplie[ which(x$timestamp %in% c(as.Date.character("2014-04-11"),as.Date.character("2014-04-14"),as.Date.character("2014-04-15"),as.Date.character("2014-10-13"),as.Date.character("2015-04-13"),as.Date.character("2015-10-12"),as.Date.character("2016-04-11"),as.Date.character("2016-10-11"),as.Date.character("2017-04-11"),as.Date.character("2017-10-11"),as.Date.character("2018-04-11"),as.Date.character("2018-10-11"))), ]


write.table(prix_dates_constation[,-1], file = "C:/Users/Mouhcine/Desktop/ENSIMAG/PEPES Pr/PEPES Pr/matrice.txt", sep = " ",row.names = FALSE,col.names = FALSE)
