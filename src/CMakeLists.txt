
include_directories(.)

add_library(Pricer BlackScholesModel.cpp MonteCarlo.cpp HedgePortfolio.cpp)
add_library(Options Basket.cpp Asian.cpp Performance.cpp)
