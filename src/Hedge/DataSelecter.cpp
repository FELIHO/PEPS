#include "pch.h"
#include "DataSelecter.hpp"
#include <math.h>

using namespace std;
using namespace Computations;

DataSelecter::DataSelecter(PnlVectInt *ConstationDate) {
  ConstationDate_ = ConstationDate;
}

DataSelecter::DataSelecter(const DataSelecter &D) {
  ConstationDate_ = pnl_vect_int_copy(D.ConstationDate_);
}


DataSelecter::~DataSelecter(){
  pnl_vect_int_free(&ConstationDate_);
}

PnlMat * DataSelecter::getPast(const PnlMat *allData, const PnlVectInt *dateIndexes, const int Date) {
  PnlVectInt *rebalancementDateIndexestoDate = getRebalecementDateIndexestoDate(dateIndexes, Date);
  int indexDate = getIndexDate(dateIndexes);

  if ((rebalancementDateIndexestoDate->size < 3) && (indexDate < 0)){
    pnl_vect_int_free(&rebalancementDateIndexestoDate);
    return pnl_mat_new();
  }
  else if ((rebalancementDateIndexestoDate->size < 3) && (indexDate >= 0)) {
    PnlMat *resultPast = pnl_mat_create(1, allData->n);
    PnlVect *dataFeed = pnl_vect_new();
    pnl_mat_get_row(dataFeed, allData, indexDate);
    pnl_mat_set_row(resultPast, dataFeed, 0);
    pnl_vect_int_free(&rebalancementDateIndexestoDate);
    pnl_vect_free(&dataFeed);
    return resultPast;
  }


  PnlMat *resultPast = pnl_mat_create(rebalancementDateIndexestoDate->size + 1, allData->n);
  PnlVect *dataFeed = pnl_vect_new();
  PnlVect *nextdataFeed = pnl_vect_new();

  pnl_mat_get_row(dataFeed, allData, pnl_vect_int_get(rebalancementDateIndexestoDate, 0));
  pnl_mat_get_row(nextdataFeed, allData, pnl_vect_int_get(rebalancementDateIndexestoDate, 1));
  pnl_vect_plus_vect(dataFeed, nextDataFeed);

  pnl_mat_get_row(nextdataFeed, allData, pnl_vect_int_get(rebalancementDateIndexestoDate, 2));
  pnl_vect_plus_vect(dataFeed, nextDataFeed);
  pnl_mat_set_row(resultPast, dataFeed, 0);
  pnl_vect_free(&nextdataFeed);

	for (int i = 1; i < rebalancementDateIndexestoDate->size - 3; i++){
    pnl_mat_get_row(dataFeed, allData, pnl_vect_int_get(rebalancementDateIndexestoDate, i + 2));
    pnl_mat_set_row(resultPast, dataFeed, i);
  }
  pnl_mat_get_row(dataFeed, allData, indexDate);
  pnl_mat_set_row(resultPast, dataFeed, rebalancementDateIndexestoDate->size);

  pnl_vect_int_free(&rebalancementDateIndexestoDate);
  pnl_vect_free(&dataFeed);
  return resultPast;

}


int DataSelecter::getIndexDate(const PnlVectInt *dateIndexes, const int Date) {
	for (int i = 0; i < dateIndexes->size; i++){
		if (pnl_vect_int_get(dateIndexes, i) == Date){
			return i;
		}
	}
  return -1;
}

PnlVectInt* DataSelecter::getRebalecementDateIndexes(const PnlVectInt *dateIndexes) {
  PnlVectInt *rebalancementDateIndexes = pnl_vect_int_create_from_scalar(ConstationDate_->size,0);
  int counter = 0;
  for (int i = 0; i < dateIndexes->size; i++){
    if (pnl_vect_int_get(dateIndexes, i) == ConstationDate_[counter]){
      pnl_vect_int_set(rebalancementDateIndexes, counter, i);
      counter++;
    }
  }
  pnl_vect_int_resize(rebalancementDateIndexes, counter);
  return rebalancementDateIndexes;
}

PnlVectInt* DataSelecter::getRebalecementDateIndexestoDate(const PnlVectInt *dateIndexes, const int Date) {
  PnlVectInt *rebalancementDateIndexestoDate = getRebalecementDateIndexes(dateIndexes);
  int counter = 0;
  for (int i = 0; i < rebalancementDateIndexes->size; i++){
    if (pnl_vect_get(rebalancementDateIndexes, i) > Date){
    break;
    }
    counter++;
  }
  pnl_vect_int_resize(rebalancementDateIndexes, counter);
  return rebalancementDateIndexestoDate;
}