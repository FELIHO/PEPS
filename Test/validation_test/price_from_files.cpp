#include <gtest/gtest.h>
#include "MonteCarlo.hpp"
#include "jlparser/parser.hpp"
#include "Asian.hpp"
#include "Basket.hpp"
#include "Performance.hpp"

using namespace std;

size_t n_samples;
int size, nbTimeSteps;
double T, r, strike, rho;
PnlVect *spot, *sigma, *weights;
string type;
BlackScholesModel *bs_model;
MonteCarlo *mc_pricer;
Option *option;
double prix,ic;
double expected_prix,expected_ic;


TEST(Price, basket) {
    //Extracting data from file
    char const *basket_file = "../data/basket.dat";
    Param *P = new Parser(basket_file);
    P->extract("option type", type);
    P->extract("option size", size);
    P->extract("strike", strike);
    P->extract("spot", spot, size);
    P->extract("maturity", T);
    P->extract("volatility", sigma, size);
    P->extract("interest rate", r);
    P->extract("correlation", rho);
    P->extract("payoff coefficients", weights, size);
    P->extract("timestep number", nbTimeSteps);
    P->extract("sample number", n_samples);
    expected_prix = 13.627;
    expected_ic = 0.025*1.96*2;

    // Initialisation Option
    option = new Basket(T, nbTimeSteps, size, strike, weights);
    // Initialisation de Blackscholes
    bs_model = new BlackScholesModel(size, r, rho, sigma, spot);
    // Initialization de Montecarlo
    mc_pricer = new MonteCarlo(bs_model, option, 0.1, n_samples);

    //price
    mc_pricer->price(prix,ic);

    //assertion
    EXPECT_NEAR(prix,expected_prix,ic);
    EXPECT_NEAR(prix,expected_prix,expected_ic);
    EXPECT_NEAR(ic,expected_ic,0.15*expected_ic);
}

TEST(Price, basket_1) {
    //Extracting data from file
    char const *basket1_file = "../data/basket_1.dat";
    Param *P = new Parser(basket1_file);
    P->extract("option type", type);
    P->extract("option size", size);
    P->extract("strike", strike);
    P->extract("spot", spot, size);
    P->extract("maturity", T);
    P->extract("volatility", sigma, size);
    P->extract("interest rate", r);
    P->extract("correlation", rho);
    P->extract("payoff coefficients", weights, size);
    P->extract("timestep number", nbTimeSteps);
    P->extract("sample number", n_samples);
    expected_prix = 13.616294;
    expected_ic = 2*1.96*sqrt(32.053411/n_samples);

    // Initialisation Option
    option = new Basket(T, nbTimeSteps, size, strike, weights);
    // Initialisation de Blackscholes
    bs_model = new BlackScholesModel(size, r, rho, sigma, spot);
    // Initialization de Montecarlo
    mc_pricer = new MonteCarlo(bs_model, option, 0.1, n_samples);

    //price
    mc_pricer->price(prix,ic);

    //assertion
    EXPECT_NEAR(prix,expected_prix,ic);
    EXPECT_NEAR(prix,expected_prix,expected_ic);
    EXPECT_NEAR(ic,expected_ic,0.15*expected_ic);
}

TEST(Price, basket_2) {
    //Extracting data from file
    char const *basket2_file = "../data/basket_2.dat";
    Param *P = new Parser(basket2_file);
    P->extract("option type", type);
    P->extract("option size", size);
    P->extract("strike", strike);
    P->extract("spot", spot, size);
    P->extract("maturity", T);
    P->extract("volatility", sigma, size);
    P->extract("interest rate", r);
    P->extract("correlation", rho);
    P->extract("payoff coefficients", weights, size);
    P->extract("timestep number", nbTimeSteps);
    P->extract("sample number", n_samples);
    expected_prix = 9.238710;
    expected_ic = 2*1.96*sqrt(152.094852/n_samples);

    // Initialisation Option
    option = new Basket(T, nbTimeSteps, size, strike, weights);
    // Initialisation de Blackscholes
    bs_model = new BlackScholesModel(size, r, rho, sigma, spot);
    // Initialization de Montecarlo
    mc_pricer = new MonteCarlo(bs_model, option, 0.1, n_samples);

    //price
    mc_pricer->price(prix,ic);

    //assertion
    EXPECT_NEAR(prix,expected_prix,2*ic);
    EXPECT_NEAR(prix,expected_prix,2*expected_ic);
    EXPECT_NEAR(ic,expected_ic,0.2*expected_ic);   
}

TEST(Price, asian) {
    //Extracting data from file
    char const *asian_file = "../data/asian.dat";
    Param *P = new Parser(asian_file);
    P->extract("option type", type);
    P->extract("option size", size);
    P->extract("strike", strike);
    P->extract("spot", spot, size);
    P->extract("maturity", T);
    P->extract("volatility", sigma, size);
    P->extract("interest rate", r);
    P->extract("correlation", rho);
    P->extract("payoff coefficients", weights, size);
    P->extract("timestep number", nbTimeSteps);
    P->extract("sample number", n_samples);
    expected_prix = 3.91;
    expected_ic = 2*1.96*0.029;

    // Initialisation Option
    option = new Asian(T, nbTimeSteps, size, strike, weights);
    // Initialisation de Blackscholes
    bs_model = new BlackScholesModel(size, r, rho, sigma, spot);
    // Initialization de Montecarlo
    mc_pricer = new MonteCarlo(bs_model, option, 0.1, n_samples);

    //price
    mc_pricer->price(prix,ic);

    //assertion
    EXPECT_NEAR(prix,expected_prix,2*ic);
    EXPECT_NEAR(prix,expected_prix,2*expected_ic);
    EXPECT_NEAR(ic,expected_ic,0.2*expected_ic);  
}

TEST(Price, perf) {
    //Extracting data from file
    char const *perf_file = "../data/perf.dat";
    Param *P = new Parser(perf_file);
    P->extract("option type", type);
    P->extract("option size", size);
    P->extract("spot", spot, size);
    P->extract("maturity", T);
    P->extract("volatility", sigma, size);
    P->extract("interest rate", r);
    P->extract("correlation", rho);
    P->extract("payoff coefficients", weights, size);
    P->extract("timestep number", nbTimeSteps);
    P->extract("sample number", n_samples);
    expected_prix = 1.287353;
    expected_ic = 2*1.96*0.000587;

    // Initialisation Option
    option = new Performance(T, nbTimeSteps, size, weights);
    // Initialisation de Blackscholes
    bs_model = new BlackScholesModel(size, r, rho, sigma, spot);
    // Initialization de Montecarlo
    mc_pricer = new MonteCarlo(bs_model, option, 0.1, n_samples);

    //price
    mc_pricer->price(prix,ic);

    //assertion
    EXPECT_NEAR(prix,expected_prix,2*ic);
    EXPECT_NEAR(prix,expected_prix,2*expected_ic);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}