include_directories(.)


#############################################
################## PEPS #####################
#############################################

############ test de validation #############


add_executable(correlation_and_covariance_test correlation_and_covariance_test.cpp)
target_link_libraries(correlation_and_covariance_test ${LIBS} ${GTEST_LIBRARIES} pthread)






