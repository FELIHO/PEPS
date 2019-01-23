#include "DataProvider.hpp"
#include "BlackScholesModel.hpp"
using namespace Computations;



DataProvider::DataProvider()
{
  DataFeed_ = pnl_mat_new();
  indexFirstSpot_ = 0;
}

DataProvider::~DataProvider(){
	pnl_mat_free(&DataFeed_);
}

DataProvider::DataProvider(const DataProvider& D) {
  DataFeed_ = pnl_mat_copy(D.DataFeed_);
  indexFirstSpot_ = D.indexFirstSpot_;
}
DataProvider& DataProvider::operator=(const DataProvider &D) {
  DataFeed_ = D.DataFeed_;
  indexFirstSpot_ = D.indexFirstSpot_;
}
