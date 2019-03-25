#include "Computations.hpp"

#include "MonteCarlo.hpp"
#include "Kozei.hpp"
#include "ParameterEstimation.hpp"
#include "DataSelecter.hpp"
#include <list>
#include "ForwardTest.hpp"
#include "Kozei.hpp"
#include <vector>
#include <iostream>
#include <time.h>

void Computations::getPriceKozeiNative(double &ic, double &prix, int n_samples,
	double h, int date, double inv_init, int numberDateEstimation, double r)
{
	string str3;
	string str4;
	string str5;
	string str = "/home/jmari/ENSIMAG/3A/PEPS/p.e.p.s/Hist_kozei/";

	str3 = str + "kozei_dataFeed.dat";
	const char *cstr = str3.c_str();
	PnlMat *assetData = pnl_mat_create_from_file(cstr);

	str4 = str + "kozei_exchangeRates.dat";
	const char *cstr2 = str4.c_str();
	PnlMat *exchangeRates = pnl_mat_create_from_file(cstr2);
	
	str5 = str + "kozei_IndexdataFeed.dat";
	const char *cstr3 = str5.c_str();
	PnlVect *dateIndexes = pnl_vect_create_from_file(cstr3);

	Kozei *K_ = new Kozei(inv_init);

	PnlMat* kozeiPath = K_->path_matrix(assetData, exchangeRates);
	

	int firstDateIndex = Tools::find(dateIndexes,20140411);
	int currentDateIndex = Tools::find(dateIndexes,date, firstDateIndex);
	double t = (currentDateIndex - firstDateIndex)/((double) Tools::NumberOfDaysPerYear);

	K_->SetNivauxInitiaux(kozeiPath,firstDateIndex);

	DataSelecter* ds = new DataSelecter(kozeiPath, firstDateIndex);


	PnlMat *past = ds->getPast(t, K_->T_, K_->nbTimeSteps_);

	PnlVect* spot = pnl_vect_new();
	pnl_mat_get_row(spot, past, 0);

	ParameterEstimation pe;
	PnlMat* window = ds->getEstimationWindow(t, numberDateEstimation);
	PnlMat* corr = pe.getCorrelationMatrix(window);
	PnlVect* sigma = pe.getVolatilitiesVector(window);
	BlackScholesModel* bs = new BlackScholesModel(past->n, r, corr, sigma, spot);

	MonteCarlo *mc = new MonteCarlo(bs, K_, h, n_samples);


	mc->price(past, t, prix, ic);

	pnl_mat_free(&assetData);
	pnl_mat_free(&exchangeRates);
	pnl_vect_free(&dateIndexes);
	pnl_mat_free(&kozeiPath);
	delete(ds);
	delete(K_);
	pnl_mat_free(&past);
	pnl_vect_free(&spot);
	pnl_mat_free(&window);
	pnl_mat_free(&corr);
	pnl_vect_free(&sigma);
	delete(bs);
	delete(mc);
}


string Computations::getDeltaKozeiNative(int n_samples,
	double h, int date, double inv_init, int numberDateEstimation, double r)
{

	string str3;
	string str4;
	string str5;
	string str = "/home/jmari/ENSIMAG/3A/PEPS/p.e.p.s/Hist_kozei/";


	str3 = str + "kozei_dataFeed.dat";
	const char *cstr = str3.c_str();
	PnlMat *assetData = pnl_mat_create_from_file(cstr);

	str4 = str + "kozei_exchangeRates.dat";
	const char *cstr2 = str4.c_str();
	PnlMat *exchangeRates = pnl_mat_create_from_file(cstr2);
	
	str5 = str + "kozei_IndexdataFeed.dat";
	const char *cstr3 = str5.c_str();
	PnlVect *dateIndexes = pnl_vect_create_from_file(cstr3);

	Kozei *K_ = new Kozei(inv_init);

	PnlMat* kozeiPath = K_->path_matrix(assetData, exchangeRates);
	

	int firstDateIndex = Tools::find(dateIndexes,20140411);
	int currentDateIndex = Tools::find(dateIndexes,date, firstDateIndex);
	double t = (currentDateIndex - firstDateIndex)/((double) Tools::NumberOfDaysPerYear);

	K_->SetNivauxInitiaux(kozeiPath,firstDateIndex);

	DataSelecter* ds = new DataSelecter(kozeiPath, firstDateIndex);


	PnlMat *past = ds->getPast(t, K_->T_, K_->nbTimeSteps_);

	PnlVect* spot = pnl_vect_new();
	pnl_mat_get_row(spot, past, 0);

	ParameterEstimation pe;
	PnlMat* window = ds->getEstimationWindow(t, numberDateEstimation);
	PnlMat* corr = pe.getCorrelationMatrix(window);
	PnlVect* sigma = pe.getVolatilitiesVector(window);
	BlackScholesModel* bs = new BlackScholesModel(past->n, r, corr, sigma, spot);

	MonteCarlo *mc = new MonteCarlo(bs, K_, h, n_samples);

	PnlVect* Assets_ZC_prices = pnl_vect_new();
	pnl_mat_get_row(Assets_ZC_prices,kozeiPath,currentDateIndex);


	PnlVect *deltaV = pnl_vect_create_from_scalar(past->n, 0.0);
	mc->delta(past, t, deltaV);

	pnl_mat_free(&assetData);
	pnl_mat_free(&exchangeRates);
	pnl_vect_free(&dateIndexes);
	pnl_mat_free(&kozeiPath);
	delete(ds);
	delete(K_);
	pnl_vect_free(&spot);
	pnl_mat_free(&window);
	pnl_mat_free(&corr);
	pnl_vect_free(&sigma);
	delete(bs);
	delete(mc);
	
	

	list<string> Names;
	Names.push_back("AWK");
	Names.push_back("A");
	Names.push_back("DHR");
	Names.push_back("ECL");
	Names.push_back("FLS");
	Names.push_back("PNR");
	Names.push_back("UUGRY");
	Names.push_back("AGCO");
	Names.push_back("CNI");
	Names.push_back("CF");
	Names.push_back("K");
	Names.push_back("MOS");
	Names.push_back("MDLZ");
	Names.push_back("DE");
	Names.push_back("SU");
	Names.push_back("CSX");
	Names.push_back("NSC");
	Names.push_back("6326.TOK");
	Names.push_back("9020.TOK");
	Names.push_back("SVT.LON");
	Names.push_back("WEIR.LON");
	Names.push_back("SEV.PAR");
	Names.push_back("DSM.AMS");
	Names.push_back("SGO.PAR");
	Names.push_back("AA9.BER");
	Names.push_back("LR.PAR");
	Names.push_back("PHIA.AMS");
	Names.push_back("SIE.FRK");
	Names.push_back("ABBN.VSX");
	Names.push_back("BRFS3.SAO");
	//USD JPY GBP CHF BRL
	Names.push_back("USD");
	Names.push_back("JPY");
	Names.push_back("GBP");
	Names.push_back("CHF");
	Names.push_back("BRL");

	std::list<std::string>::iterator iter;
	std::string findString = "AWK";
	string strDelta = "[";
	string temp;

	if ((iter = std::find(Names.begin(), Names.end(), findString)) != Names.end())
	{
		for (int idx = 0; idx < past->n; idx++)
		{
			temp = "{\"name\":\"";
			temp += *iter;
			temp += "\", \"delta\":";
			temp += to_string(pnl_vect_get(deltaV, idx));
			temp += "}";
			if (idx < past->n - 1) {
				temp += ", ";
			}
			strDelta += temp;
			++iter;
		}

	}

	strDelta += "]";

	pnl_mat_free(&past);
	pnl_vect_free(&deltaV);
	return strDelta;
}


string Computations::getListPriceKozeiNative(int n_samples,
	double h, double inv_init, int numberDateEstimation, double r)
{

	string str3;
	string str4;
	string str5;
	string str = "/home/jmari/ENSIMAG/3A/PEPS/p.e.p.s/Hist_kozei/";


	str3 = str + "kozei_dataFeed.dat";
	const char *cstr = str3.c_str();
	PnlMat *assetData = pnl_mat_create_from_file(cstr);

	str4 = str + "kozei_exchangeRates.dat";
	const char *cstr2 = str4.c_str();
	PnlMat *exchangeRates = pnl_mat_create_from_file(cstr2);
	
	str5 = str + "kozei_IndexdataFeed.dat";
	const char *cstr3 = str5.c_str();
	PnlVect *dateIndexes = pnl_vect_create_from_file(cstr3);

	Kozei *K = new Kozei(inv_init);

	PnlMat* kozeiPath = K->path_matrix(assetData, exchangeRates);

	int firstDateIndex = Tools::find(dateIndexes,20140411);
	
	K->SetNivauxInitiaux(kozeiPath,firstDateIndex);

	DataSelecter* ds = new DataSelecter(kozeiPath, firstDateIndex);

	PnlVect* spot = pnl_vect_new();
	pnl_mat_get_row(spot, kozeiPath, firstDateIndex);

	ParameterEstimation pe;
	PnlMat* window = ds->getEstimationWindow(0, numberDateEstimation);
	PnlMat* corr = pe.getCorrelationMatrix(window);
	PnlVect* sigma = pe.getVolatilitiesVector(window);
	BlackScholesModel* bs = new BlackScholesModel(kozeiPath->n, r, corr, sigma, spot);

	MonteCarlo *mc = new MonteCarlo(bs, K, h, n_samples);

	double last_iteration = (kozeiPath->m - firstDateIndex);

	PnlMat *past;
	double prix,ic;
	string strDelta = "[";

	string temp;

	double t = 0;
	int currentDate;
	for (int j = 0; j < last_iteration; j++) {
		currentDate = pnl_vect_get(dateIndexes, firstDateIndex + j);
		past = ds->getPast(t, K->T_, K->nbTimeSteps_);
		mc->price(past, t, prix, ic);

		temp = "{\"timestamp\":";
		temp += to_string(currentDate);
		temp += ", \"price\":";
		temp += to_string(prix);
		temp += "}";
		if (j < last_iteration - 1) {
			temp += ", ";
		}

		strDelta += temp;

		t += 1./Tools::NumberOfDaysPerYear;

	}

	strDelta += "]";

	pnl_mat_free(&assetData);
	pnl_mat_free(&exchangeRates);
	pnl_vect_free(&dateIndexes);
	pnl_mat_free(&kozeiPath);
	delete(ds);
	delete(K);
	pnl_mat_free(&past);
	pnl_vect_free(&spot);
	pnl_mat_free(&window);
	pnl_mat_free(&corr);
	pnl_vect_free(&sigma);
	delete(bs);
	delete(mc);


	return strDelta;
}

void Computations::getProfitAndLossForward(double &profitAndLoss, int n_samples,
	double h, double inv_init, double r, int numberOfRebalancement, double rho, double sigma, double spot) {
	Kozei *kozei = new Kozei(inv_init);
	ForwardTest* forwardTest = new ForwardTest(kozei, r, rho, sigma, spot, n_samples, h);
	int step = kozei->T_*Tools::NumberOfDaysPerYear / kozei->nbTimeSteps_;
	forwardTest->setRebalancementFrequence(numberOfRebalancement);
	profitAndLoss = forwardTest->ProfitAndLoss();
}

void Computations::getProfitAndLossBackward(double &profitAndLoss, int n_samples,
	double h, double inv_init, int numberDateEstimation, double r, int numberOfRebalancement) {
	profitAndLoss = 0.05;
}


void Computations::calleuro(double &ic, double &prix, int nb_samples, double T,
	double S0, double K, double sigma, double r)
{
	double drift = (r - sigma * sigma / 2.) * T;
	double sqrt_T = sqrt(T);
	double sum = 0;
	double var = 0;
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	double payoff;
	for (int i = 0; i < nb_samples; i++)
	{
		payoff = S0 * exp(drift + sigma * sqrt_T * pnl_rng_normal(rng));
		payoff = MAX(payoff - K, 0.);
		sum += payoff;
		var += payoff * payoff;
	}
	prix = exp(-r * T) * sum / nb_samples;
	var = exp(-2.*r*T) * var / nb_samples - prix * prix;
	ic = 1.96 * sqrt(var / nb_samples);
	pnl_rng_free(&rng);
}

void Computations::puteuro(double &ic, double &prix, int nb_samples, double T,
	double S0, double K, double sigma, double r)
{
	double drift = (r - sigma * sigma / 2.) * T;
	double sqrt_T = sqrt(T);
	double sum = 0;
	double var = 0;
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	double payoff;
	for (int i = 0; i < nb_samples; i++)
	{
		payoff = S0 * exp(drift + sigma * sqrt_T * pnl_rng_normal(rng));
		payoff = MAX(K - payoff, 0.);
		sum += payoff;
		var += payoff * payoff;
	}
	prix = exp(-r * T) * sum / nb_samples;
	var = exp(-2.*r*T) * var / nb_samples - prix * prix;
	ic = 1.96 * sqrt(var / nb_samples);
	pnl_rng_free(&rng);
}