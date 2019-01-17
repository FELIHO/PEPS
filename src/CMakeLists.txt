
include_directories(.)

add_library(Pricer BlackScholesModel.cpp MonteCarlo.cpp HedgePortfolio.cpp FakeRnd.cpp PnlRand.cpp)
add_library(Options Basket.cpp Asian.cpp Performance.cpp)
