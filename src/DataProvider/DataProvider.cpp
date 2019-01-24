#include "DataProvider.hpp"
#include "BlackScholesModel.hpp"
using namespace Computations;



DataProvider::DataProvider()
{
  DataFeed_ = pnl_mat_new();
  indexFirstSpot_ = 0;
  mc_ = new MonteCarlo();
}

DataProvider::~DataProvider(){
	pnl_mat_free(&DataFeed_);
  delete(mc_);
}

DataProvider::DataProvider(const DataProvider& D) {
  DataFeed_ = pnl_mat_copy(D.DataFeed_);
  indexFirstSpot_ = D.indexFirstSpot_;
  mc_ = new MonteCarlo(*D.mc_);
}
DataProvider& DataProvider::operator=(const DataProvider &D) {
  DataFeed_ = D.DataFeed_;
  indexFirstSpot_ = D.indexFirstSpot_;
  mc_ = D.mc_;
  return *this;
}
