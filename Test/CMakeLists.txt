enable_testing()

include_directories (${CMAKE_SOURCE_DIR}/src)


set(LIBS ${LIBS} Pricer Options)


#############################################
################## PEPS  ####################
#############################################

#add_executable (test_kozei test_kozei.cpp)
#add_test(test_kozei test_kozei)
#target_link_libraries(test_kozei ${LIBS})

#add_executable (test_dataselecter test_dataSelecter.cpp)
#target_link_libraries(test_dataselecter ${LIBS})
#add_test(test_dataselecter test_dataselecter)

#add_executable (test_getCorrelation test_getCorrelation.cpp)
#target_link_libraries(test_getCorrelation ${LIBS})
#add_test(test_getCorrelation test_getCorrelation)

#add_executable (test_getCovariance test_getCovariance.cpp)
#target_link_libraries(test_getCovariance ${LIBS})
#add_test(test_getCovariance test_getCovariance)

#add_executable (test_getLogRendements test_getLogRendements.cpp)
#target_link_libraries(test_getLogRendements ${LIBS})
#add_test(test_getLogRendements test_getLogRendements)

#add_executable (test_getVolatilies test_getVolatilies.cpp)
#target_link_libraries(test_getVolatilies ${LIBS})
#add_test(test_getVolatilies test_getVolatilies)


#add_executable (test_taux_change test_taux_change.cpp)
#target_link_libraries(test_taux_change ${LIBS})
#add_test(test_taux_change test_taux_change)

#add_executable (test_hedgePortfolio_Kozei test_hedgePortfolio_Kozei.cpp)
#target_link_libraries(test_hedgePortfolio_Kozei ${LIBS})
#add_test(test_hedgePortfolio_Kozei test_hedgePortfolio_Kozei)


#add_executable (test_HistoricalDataProvider test_HistoricalDataProvider.cpp)
#target_link_libraries(test_HistoricalDataProvider ${LIBS})
#add_test(test_HistoricalDataProvider test_HistoricalDataProvider)

#add_executable (test_SimulatedDataProvider test_SimulatedDataProvider.cpp)
#target_link_libraries(test_SimulatedDataProvider ${LIBS})
#add_test(test_SimulatedDataProvider test_SimulatedDataProvider)

#add_executable (test_delta_kozei test_delta_kozei.cpp)
#target_link_libraries(test_delta_kozei ${LIBS})
#add_test(test_delta_kozei test_delta_kozei)

#add_executable (test_BackWardTest test_BackWardTest.cpp)
#target_link_libraries(test_BackWardTest ${LIBS})
#add_test(test_BackWardTest test_BackWardTest)

#add_executable (test_BackWardTestSixMonthsWithDataProvider test_BackWardTestSixMonthsWithDataProvider.cpp)
#target_link_libraries(test_BackWardTestSixMonthsWithDataProvider ${LIBS})
#add_test(test_BackWardTestSixMonthsWithDataProvider test_BackWardTestSixMonthsWithDataProvider)

#add_executable (test_kozei_BackWardTest test_kozei_BackWardTest.cpp)
#target_link_libraries(test_kozei_BackWardTest ${LIBS})
#add_test(test_kozei_BackWardTest test_kozei_BackWardTest)

#add_executable (test_mem_Kozei test_mem_Kozei.cpp)
#target_link_libraries(test_mem_Kozei ${LIBS})
#add_test(test_mem_Kozei test_mem_Kozei)

# My Tests

add_executable (test_pnl test_pnl.cpp)
target_link_libraries(test_pnl jl_parser ${LIBS})
add_test (test_pnl test_pnl)

add_executable (test_asian test_asian.cpp)
target_link_libraries(test_asian jl_parser ${LIBS})
add_test (test_asian test_asian)

add_executable (test_basket test_basket.cpp)
target_link_libraries(test_basket jl_parser ${LIBS})
add_test (test_basket test_basket)

add_executable (test_basket_1 test_basket_1.cpp)
target_link_libraries(test_basket_1 jl_parser ${LIBS})
add_test (test_basket_1 test_basket_1)

add_executable (test_basket_2 test_basket_2.cpp)
target_link_libraries(test_basket_2 jl_parser ${LIBS})
add_test (test_basket_2 test_basket_2)

add_executable (test_performance test_performance.cpp)
target_link_libraries(test_performance jl_parser ${LIBS})
add_test (test_performance test_performance)

add_executable (test_BlackScholesModel test_BlackScholesModel.cpp)
target_link_libraries(test_BlackScholesModel jl_parser ${LIBS})
add_test(test_BlackScholesModel test_BlackScholesModel)

#test unitaire avec des assertions

add_executable (test_unit_asset_t0 test_unit_asset_t0.cpp)
target_link_libraries(test_unit_asset_t0 jl_parser ${LIBS})
add_test(test_unit_asset_t0 test_unit_asset_t0)

add_executable (test_unit_asset_ti test_unit_asset_ti.cpp)
target_link_libraries(test_unit_asset_ti jl_parser ${LIBS})
add_test(test_unit_asset_ti test_unit_asset_ti)

add_executable (test_unit_asset_t test_unit_asset_t.cpp)
target_link_libraries(test_unit_asset_t jl_parser ${LIBS})
add_test(test_unit_asset_t test_unit_asset_t)

add_executable (test_unit_concatenationMatrice test_unit_concatenationMatrice.cpp)
target_link_libraries(test_unit_concatenationMatrice jl_parser ${LIBS})
add_test(test_unit_concatenationMatrice test_unit_concatenationMatrice)

add_executable (test_unit_shift test_unit_shift.cpp)
target_link_libraries(test_unit_shift jl_parser ${LIBS})
add_test(test_unit_shift test_unit_shift)

add_executable (test_hedgePortfolio test_hedgePortfolio.cpp)
target_link_libraries(test_hedgePortfolio jl_parser ${LIBS})
add_test(test_hedgePortfolio test_hedgePortfolio)

add_executable (test_hedgePortfolio_Kozei_ratesEx test_hedgePortfolio_Kozei_ratesEx.cpp)
target_link_libraries(test_hedgePortfolio_Kozei_ratesEx jl_parser ${LIBS})
add_test(test_hedgePortfolio_Kozei_ratesEx test_hedgePortfolio_Kozei_ratesEx)


add_executable (test_unit_deltas test_unit_deltas.cpp)
target_link_libraries(test_unit_deltas jl_parser ${LIBS})
add_test(test_unit_deltas test_unit_deltas)


#test unitaire de mémoire à utiliser avec valgrind

add_executable (test_mem_asset_t0 test_mem_asset_t0.cpp)
target_link_libraries(test_mem_asset_t0 jl_parser ${LIBS})
add_test(test_mem_asset_t0 test_mem_asset_t0)

add_executable (test_mem_asset_t test_mem_asset_t.cpp)
target_link_libraries(test_mem_asset_t jl_parser ${LIBS})
add_test(test_mem_asset_t test_mem_asset_t)

add_executable (test_mem_asset_ti test_mem_asset_ti.cpp)
target_link_libraries(test_mem_asset_ti jl_parser ${LIBS})
add_test(test_mem_asset_ti test_mem_asset_ti)

add_executable (test_mem_shift test_mem_shift.cpp)
target_link_libraries(test_mem_shift jl_parser ${LIBS})
add_test(test_mem_shift test_mem_shift)

add_executable (test_mem_price_t0 test_mem_price_t0.cpp)
target_link_libraries(test_mem_price_t0 jl_parser ${LIBS})
add_test(test_mem_price_t0 test_mem_price_t0)

add_executable (test_mem_price_t test_mem_price_t.cpp)
target_link_libraries(test_mem_price_t jl_parser ${LIBS})
add_test(test_mem_price_t test_mem_price_t)

add_executable (test_mem_price_ti test_mem_price_ti.cpp)
target_link_libraries(test_mem_price_ti jl_parser ${LIBS})
add_test(test_mem_price_ti test_mem_price_ti)

add_executable (test_mem_delta_t test_mem_delta_t.cpp)
target_link_libraries(test_mem_delta_t jl_parser ${LIBS})
add_test(test_mem_delta_t test_mem_delta_t)

add_executable (test_mem_delta_ti test_mem_delta_ti.cpp)
target_link_libraries(test_mem_delta_ti jl_parser ${LIBS})
add_test(test_mem_delta_ti test_mem_delta_ti)
