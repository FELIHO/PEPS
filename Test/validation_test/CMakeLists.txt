include_directories(.)


#############################################
################## PEPS #####################
#############################################

############ test de validation #############

add_executable (PL_rebal_Call PL_rebal_Call.cpp)
target_link_libraries(PL_rebal_Call ${LIBS})

add_executable(correlation_and_covariance_test correlation_and_covariance_test.cpp)
target_link_libraries(correlation_and_covariance_test ${LIBS} ${GTEST_LIBRARIES} pthread)


add_executable (PL_rebal_Asian PL_rebal_Asian.cpp)
target_link_libraries(PL_rebal_Asian ${LIBS})

#add_executable (PL_rebal_Kozei PL_rebal_Kozei.cpp)
#target_link_libraries(PL_rebal_Kozei ${LIBS})


