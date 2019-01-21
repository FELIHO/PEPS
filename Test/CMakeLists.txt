enable_testing()

include_directories (${CMAKE_SOURCE_DIR}/src)


set(LIBS ${LIBS} Pricer Options)


# My Tests

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

add_executable (test_unit_deltas test_unit_deltas.cpp)
target_link_libraries(test_unit_deltas jl_parser ${LIBS})
add_test(test_unit_deltas test_unit_deltas)

add_executable (test_unit_pnl test_unit_pnl.cpp)
target_link_libraries(test_unit_pnl jl_parser ${LIBS})
add_test(test_unit_pnl test_unit_pnl)

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


#############################################
################## PEPS  ####################
#############################################

add_executable (test_kozei test_kozei.cpp)
target_link_libraries(test_kozei jl_parser ${LIBS})
add_test(test_kozei test_kozei)

add_executable (test_dataselecter test_dataSelecter.cpp)
target_link_libraries(test_dataselecter jl_parser ${LIBS})
add_test(test_dataselecter test_dataselecter)
