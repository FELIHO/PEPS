include_directories(.)


#############################################
################## PEPS  ####################
#############################################

############# test de mémoire ###############

add_executable(resize resize.cpp)
target_link_libraries(resize ${LIBS})

add_executable (asset_t0 asset_t0.cpp)
target_link_libraries(asset_t0 ${LIBS})

add_executable (asset_t asset_t.cpp)
target_link_libraries(asset_t ${LIBS})

add_executable (asset_ti asset_ti.cpp)
target_link_libraries(asset_ti ${LIBS})

add_executable (shift shift.cpp)
target_link_libraries(shift ${LIBS})

add_executable (price_t0 price_t0.cpp)
target_link_libraries(price_t0 ${LIBS})

add_executable (price_t price_t.cpp)
target_link_libraries(price_t ${LIBS})

add_executable (price_ti price_ti.cpp)
target_link_libraries(price_ti ${LIBS})

add_executable (delta_t delta_t.cpp)
target_link_libraries(delta_t ${LIBS})

add_executable (delta_ti delta_ti.cpp)
target_link_libraries(delta_ti ${LIBS})

add_executable (Kozei Kozei.cpp)
target_link_libraries(Kozei ${LIBS})
