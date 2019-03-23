#include "Computations.hpp"

void Computations::getPriceKozeiNative(double &ic, double &prix, int n_samples,
	double h, int date, double inv_init, int numberDateEstimation, double r)
{
	string str3;
	string str4;
	string str5;
	str3 = "/home/jmari/ENSIMAG/3A/PEPS/p.e.p.s/Hist_kozei";
	str4 = "/home/jmari/ENSIMAG/3A/PEPS/p.e.p.s/Hist_kozei";
	str5 = "/home/jmari/ENSIMAG/3A/PEPS/p.e.p.s/Hist_kozei";

	str3 = str3 + "kozei_dataFeed.dat";
	const char *cstr = str3.c_str();
	PnlMat *assetData = pnl_mat_create_from_file(cstr);

	str4 = str4 + "kozei_changeRates.dat";
	const char *cstr2 = str4.c_str();
	PnlMat *exchangeRates = pnl_mat_create_from_file(cstr2);
	
	str5 = str5 + "kozei_IndexdataFeed.dat";
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
	str3 = "/home/jmari/ENSIMAG/3A/PEPS/p.e.p.s/Hist_kozei";
	str4 = "/home/jmari/ENSIMAG/3A/PEPS/p.e.p.s/Hist_kozei";
	str5 = "/home/jmari/ENSIMAG/3A/PEPS/p.e.p.s/Hist_kozei";

	str3 = str3 + "kozei_dataFeed.dat";
	const char *cstr = str3.c_str();
	PnlMat *assetData = pnl_mat_create_from_file(cstr);

	str4 = str4 + "kozei_changeRates.dat";
	const char *cstr2 = str4.c_str();
	PnlMat *exchangeRates = pnl_mat_create_from_file(cstr2);
	
	str5 = str5 + "kozei_IndexdataFeed.dat";
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


	PnlVect *deltaV = pnl_vect_create_from_scalar(past->n, 0.0);
	mc->delta(past, t, deltaV);

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
	Names.push_back("USD/EUR");
	Names.push_back("JPY/EUR");
	Names.push_back("GBP/EUR");
	Names.push_back("CHF/EUR");
	Names.push_back("BRL/EUR");

	std::list<std::string>::iterator iter;
	std::string findString = "AWK";
	string strDelta = "[";
	string temp;

	if ((iter = std::find(Names.begin(), Names.end(), findString)) != Names.end())
	{
		for (int idx = 0; idx < past->n; idx++)
		{
			temp = "{ name:\"";
			temp += *iter;
			temp += "\", delta:";
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

	pnl_vect_free(&deltaV);
	return strDelta;
}


string Computations::getListPriceKozeiNative(int n_samples,
	double h, double inv_init, int numberDateEstimation, double r)
{

	string str3;
	string str4;
	string str5;
	str3 = "/home/jmari/ENSIMAG/3A/PEPS/p.e.p.s/Hist_kozei";
	str4 = "/home/jmari/ENSIMAG/3A/PEPS/p.e.p.s/Hist_kozei";
	str5 = "/home/jmari/ENSIMAG/3A/PEPS/p.e.p.s/Hist_kozei";

	str3 = str3 + "kozei_dataFeed.dat";
	const char *cstr = str3.c_str();
	PnlMat *assetData = pnl_mat_create_from_file(cstr);

	str4 = str4 + "kozei_changeRates.dat";
	const char *cstr2 = str4.c_str();
	PnlMat *exchangeRates = pnl_mat_create_from_file(cstr2);
	
	str5 = str5 + "kozei_IndexdataFeed.dat";
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

	PnlMat *past;
	double prix,ic;
	string strDelta = "[";

	string temp;

	double t = 0;
	int currentDate;
	for (int j = 0; j < K->T_*Tools::NumberOfDaysPerYear; j++) {
		currentDate = pnl_vect_get(dateIndexes, firstDateIndex + j);
		past = ds->getPast(t, K->T_, K->nbTimeSteps_);
		mc->price(past, t, prix, ic);

		temp = "{ timestamp:";
		temp += to_string(currentDate);
		temp += ", price:";
		temp += to_string(prix);
		temp += "}";
		if (j < K->T_*Tools::NumberOfDaysPerYear - 1) {
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

// void Computations::getPriceKozeiNative(double &ic, double &prix, int n_samples,
// 	double h, int date, double inv_init, int numberDateEstimation, double r)
// {
// 	string str3;
// 	string str4;
// 	string str5;
// 	str3 = "C:\\Users\\lione\\Desktop\\Ensimag\\PEPS\\NEWPEPS\\Kozei_DataFeeds\\";
// 	str4 = "C:\\Users\\lione\\Desktop\\Ensimag\\PEPS\\NEWPEPS\\Kozei_DataFeeds\\";
// 	str5 = "C:\\Users\\lione\\Desktop\\Ensimag\\PEPS\\NEWPEPS\\Kozei_DataFeeds\\";

// 	str3 = str3 + "kozei_dataFeed.dat";
// 	const char *cstr = str3.c_str();
// 	PnlMat *allData = pnl_mat_create_from_file(cstr);
	
// 	str4 = str4 + "kozei_IndexdataFeed.dat";
// 	const char *cstr2 = str4.c_str();
// 	PnlVectInt *dateIndexes = pnl_vect_int_create_from_file(cstr2);

// 	str5 = str5 + "kozei_Constatation.dat";
// 	const char *cstr3 = str5.c_str();
// 	PnlVectInt *constationDate = pnl_vect_int_create_from_file(cstr3);


// 	SuperDataSelecter ds = SuperDataSelecter(constationDate);
// 	PnlMat *past = ds.getPast(allData, dateIndexes, date);

// 	PnlVect* spot = pnl_vect_new();
// 	pnl_mat_get_row(spot, past, 0);

// 	ParameterEstimation pe;
// 	PnlMat window = ds.getWindowPreviousEstimation(allData, dateIndexes, date, numberDateEstimation);
// 	PnlMat* corr = pe.getCorrelationMatrix(&window);
// 	PnlVect* sigma = pe.getVolatilitiesVector(&window);
// 	BlackScholesModel *bc = new BlackScholesModel(30, r, corr, sigma, spot);
// 	Kozei *kozei_test = new Kozei(inv_init);
// 	MonteCarlo *mc_test = new MonteCarlo(bc, kozei_test, h, n_samples);
// 	double T = 8.0 + 11.0 / 365;
// 	double t = ds.computeTvalue(dateIndexes, pnl_vect_int_get(constationDate, 0), date, T);
// 	mc_test->price(past, t, prix, ic);

// 	pnl_vect_free(&spot);
// 	pnl_vect_free(&sigma);
// 	pnl_vect_int_free(&dateIndexes);

// 	pnl_mat_free(&allData);
// 	pnl_mat_free(&past);
// 	pnl_mat_free(&corr);

// }


// string Computations::getDeltaKozeiNative(int n_samples,
// 	double h, int date, double inv_init, int numberDateEstimation, double r)
// {

// 	string str3;
// 	string str4;
// 	string str5;
// 	str3 = "C:\\Users\\lione\\Desktop\\Ensimag\\PEPS\\NEWPEPS\\Kozei_DataFeeds\\";
// 	str4 = "C:\\Users\\lione\\Desktop\\Ensimag\\PEPS\\NEWPEPS\\Kozei_DataFeeds\\";
// 	str5 = "C:\\Users\\lione\\Desktop\\Ensimag\\PEPS\\NEWPEPS\\Kozei_DataFeeds\\";

// 	str3 = str3 + "kozei_dataFeed.dat";
// 	const char *cstr = str3.c_str();
// 	PnlMat *allData = pnl_mat_create_from_file(cstr);

// 	str4 = str4 + "kozei_IndexdataFeed.dat";
// 	const char *cstr2 = str4.c_str();
// 	PnlVectInt *dateIndexes = pnl_vect_int_create_from_file(cstr2);

// 	str5 = str5 + "kozei_Constatation.dat";
// 	const char *cstr3 = str5.c_str();
// 	PnlVectInt *constationDate = pnl_vect_int_create_from_file(cstr3);


// 	SuperDataSelecter ds = SuperDataSelecter(constationDate);
// 	PnlMat *past = ds.getPast(allData, dateIndexes, date);

// 	PnlVect* spot = pnl_vect_new();
// 	pnl_mat_get_row(spot, past, 0);

// 	PnlVect *deltaV = pnl_vect_create_from_scalar(past->n, 0.0);
// 	PnlVect *icdelta = pnl_vect_create_from_scalar(past->n, 0.0);

// 	ParameterEstimation pe;
// 	PnlMat window = ds.getWindowPreviousEstimation(allData, dateIndexes, date, numberDateEstimation);
// 	PnlMat* corr = pe.getCorrelationMatrix(&window);
// 	PnlVect* sigma = pe.getVolatilitiesVector(&window);
// 	BlackScholesModel *bc = new BlackScholesModel(30, r, corr, sigma, spot);
// 	Kozei *kozei_test = new Kozei(inv_init);
// 	MonteCarlo *mc_test = new MonteCarlo(bc, kozei_test, h, n_samples);
// 	double T = 8.0 + 11.0 / 365;
// 	double t = ds.computeTvalue(dateIndexes, pnl_vect_int_get(constationDate, 0), date, T);
// 	mc_test->delta(past, t, deltaV, icdelta);

// 	list<string> Names;
// 	Names.push_back("AWK");
// 	Names.push_back("A");
// 	Names.push_back("DHR");
// 	Names.push_back("ECL");
// 	Names.push_back("FLS");
// 	Names.push_back("PNR");
// 	Names.push_back("UUGRY");
// 	Names.push_back("AGCO");
// 	Names.push_back("CNI");
// 	Names.push_back("CF");
// 	Names.push_back("K");
// 	Names.push_back("MOS");
// 	Names.push_back("MDLZ");
// 	Names.push_back("DE");
// 	Names.push_back("SU");
// 	Names.push_back("CSX");
// 	Names.push_back("NSC");
// 	Names.push_back("6326.TOK");
// 	Names.push_back("9020.TOK");
// 	Names.push_back("SVT.LON");
// 	Names.push_back("WEIR.LON");
// 	Names.push_back("SEV.PAR");
// 	Names.push_back("DSM.AMS");
// 	Names.push_back("SGO.PAR");
// 	Names.push_back("AA9.BER");
// 	Names.push_back("LR.PAR");
// 	Names.push_back("PHIA.AMS");
// 	Names.push_back("SIE.FRK");
// 	Names.push_back("ABBN.VSX");
// 	Names.push_back("BRFS3.SAO");

// 	std::list<std::string>::iterator iter;
// 	std::string findString = "AWK";
// 	string strDelta = "[";
// 	string temp;

// 	if ((iter = std::find(Names.begin(), Names.end(), findString)) != Names.end())
// 	{
// 		for (int idx = 0; idx < 30; idx++)
// 		{
// 			temp = "{ name:\"";
// 			temp += *iter;
// 			temp += "\", delta:";
// 			temp += to_string(pnl_vect_get(deltaV, idx));
// 			temp += "}";
// 			if (idx < 29) {
// 				temp += ", ";
// 			}
// 			strDelta += temp;
// 			++iter;
// 		}

// 	}

// 	strDelta += "]";

// 	pnl_vect_free(&spot);
// 	pnl_vect_free(&deltaV);
// 	pnl_vect_free(&icdelta);
// 	pnl_vect_free(&sigma);
// 	pnl_vect_int_free(&dateIndexes);

// 	pnl_mat_free(&allData);
// 	pnl_mat_free(&past);
// 	pnl_mat_free(&corr);
// 	return strDelta;
// }


// string Computations::getListPriceKozeiNative(int n_samples,
// 	double h, double inv_init, int numberDateEstimation, double r)
// {

// 	string str3;
// 	string str4;
// 	string str5;
// 	str3 = "C:\\Users\\lione\\Desktop\\Ensimag\\PEPS\\NEWPEPS\\Kozei_DataFeeds\\";
// 	str4 = "C:\\Users\\lione\\Desktop\\Ensimag\\PEPS\\NEWPEPS\\Kozei_DataFeeds\\";
// 	str5 = "C:\\Users\\lione\\Desktop\\Ensimag\\PEPS\\NEWPEPS\\Kozei_DataFeeds\\";

// 	str3 = str3 + "kozei_dataFeed.dat";
// 	const char *cstr = str3.c_str();
// 	PnlMat *allData = pnl_mat_create_from_file(cstr);

// 	str4 = str4 + "kozei_IndexdataFeed.dat";
// 	const char *cstr2 = str4.c_str();
// 	PnlVectInt *dateIndexes = pnl_vect_int_create_from_file(cstr2);

// 	str5 = str5 + "kozei_Constatation.dat";
// 	const char *cstr3 = str5.c_str();
// 	PnlVectInt *constationDate = pnl_vect_int_create_from_file(cstr3);


// 	SuperDataSelecter ds = SuperDataSelecter(constationDate);
// 	PnlMat *past = pnl_mat_new();
// 	PnlVect* spot = pnl_vect_new();
// 	double ic, prix, t;
// 	int dateCurrent;

// 	ParameterEstimation pe;
// 	PnlMat window = ds.getWindowPreviousEstimation(allData, dateIndexes, pnl_vect_int_get(constationDate, 0), numberDateEstimation);
// 	PnlMat* corr = pe.getCorrelationMatrix(&window);
// 	PnlVect* sigma = pe.getVolatilitiesVector(&window);
// 	BlackScholesModel *bc = new BlackScholesModel(30, r, corr, sigma, spot);
// 	Kozei *kozei_test = new Kozei(inv_init);
// 	MonteCarlo *mc_test = new MonteCarlo(bc, kozei_test, h, n_samples);
// 	double T = 8.0 + 11.0 / 365;	
// 	int begin = ds.getIndexSpotDate(dateIndexes) + 10;
// 	string strDelta = "[";
// 	string temp;

// 	for (int j = begin; j < dateIndexes->size; j++) {
// 		dateCurrent = pnl_vect_int_get(dateIndexes, j);
// 		t = ds.computeTvalue(dateIndexes, pnl_vect_int_get(constationDate, 0), dateCurrent, T);
// 		past = ds.getPast(allData, dateIndexes, dateCurrent);
// 		pnl_mat_get_row(spot, past, 0);
// 		mc_test->price(past, t, prix, ic);

// 		temp = "{ timestamp:";
// 		temp += to_string(dateCurrent);
// 		temp += ", price:";
// 		temp += to_string(prix);
// 		temp += "}";
// 		if (j < dateIndexes->size - 1) {
// 			temp += ", ";
// 		}
// 		strDelta += temp;

// 	}

// 	strDelta += "]";

// 	pnl_vect_free(&spot);
// 	pnl_vect_free(&sigma);
// 	pnl_vect_int_free(&dateIndexes);

// 	pnl_mat_free(&allData);
// 	pnl_mat_free(&past);
// 	pnl_mat_free(&corr);
// 	return strDelta;
// }