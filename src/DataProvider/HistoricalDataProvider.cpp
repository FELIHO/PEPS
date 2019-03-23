#include "HistoricalDataProvider.hpp"
#include "DataSelecter.hpp"



HistoricalDataProvider::HistoricalDataProvider() {
  fileData_ = new char();
  firstDateIndex_ = 0;
}


HistoricalDataProvider::HistoricalDataProvider(const HistoricalDataProvider &HDP) {
  fileData_ = new char(*HDP.fileData_);
  firstDateIndex_ = HDP.firstDateIndex_;
}

HistoricalDataProvider& HistoricalDataProvider::operator= (const HistoricalDataProvider &HDP) {
  fileData_ = HDP.fileData_;
  firstDateIndex_ = HDP.firstDateIndex_;
  return *this;
}

HistoricalDataProvider::~HistoricalDataProvider(){
}

HistoricalDataProvider::HistoricalDataProvider(char const *fileData, int firstDateIndex){
	fileData_ = fileData;
	firstDateIndex_ = firstDateIndex;
}

PnlMat* HistoricalDataProvider::getMarketData(double T, int nbTimeSteps, int nbRebalancementPerStep){
	PnlMat* Data = pnl_mat_create_from_file(fileData_);
	DataSelecter* DS = new DataSelecter(Data,firstDateIndex_);
	PnlMat* marketData = DS->getData(T,nbTimeSteps*nbRebalancementPerStep);
	delete(DS);
	return marketData;
}



PnlMat* HistoricalDataProvider::getDailyMarketData(double T){
	PnlMat* Data = pnl_mat_create_from_file(fileData_);
	int H = T*Tools::NumberOfDaysPerYear;
	DataSelecter* DS = new DataSelecter(Data,firstDateIndex_);
	PnlMat* marketData = DS->getData(T,H);
	delete(DS);
	return marketData;
}



PnlMat* HistoricalDataProvider::getWeeklyMarketData(double T, int nbTimeSteps){
	int nbRebalancementPerStep = (T*Tools::NumberOfWeeksPerYear)/nbTimeSteps;
	return getMarketData(T, nbTimeSteps, nbRebalancementPerStep);
}

PnlMat* HistoricalDataProvider::getMonthlyMarketData(double T, int nbTimeSteps){
	int nbRebalancementPerStep = (T*12)/nbTimeSteps;
	return getMarketData(T, nbTimeSteps, nbRebalancementPerStep);
}