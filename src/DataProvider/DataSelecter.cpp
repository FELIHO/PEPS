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

PnlMat* DataSelecter::getData(double T, int H){
  float daysPerStep = (T*260)/H; // nbr de jours entre deux rebalancement
	PnlMat* marketData = pnl_mat_create(H+1,dailyData_->n);
  PnlVect* V = pnl_vect_new();
  for(int i=0; i<H+1; i++){
    pnl_mat_get_row(V,dailyData_,i*daysPerStep);
    pnl_mat_set_row(marketData, V ,i);
  }
  pnl_vect_free(&V);
  return marketData;
}

PnlMat* DataSelecter::getData(double T, int nbTimeSteps, int nbRebalancementPerStep){
  int H = nbRebalancementPerStep*nbTimeSteps;
  return getData(T,H);
}

PnlMat* DataSelecter::getEstimationWindow(double t, int nbDays){
  int currentIndex = t*260 + firstDateIndex_;
  PnlMat* estimationWindow = pnl_mat_create(nbDays,dailyData_->n);
  PnlVect* V = pnl_vect_new();
  for(int i=0; i<nbDays; i++){
    pnl_mat_get_row(V,dailyData_,currentIndex-i);
    pnl_mat_set_row(estimationWindow, V ,i);
  }
  pnl_vect_free(&V);
  return estimationWindow;
}


// DataSelecter::DataSelecter() {
//   ConstationDate_ = pnl_vect_int_new();
// }

// 




// PnlMat * DataSelecter::getPast(const PnlMat *allData, const PnlVectInt *dateIndexes, const int Date) {
//   PnlVectInt *rebalancementDateIndexestoDate = getRebalecementDateIndexestoDate(dateIndexes, Date);
//   int indexDate = getIndexDate(dateIndexes, Date);

//   if ((rebalancementDateIndexestoDate->size < 3) && (indexDate < 0)){
//     pnl_vect_int_free(&rebalancementDateIndexestoDate);
//     return pnl_mat_new();
//   }
//   else if ((rebalancementDateIndexestoDate->size < 3) && (indexDate >= 0)) {
//     PnlMat *resultPast = pnl_mat_create(1, allData->n);
//     PnlVect *dataFeed = pnl_vect_new();
//     pnl_mat_get_row(dataFeed, allData, indexDate);
//     pnl_mat_set_row(resultPast, dataFeed, 0);
//     pnl_vect_int_free(&rebalancementDateIndexestoDate);
//     pnl_vect_free(&dataFeed);
//     return resultPast;
//   }


//   PnlMat *resultPast = pnl_mat_create(rebalancementDateIndexestoDate->size - 2, allData->n);
//   PnlVect *dataFeed = pnl_vect_new();
//   PnlVect *nextdataFeed = pnl_vect_new();

//   pnl_mat_get_row(dataFeed, allData, pnl_vect_int_get(rebalancementDateIndexestoDate, 0));
//   pnl_mat_get_row(nextdataFeed, allData, pnl_vect_int_get(rebalancementDateIndexestoDate, 1));
//   pnl_vect_plus_vect(dataFeed, nextdataFeed);

//   pnl_mat_get_row(nextdataFeed, allData, pnl_vect_int_get(rebalancementDateIndexestoDate, 2));
//   pnl_vect_plus_vect(dataFeed, nextdataFeed);
//   pnl_vect_div_scalar(dataFeed, 3);
//   pnl_mat_set_row(resultPast, dataFeed, 0);
//   pnl_vect_free(&nextdataFeed);

// 	for (int i = 1; i < rebalancementDateIndexestoDate->size - 3; i++){
//     pnl_mat_get_row(dataFeed, allData, pnl_vect_int_get(rebalancementDateIndexestoDate, i + 2));
//     pnl_mat_set_row(resultPast, dataFeed, i);
//   }
//   pnl_mat_get_row(dataFeed, allData, indexDate);
//   pnl_mat_set_row(resultPast, dataFeed, resultPast->m - 1);

//   pnl_vect_int_free(&rebalancementDateIndexestoDate);
//   pnl_vect_free(&dataFeed);
//   return resultPast;

// }

// PnlMat * DataSelecter::getApproximatePath(const PnlMat *FullData, int nbTimeSteps) {
//   int timeSteps = FullData->n / nbTimeSteps;
//   PnlMat *resultPath = pnl_mat_create(nbTimeSteps + 1, FullData->n);
//   PnlVect *dataFeed = pnl_vect_new();
//   pnl_mat_get_row(dataFeed, FullData, 0);
//   pnl_mat_set_row(resultPath, dataFeed, 0);
//   for (int i = 1; i < nbTimeSteps - 1; i++){
//   pnl_mat_get_row(dataFeed, FullData, timeSteps * i);
//   pnl_mat_set_row(resultPath, dataFeed, i);
//     }
//   pnl_mat_get_row(dataFeed, FullData, FullData->m - 1);
//   pnl_mat_set_row(resultPath, dataFeed, resultPath->m - 1);
//   pnl_vect_free(&dataFeed);
//   return resultPath;
// }


// int DataSelecter::getIndexDate(const PnlVectInt *dateIndexes, const int Date) {
// 	for (int i = 0; i < dateIndexes->size; i++){
// 		if (pnl_vect_int_get(dateIndexes, i) == Date){
// 			return i;
// 		}
// 	}
//   return -1;
// }

// int DataSelecter::getIndexSpotDate(const PnlVectInt *dateIndexes) {
//   return getIndexDate(dateIndexes, pnl_vect_int_get(ConstationDate_, 0));
// }

// PnlVectInt* DataSelecter::getRebalecementDateIndexes(const PnlVectInt *dateIndexes) {
//   PnlVectInt *rebalancementDateIndexes = pnl_vect_int_create_from_scalar(ConstationDate_->size,0);
//   int counter = 0;
//   for (int i = 0; i < dateIndexes->size; i++){
//     if (pnl_vect_int_get(dateIndexes, i) == pnl_vect_int_get(ConstationDate_, counter)){
//       pnl_vect_int_set(rebalancementDateIndexes, counter, i);
//       counter++;
//     }
//   }
//   pnl_vect_int_resize(rebalancementDateIndexes, counter);
//   return rebalancementDateIndexes;
// }

// PnlVectInt* DataSelecter::getRebalecementDateIndexestoDate(const PnlVectInt *dateIndexes, const int Date) {
//   PnlVectInt *rebalancementDateIndexestoDate = getRebalecementDateIndexes(dateIndexes);
//   int counter = 0;
//   for (int i = 0; i < rebalancementDateIndexestoDate->size; i++){
//     if (pnl_vect_int_get(rebalancementDateIndexestoDate, i) > Date){
//     break;
//     }
//     counter++;
//   }
//   pnl_vect_int_resize(rebalancementDateIndexestoDate, counter);
//   return rebalancementDateIndexestoDate;
// }

// PnlVect* DataSelecter::getSpotFromData(const PnlMat *allData, const PnlVectInt *dateIndexes, const int Date){
//   int index = getIndexDate(dateIndexes, Date);
//   PnlVect* spot = pnl_vect_new();
//   pnl_mat_get_row(spot, allData, index);
//   return spot;
// }

// PnlMat DataSelecter::getWindowForEstimation(const PnlMat *allData, const PnlVectInt *dateIndexes, const int DateBegin, const int timeEstimation){
//     int index = getIndexDate(dateIndexes, DateBegin);
//     PnlMat windowEstimation = pnl_mat_wrap_mat_rows(allData, index, index + timeEstimation);
//     return windowEstimation;
// }

// PnlMat DataSelecter::getWindowPreviousEstimation(const PnlMat *allData, const PnlVectInt *dateIndexes, const int DateEnd, const int timeEstimation){
//     int index = getIndexDate(dateIndexes, DateEnd);
//     PnlMat windowEstimation = pnl_mat_wrap_mat_rows(allData, index - timeEstimation, index );
//     return windowEstimation;
// }

// double DataSelecter::computeTvalue(const PnlVectInt *dateIndexes, const int DateZero, const int DateT, const double T) {
//   int indexZero = getIndexDate(dateIndexes, DateZero);
//   int indexT = getIndexDate(dateIndexes, DateT);
//   return (indexT - indexZero + 0.0)/(T * 252);
// }