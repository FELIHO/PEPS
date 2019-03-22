spot <-seq(0,200,by=4)
results_BS_vector<-c()
results_diff_vector<-c()
results_pricer_vector<-c()

for(i in results_BS){
  results_BS_vector<-append(results_BS_vector,i)
}
for(i in results_pricer){
  results_pricer_vector<-append(results_pricer_vector,i)
}
for(i in results_diff){
  results_diff_vector<-append(results_diff_vector,i)
}
plot(x=spot,y=results_BS_vector,type="l")
lines(spot,results_pricer_vector,col="red")

plot(spot,results_diff_vector,type="l")