#include "DataSelecter.hpp"

DataSelecter::DataSelecter() {
  firstDateIndex_ = 0;
  dailyData_ = pnl_mat_new();
}

DataSelecter::DataSelecter(const DataSelecter &DS) {
  firstDateIndex_ = DS.firstDateIndex_;
  dailyData_ = pnl_mat_copy(DS.dailyData_);
}

DataSelecter& DataSelecter::operator= (const DataSelecter &DS) {
  firstDateIndex_ = DS.firstDateIndex_;
  dailyData_ = DS.dailyData_;
  return *this;
}

DataSelecter::~DataSelecter(){
  pnl_mat_free(&dailyData_);
}

DataSelecter::DataSelecter(PnlMat* dailyData) {
  firstDateIndex_ = 0;
  dailyData_ = pnl_mat_copy(dailyData);
}

DataSelecter::DataSelecter(PnlMat* dailyData, int firstDateIndex) {
  firstDateIndex_ = firstDateIndex;
  dailyData_ = pnl_mat_copy(dailyData);
}

PnlMat* DataSelecter::getPast(double t, double T, int nbTimeSteps){
  if (dailyData_->m - firstDateIndex_ < t*Tools::NumberOfDaysPerYear){
    throw invalid_argument("la marketData ne contient pas toutes les données jusqu'à la date t");
  }
  int nbRowsPast = ceil(t*nbTimeSteps/T)+1;
  PnlMat* past = pnl_mat_create(nbRowsPast,dailyData_->n);
  int daysPerStep = (T*Tools::NumberOfDaysPerYear)/nbTimeSteps; //nbr de jours entre deux dates de constation
  PnlVect* V = pnl_vect_new();
  for(int i=0; i<nbRowsPast-1; i++){
    pnl_mat_get_row(V,dailyData_, firstDateIndex_ + i*daysPerStep);
    pnl_mat_set_row(past,V,i);
  }
  pnl_mat_get_row(V,dailyData_, firstDateIndex_ + t*Tools::NumberOfDaysPerYear);
  pnl_mat_set_row(past,V,nbRowsPast-1);
  return past;
}

PnlMat* DataSelecter::getEstimationWindow(double t, int nbDays){
  int currentIndex = t*Tools::NumberOfDaysPerYear + firstDateIndex_;
  PnlMat* estimationWindow = pnl_mat_create(nbDays,dailyData_->n);
  PnlVect* V = pnl_vect_new();
  for(int i=0; i<nbDays; i++){
    pnl_mat_get_row(V,dailyData_,currentIndex-i);
    pnl_mat_set_row(estimationWindow, V ,nbDays-1-i);
  }
  pnl_vect_free(&V);
  return estimationWindow;
}

PnlMat* DataSelecter::getHistoricalEstimationWindow(){
  PnlMat* estimationWindow = pnl_mat_create(firstDateIndex_,dailyData_->n);
  pnl_mat_extract_subblock (estimationWindow, dailyData_, 0 , firstDateIndex_ , 0, dailyData_->n);
  return estimationWindow;
}

PnlMat* DataSelecter::getData(double T, int H){
  float daysPerStep = (T*Tools::NumberOfDaysPerYear)/H; // nbr de jours entre deux rebalancement
	PnlMat* marketData = pnl_mat_create(H+1,dailyData_->n);
  PnlVect* V = pnl_vect_new();
  for(int i=0; i<H+1; i++){
    pnl_mat_get_row(V,dailyData_,firstDateIndex_ + i*daysPerStep);
    pnl_mat_set_row(marketData, V ,i);
  }
  pnl_vect_free(&V);
  return marketData;
}

PnlMat* DataSelecter::getData(double T, int nbTimeSteps, int nbRebalancementPerStep){
  int H = nbRebalancementPerStep*nbTimeSteps;
  return getData(T,H);
}