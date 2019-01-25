#include "SemiHistoricalDataProvider.hpp"



SemiHistoricalDataProvider::SemiHistoricalDataProvider() {
  mod_ = new BlackScholesModel();
  rng_ = new FakeRnd(0.1);
  fileData_ = new char();
}


SemiHistoricalDataProvider::SemiHistoricalDataProvider(const SemiHistoricalDataProvider &SHDP) {
  mod_ = new BlackScholesModel(*SHDP.mod_);
  rng_ = SHDP.rng_->clone();
  fileData_ = new char(*SHDP.fileData_);
}

SemiHistoricalDataProvider& SemiHistoricalDataProvider::operator= (const SemiHistoricalDataProvider &SHDP) {
  mod_ = SHDP.mod_;
  rng_ = SHDP.rng_;
  fileData_ = SHDP.fileData_;
  return *this;
}

SemiHistoricalDataProvider::~SemiHistoricalDataProvider(){
	delete(mod_);
	delete(rng_);
}

SemiHistoricalDataProvider::SemiHistoricalDataProvider(RandomGen *rng, char const *fileData){
	rng_ = rng->clone();
	fileData_ = fileData;
	PnlMat* Data = pnl_mat_create_from_file(fileData_);
	// ParameterEstimation pe ;
	// PnlVect* sigma = pe.getVolatilitiesVector(Data);
	// PnlMat* rho = pe.getCorrelationMatrix(Data);
	// PnlVect* spot = pnl_vect_create_from_double(Data->n, 100);
	// PnlVect* trend = pe.getTrend(Data);
	// mod_ =  new BlackScholesModel(spot->size, pnl_vect_sum(trend)/(trend->size) , rho, sigma, spot, trend);
	// pnl_mat_free(&rho);
	// pnl_vect_free(&sigma);
	// pnl_vect_free(&spot);

	PnlVect *spot = pnl_vect_create_from_scalar(Data->n, 100);
  	PnlVect *sigma = pnl_vect_create_from_scalar(Data->n, 0.4);

	mod_ = new BlackScholesModel(Data->n, 0.03, 0.1, sigma, spot);

	pnl_mat_free(&Data);

}

PnlMat* SemiHistoricalDataProvider::getMarketData(double T, int nbTimeSteps, int nbRebalancementPerStep){
	PnlMat* Data = pnl_mat_create_from_file(fileData_);
	int H = nbRebalancementPerStep*nbTimeSteps;
	int nbDaysPerTimeStep = (T/H)*260.0;
	int nbAvailableHistData = ( Data->m / nbDaysPerTimeStep ) + 1 ;
	PnlMat* HistData = pnl_mat_create( nbAvailableHistData - 1 , Data->n);
	PnlMat* SimData = pnl_mat_create( H - nbAvailableHistData + 2 , Data->n);
	PnlMat* marketData = pnl_mat_create(H+1,Data->n);

	for (int i =0; i < nbAvailableHistData ; i++) {
		PnlVect V = pnl_vect_wrap_mat_row(Data, i*nbDaysPerTimeStep);
		if( i != nbAvailableHistData - 1){
			pnl_mat_set_row(HistData, &V, i);
		}
		else {
			pnl_mat_set_row(SimData, &V, 0);
		}
	}
	mod_->simulateAsset(SimData, T/H, H - nbAvailableHistData +1 , rng_, mod_->trend_);

	mod_->concatenationMatrice( marketData , HistData, SimData );
	pnl_mat_free(&Data);
	pnl_mat_free(&HistData);
	pnl_mat_free(&SimData);

	return marketData;
}



PnlMat* SemiHistoricalDataProvider::getDailyMarketData(double T){
	PnlMat* HistData = pnl_mat_create_from_file(fileData_);
	int H = T*260;

	PnlMat* SimData = pnl_mat_create( H - HistData->m + 2 , HistData->n);

	PnlVect V = pnl_vect_wrap_mat_row(HistData, HistData->m -1);
	pnl_mat_set_row(SimData, &V, 0);

	mod_->simulateAsset(SimData, T/H, H - HistData->m +1 , rng_, mod_->trend_);

	PnlMat* marketData = pnl_mat_create(H+1,HistData->n);

	PnlMat* subHistData = pnl_mat_new();
    pnl_mat_extract_subblock(subHistData, HistData, 0, HistData->m-1, 0, HistData->n);

	mod_->concatenationMatrice( marketData , subHistData, SimData );

	pnl_mat_free(&subHistData);
	pnl_mat_free(&HistData);
	pnl_mat_free(&SimData);

	return marketData;
}



PnlMat* SemiHistoricalDataProvider::getWeeklyMarketData(double T, int nbTimeSteps){
	int nbRebalancementPerStep = (T*52)/nbTimeSteps;
	return getMarketData(T, nbTimeSteps, nbRebalancementPerStep);
}

PnlMat* SemiHistoricalDataProvider::getMonthlyMarketData(double T, int nbTimeSteps){
	int nbRebalancementPerStep = (T*12)/nbTimeSteps;
	return getMarketData(T, nbTimeSteps, nbRebalancementPerStep);
}
