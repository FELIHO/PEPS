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



