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


add_executable (test_taux_change test_taux_change.cpp)
target_link_libraries(test_taux_change ${LIBS})
add_test(test_taux_change test_taux_change)

add_executable (test_hedgePortfolio_Kozei test_hedgePortfolio_Kozei.cpp)
target_link_libraries(test_hedgePortfolio_Kozei ${LIBS})
add_test(test_hedgePortfolio_Kozei test_hedgePortfolio_Kozei)

add_executable (test_Kozei_payoff test_Kozei_payoff.cpp)
target_link_libraries(test_Kozei_payoff ${LIBS})
add_test(test_Kozei_payoff test_Kozei_payoff)


#add_executable (test_HistoricalDataProvider test_HistoricalDataProvider.cpp)
#target_link_libraries(test_HistoricalDataProvider ${LIBS})
#add_test(test_HistoricalDataProvider test_HistoricalDataProvider)

#add_executable (test_SimulatedDataProvider test_SimulatedDataProvider.cpp)
#target_link_libraries(test_SimulatedDataProvider ${LIBS})
#add_test(test_SimulatedDataProvider test_SimulatedDataProvider)

add_executable (test_delta_kozei test_delta_kozei.cpp)
target_link_libraries(test_delta_kozei ${LIBS})
add_test(test_delta_kozei test_delta_kozei)

#add_executable (test_BackWardTest test_BackWardTest.cpp)
#target_link_libraries(test_BackWardTest ${LIBS})
#add_test(test_BackWardTest test_BackWardTest)

#add_executable (test_BackWardTestSixMonthsWithDataProvider test_BackWardTestSixMonthsWithDataProvider.cpp)
#target_link_libraries(test_BackWardTestSixMonthsWithDataProvider ${LIBS})
#add_test(test_BackWardTestSixMonthsWithDataProvider test_BackWardTestSixMonthsWithDataProvider)

#add_executable (test_kozei_BackWardTest test_kozei_BackWardTest.cpp)
#target_link_libraries(test_kozei_BackWardTest ${LIBS})
#add_test(test_kozei_BackWardTest test_kozei_BackWardTest)




# My Tests

add_executable (test_pnl test_pnl.cpp)
target_link_libraries(test_pnl ${LIBS})
add_test (test_pnl test_pnl)

add_executable (test_hedgePortfolio_Kozei_ratesEx test_hedgePortfolio_Kozei_ratesEx.cpp)
target_link_libraries(test_hedgePortfolio_Kozei_ratesEx ${LIBS})
add_test (test_hedgePortfolio_Kozei_ratesEx test_hedgePortfolio_Kozei_ratesEx)


#test unitaire avec des assertions

add_executable (test_unit_asset_t0 test_unit_asset_t0.cpp)
target_link_libraries(test_unit_asset_t0 ${LIBS})
add_test(test_unit_asset_t0 test_unit_asset_t0)

add_executable (test_unit_asset_ti test_unit_asset_ti.cpp)
target_link_libraries(test_unit_asset_ti ${LIBS})
add_test(test_unit_asset_ti test_unit_asset_ti)

add_executable (test_unit_asset_t test_unit_asset_t.cpp)
target_link_libraries(test_unit_asset_t ${LIBS})
add_test(test_unit_asset_t test_unit_asset_t)

add_executable (test_unit_concatenationMatrice test_unit_concatenationMatrice.cpp)
target_link_libraries(test_unit_concatenationMatrice ${LIBS})
add_test(test_unit_concatenationMatrice test_unit_concatenationMatrice)

add_executable (test_unit_shift test_unit_shift.cpp)
target_link_libraries(test_unit_shift ${LIBS})
add_test(test_unit_shift test_unit_shift)

add_executable (test_hedgePortfolio test_hedgePortfolio.cpp)
target_link_libraries(test_hedgePortfolio ${LIBS})
add_test(test_hedgePortfolio test_hedgePortfolio)

add_executable (test_unit_deltas test_unit_deltas.cpp)
target_link_libraries(test_unit_deltas ${LIBS})
add_test(test_unit_deltas test_unit_deltas)


#test unitaire de mémoire à utiliser avec valgrind

add_executable (test_mem_asset_t0 test_mem_asset_t0.cpp)
target_link_libraries(test_mem_asset_t0 ${LIBS})
add_test(test_mem_asset_t0 test_mem_asset_t0)

add_executable (test_mem_asset_t test_mem_asset_t.cpp)
target_link_libraries(test_mem_asset_t ${LIBS})
add_test(test_mem_asset_t test_mem_asset_t)

add_executable (test_mem_asset_ti test_mem_asset_ti.cpp)
target_link_libraries(test_mem_asset_ti ${LIBS})
add_test(test_mem_asset_ti test_mem_asset_ti)

add_executable (test_mem_shift test_mem_shift.cpp)
target_link_libraries(test_mem_shift ${LIBS})
add_test(test_mem_shift test_mem_shift)

add_executable (test_mem_price_t0 test_mem_price_t0.cpp)
target_link_libraries(test_mem_price_t0 ${LIBS})
add_test(test_mem_price_t0 test_mem_price_t0)

add_executable (test_mem_price_t test_mem_price_t.cpp)
target_link_libraries(test_mem_price_t ${LIBS})
add_test(test_mem_price_t test_mem_price_t)

add_executable (test_mem_price_ti test_mem_price_ti.cpp)
target_link_libraries(test_mem_price_ti ${LIBS})
add_test(test_mem_price_ti test_mem_price_ti)

add_executable (test_mem_delta_t test_mem_delta_t.cpp)
target_link_libraries(test_mem_delta_t ${LIBS})
add_test(test_mem_delta_t test_mem_delta_t)

add_executable (test_mem_delta_ti test_mem_delta_ti.cpp)
target_link_libraries(test_mem_delta_ti ${LIBS})
add_test(test_mem_delta_ti test_mem_delta_ti)

add_executable (test_mem_Kozei test_mem_Kozei.cpp)
target_link_libraries(test_mem_Kozei ${LIBS})
add_test(test_mem_Kozei test_mem_Kozei)

add_executable (test_vanilla_price test_vanilla_price.cpp)
target_link_libraries(test_vanilla_price ${LIBS})
add_test(test_vanilla_price test_vanilla_price)

add_executable (test_validation_PNL test_validation_PNL.cpp)
target_link_libraries(test_validation_PNL ${LIBS})
add_test(test_validation_PNL test_validation_PNL)


add_executable (test_validation_estimation test_validation_estimation.cpp)
target_link_libraries(test_validation_estimation ${LIBS})
add_test(test_validation_estimation test_validation_estimation)
