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
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
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

# Add math lib
set(LIBS ${LIBS} m Pricer Options)

# Add jlparser
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_subdirectory(3rdparty/jlparser)

#set(CMAKE_CXX_FLAGS "-g -Wall -fdeclspec" )

# Test the parser
# add_executable(test-parser src/test_parser.cpp)
# target_link_libraries(test-parser jl_parser ${LIBS})

# add_executable(mc-example src/mc_example.cpp)
# target_link_libraries(mc-example jl_parser ${LIBS})

# Executable Product
add_executable(pricer src/pricer.cpp)
target_link_libraries(pricer jl_parser ${LIBS})

# For DLL, but unrecognised in ENSIMAG's machines
# set(CMAKE_CXX_FLAGS "-g -Wall -fdeclspec" )

# Print compiler flags
get_directory_property(DirDefs COMPILE_DEFINITIONS)
message(STATUS "COMPILE_DEFINITIONS = ${DirDefs}")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_LINKER_EXE_FLAGS}" )
else()
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_LINKER_EXE_FLAGS}" )
endif()

# Include
include_directories(include include/AssetModel include/Delta include/Hedge include/InterestModel include/MonteCarlo include/Option) # include/Rnd )


# Adding our own libraries
add_library(Pricer src/AssetModel/BlackScholesModel.cpp include/AssetModel/BlackScholesModel.hpp src/AssetModel/BlackScholesMertonModel.cpp include/AssetModel/BlackScholesMertonModel.hpp src/MonteCarlo/MonteCarlo.cpp include/MonteCarlo/MonteCarlo.hpp src/Hedge/HedgePortfolio.cpp include/Hedge/HedgePortfolio.hpp src/Delta/Delta.cpp include/Delta/Delta.hpp src/Delta/DeltaDF.cpp include/Delta/DeltaDF.hpp src/Delta/DeltaLH.cpp include/Delta/DeltaLH.hpp src/Delta/DeltaPW.cpp include/Delta/DeltaPW.hpp ) # src/Rnd/FakeRnd.cpp include/Rnd/FakeRnd.hpp src/Rnd/PnlRand.cpp include/Rnd/PnlRand.hpp)
add_library(Options src/Option/Option.cpp include/Option/Option.hpp src/Option/Call.cpp include/Option/Call.hpp src/Option/Basket.cpp include/Option/Basket.hpp src/Option/Asian.cpp include/Option/Asian.hpp src/Option/Performance.cpp include/Option/Performance.hpp)

# Adding subdirectories
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(Test)
# add_subdirectory(doc)
enable_testing()
