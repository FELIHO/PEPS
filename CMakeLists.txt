cmake_minimum_required(VERSION 3.0)

project(mc-pricer CXX)

# Prefix
if (NOT PREFIX)
    set(PREFIX ${CMAKE_CURRENT_BINARY_DIR})
endif (NOT PREFIX)
set(CMAKE_INSTALL_PREFIX ${PREFIX} CACHE STRING "Installation prefix." FORCE)

if (CXX)
    set(CMAKE_CXX_COMPILER ${CXX})
endif (CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Release or Debug
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif ()

# Detect PNL
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_definitions(-DPNL_RANGE_CHECK_OFF)
endif()

# Detect Boost
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
message(STATUS "Boost include directory ${Boost_INCLUDE_DIR}")

# Detect OPM
FIND_PACKAGE(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_CXX_FLAGS ${CMAKE_CXX_CFLAGS} ${OpenMP_CXX_FLAGS})
    message(STATUS "OpenMP found.")
else (OPENMP_FOUND)
    message(STATUS "OpenMP not found.")
endif (OPENMP_FOUND)


# Add math lib
set(LIBS ${LIBS} m Pricer Options)

# Add jlparser
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_subdirectory(3rdparty/jlparser)

#set(CMAKE_CXX_FLAGS "-g -Wall -fdeclspec" )

# Executable Product
add_executable(pricer src/pricer.cpp)
target_link_libraries(pricer jl_parser ${LIBS})

# Executable Product
add_executable(forwardTest src/ForwardTest.cpp)
target_link_libraries(forwardTest ${LIBS})

# Executable Product
add_executable(backwardTest src/BackwardTest.cpp)
target_link_libraries(backwardTest ${LIBS})

# Print compiler flags
get_directory_property(DirDefs COMPILE_DEFINITIONS)
message(STATUS "COMPILE_DEFINITIONS = ${DirDefs}")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_LINKER_EXE_FLAGS}" )
else()
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_LINKER_EXE_FLAGS}" )
endif()

# Include
include_directories(include include/AssetModel include/Delta include/Hedge include/InterestModel include/MonteCarlo include/Option include/DataProvider include/Rnd )


# Adding our own libraries
add_library(Pricer src/AssetModel/BlackScholesModel.cpp include/AssetModel/BlackScholesModel.hpp src/MonteCarlo/MonteCarlo.cpp include/MonteCarlo/MonteCarlo.hpp src/Hedge/HedgePortfolio.cpp include/Hedge/HedgePortfolio.hpp include/Rnd/RandomGen.hpp src/Rnd/PnlRnd.cpp include/Rnd/PnlRnd.hpp src/Rnd/FakeRnd.cpp include/Rnd/FakeRnd.hpp src/Hedge/ParameterEstimation.cpp include/Hedge/ParameterEstimation.hpp include/DataProvider/DataProvider.hpp src/DataProvider/SimulatedDataProvider.cpp include/DataProvider/SimulatedDataProvider.hpp src/DataProvider/SemiHistoricalDataProvider.cpp include/DataProvider/SemiHistoricalDataProvider.hpp  src/Hedge/ParameterEstimation.cpp include/Hedge/ParameterEstimation.hpp) # src/Hedge/DataSelecter.cpp include/Hedge/DataSelecter.hpp)
add_library(Options include/Option/Option.hpp src/Option/Call.cpp include/Option/Call.hpp src/Option/Basket.cpp include/Option/Basket.hpp src/Option/Asian.cpp include/Option/Asian.hpp src/Option/Performance.cpp include/Option/Performance.hpp src/Option/Kozei.cpp include/Option/Kozei.hpp)

# src/AssetModel/BlackScholesMertonModel.cpp include/AssetModel/BlackScholesMertonModel.hpp src/Delta/Delta.cpp include/Delta/Delta.hpp src/Delta/DeltaDF.cpp include/Delta/DeltaDF.hpp src/Delta/DeltaLH.cpp include/Delta/DeltaLH.hpp src/Delta/DeltaPW.cpp include/Delta/DeltaPW.hpp )


# Adding subdirectories
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(Test)
# add_subdirectory(doc)
enable_testing()

