enable_testing()

include_directories (${CMAKE_SOURCE_DIR}/src)


set(LIBS ${LIBS} Pricer Options)


# My Tests


#############################################
################## PEPS  ####################
#############################################

add_executable (test_kozei test_kozei.cpp)
target_link_libraries(test_kozei ${LIBS})
add_test(test_kozei test_kozei)

add_executable (test_dataselecter test_dataSelecter.cpp)
target_link_libraries(test_dataselecter ${LIBS})
add_test(test_dataselecter test_dataselecter)

add_executable (test_getCorrelation test_getCorrelation.cpp)
target_link_libraries(test_getCorrelation ${LIBS})
add_test(test_getCorrelation test_getCorrelation)

add_executable (test_getCovariance test_getCovariance.cpp)
target_link_libraries(test_getCovariance ${LIBS})
add_test(test_getCovariance test_getCovariance)

add_executable (test_getLogRendements test_getLogRendements.cpp)
target_link_libraries(test_getLogRendements ${LIBS})
add_test(test_getLogRendements test_getLogRendements)

add_executable (test_getVolatilies test_getVolatilies.cpp)
target_link_libraries(test_getVolatilies ${LIBS})
add_test(test_getVolatilies test_getVolatilies)


add_executable (test_taux_change test_taux_change.cpp)
target_link_libraries(test_taux_change ${LIBS})
add_test(test_taux_change test_taux_change)

add_executable (test_hedgePortfolio_Kozei test_hedgePortfolio_Kozei.cpp)
target_link_libraries(test_hedgePortfolio_Kozei ${LIBS})
add_test(test_hedgePortfolio_Kozei test_hedgePortfolio_Kozei)

add_executable (test_HistoricalDataProvider test_HistoricalDataProvider.cpp)
target_link_libraries(test_HistoricalDataProvider ${LIBS})
add_test(test_HistoricalDataProvider test_HistoricalDataProvider)

add_executable (test_SimulatedDataProvider test_SimulatedDataProvider.cpp)
target_link_libraries(test_SimulatedDataProvider ${LIBS})
add_test(test_SimulatedDataProvider test_SimulatedDataProvider)

add_executable (test_BackWardTest test_BackWardTest.cpp)
target_link_libraries(test_BackWardTest ${LIBS})
add_test(test_BackWardTest test_BackWardTest)

add_executable (test_BackWardTestSixMonthsWithDataProvider test_BackWardTestSixMonthsWithDataProvider.cpp)
target_link_libraries(test_BackWardTestSixMonthsWithDataProvider ${LIBS})
add_test(test_BackWardTestSixMonthsWithDataProvider test_BackWardTestSixMonthsWithDataProvider)

add_executable (test_kozei_BackWardTest test_kozei_BackWardTest.cpp)
target_link_libraries(test_kozei_BackWardTest ${LIBS})
add_test(test_kozei_BackWardTest test_kozei_BackWardTest)
