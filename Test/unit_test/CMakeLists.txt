include_directories(.)


#############################################
################## PEPS  ####################
############## test unitaire ################
#############################################


add_executable(tools tools.cpp)
target_link_libraries(tools ${LIBS} ${GTEST_LIBRARIES} pthread)

add_executable(blackScholesModel blackScholesModel.cpp)
target_link_libraries(blackScholesModel ${LIBS} ${GTEST_LIBRARIES} pthread)

add_executable(Computations Computations.cpp)
target_link_libraries(Computations ${LIBS})