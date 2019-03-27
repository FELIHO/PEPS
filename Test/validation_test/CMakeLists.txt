include_directories(.)


#############################################
################## PEPS #####################
############ test de validation #############
#############################################


############ Parameters estimation ###########

#add_executable(correlation_and_covariance_test correlation_and_covariance_test.cpp)
#target_link_libraries(correlation_and_covariance_test ${LIBS} ${GTEST_LIBRARIES} pthread)



############ Price validation ###############

add_executable(price_from_files price_from_files.cpp)
target_link_libraries(price_from_files jl_parser ${LIBS} ${GTEST_LIBRARIES} pthread)

add_executable (price_call_formuleBS price_call_formuleBS.cpp)
target_link_libraries(price_call_formuleBS ${LIBS})



####### Profit and loss validation ########

add_executable(delta_from_files delta_from_files.cpp)
target_link_libraries(delta_from_files jl_parser ${LIBS} ${GTEST_LIBRARIES} pthread)

add_executable (PL_rebal_Call PL_rebal_Call.cpp)
target_link_libraries(PL_rebal_Call ${LIBS})

add_executable (PL_rebal_Asian PL_rebal_Asian.cpp)
target_link_libraries(PL_rebal_Asian ${LIBS})

add_executable (PL_rebal_Kozei PL_rebal_Kozei.cpp)
target_link_libraries(PL_rebal_Kozei ${LIBS})

add_executable (PL_histo_Call PL_histo_Call.cpp)
target_link_libraries(PL_histo_Call ${LIBS})

add_executable (PL_histo_Asian PL_histo_Asian.cpp)
target_link_libraries(PL_histo_Asian ${LIBS})

add_executable (PL_histo_Kozei PL_histo_Kozei.cpp)
target_link_libraries(PL_histo_Kozei ${LIBS})
