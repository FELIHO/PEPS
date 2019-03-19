#include <gtest/gtest.h>
#include "MonteCarlo.hpp"
#include "jlparser/parser.hpp"
#include "Asian.hpp"
#include "Basket.hpp"

using namespace std;

size_t n_samples;
int size, nbTimeSteps;
double T, r, strike, rho;
PnlVect *spot, *sigma, *weights;
string type;
BlackScholesModel *bs_model;
MonteCarlo *mc_pricer;
Option *option;
PnlVect *expected_delta;

PnlVect *delta = pnl_vect_new();
PnlMat *past = pnl_mat_new();


TEST(Delta, basket_1) {
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
    expected_delta = pnl_vect_create_from_list(size, 0.0248956, 0.0249324, 0.0249503, 0.0250191, 0.0249342, 0.024961, 0.0249808, 0.0248986, 0.0249032, 0.0249421, 0.0249554, 0.0249416, 0.0249236, 0.0249541, 0.0249515, 0.0250078, 0.0248439, 0.0248783, 0.0249421, 0.0249178, 0.0249168, 0.0249511, 0.0249393, 0.0250007, 0.0249344, 0.0248973, 0.0249136, 0.0249047, 0.024927, 0.0248953, 0.0249739, 0.024962, 0.0249467, 0.0249322, 0.0249571, 0.0249858, 0.0248853, 0.024914, 0.024861, 0.0250143);
    

    // Initialisation Option
    option = new Basket(T, nbTimeSteps, size, strike, weights);
    // Initialisation de Blackscholes
    bs_model = new BlackScholesModel(size, r, rho, sigma, spot);
    // Initialization de Montecarlo
    mc_pricer = new MonteCarlo(bs_model, option, 0.1, n_samples);

    //delta
    pnl_mat_resize(past,1,size);
    pnl_mat_set_row(past,spot,0);
    mc_pricer->delta(past, 0.0, delta);

    //assertion
    bool b = pnl_vect_isequal(delta,expected_delta, 0.001);
    ASSERT_TRUE(b);
}

TEST(Delta, basket_2) {
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
    expected_delta = pnl_vect_create_from_list(size, 0.0162094, 0.01621, 0.01622, 0.0162203, 0.0162145, 0.0162044, 0.016215, 0.0162163, 0.0161995, 0.0161993, 0.0162085, 0.0161999, 0.0161848, 0.0161824, 0.0162018, 0.0161994, 0.0161971, 0.0162066, 0.0162119, 0.0162086, 0.0162088, 0.0162014, 0.0162149, 0.0162236, 0.0162311, 0.0162138, 0.0162064, 0.0162129, 0.0162166, 0.0162183, 0.0161925, 0.0162276, 0.016189, 0.0161958, 0.0162017, 0.0162142, 0.0162028, 0.0162092, 0.0162104, 0.0162025);

    // Initialisation Option
    option = new Basket(T, nbTimeSteps, size, strike, weights);
    // Initialisation de Blackscholes
    bs_model = new BlackScholesModel(size, r, rho, sigma, spot);
    // Initialization de Montecarlo
    mc_pricer = new MonteCarlo(bs_model, option, 0.1, n_samples);

    //delta
    pnl_mat_resize(past,1,size);
    pnl_mat_set_row(past,spot,0);
    mc_pricer->delta(past, 0.0, delta);

    //assertion
    bool b = pnl_vect_isequal(delta,expected_delta, 0.001);
    ASSERT_TRUE(b);
}

TEST(Delta, asian) {
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
    expected_delta = pnl_vect_create_from_list(size, 0.281640, 0.281951);


    // Initialisation Option
    option = new Asian(T, nbTimeSteps, size, strike, weights);
    // Initialisation de Blackscholes
    bs_model = new BlackScholesModel(size, r, rho, sigma, spot);
    // Initialization de Montecarlo
    mc_pricer = new MonteCarlo(bs_model, option, 0.1, n_samples);

    //delta
    pnl_mat_resize(past,1,size);
    pnl_mat_set_row(past,spot,0);
    mc_pricer->delta(past, 0.0, delta);

    //assertion
    bool b = pnl_vect_isequal(delta,expected_delta, 0.001);
    ASSERT_TRUE(b);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}