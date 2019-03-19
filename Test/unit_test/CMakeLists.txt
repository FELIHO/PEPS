include_directories(.)


#############################################
################## PEPS  ####################
#############################################

############## test unitaire ################

add_executable(tools tools.cpp)
target_link_libraries(tools ${LIBS} ${GTEST_LIBRARIES} pthread)

add_executable(blackScholesModel blackScholesModel.cpp)
target_link_libraries(blackScholesModel ${LIBS} ${GTEST_LIBRARIES} pthread)

add_executable(price price.cpp)
target_link_libraries(price jl_parser ${LIBS} ${GTEST_LIBRARIES} pthread)

add_executable(delta delta.cpp)
target_link_libraries(delta jl_parser ${LIBS} ${GTEST_LIBRARIES} pthread)