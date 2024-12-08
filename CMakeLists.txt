cmake_minimum_required(VERSION 3.12)
project(options_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(/mnt/c/Users/matte/Documents/eigen-3.4.0/eigen-3.4.0)

#find_package(SQLite3 REQUIRED)

#set(EXAMPLES
#        examples/1-price_european_options.cpp
#        examples/2-implied_volatility.cpp
#        examples/3-model_calibration.cpp
#)

# Add your source files here
set(SOURCES
        #src/portfolio-backtester.cpp
        #src/ETL.cpp
        #src/ETL.cpp
        #src/option_mc.cpp
        #src/black_scholes.hpp
        # Header Files
#        headers/options.h
#        headers/options_european.h
#        headers/options_american.h
#        headers/rapidcsv.h
#        headers/pricing_models.h
#        headers/greeks.h
#        headers/date.h
#        headers/calendar.h
        #headers/pde.h
        #headers/solve.h
        # Source Files
#        src/date.cpp
#        src/calendar.cpp
#        src/options.cpp
#        src/pricing_models.cpp
        #src/pde.cpp
        #src/solve.cpp
        main.cpp
)

#if(NOT EXAMPLE_TO_COMPILE)
#    set(EXAMPLE_TO_COMPILE main.cpp) #default
#endif()

#list(APPEND SOURCES ${EXAMPLE_TO_COMPILE})

# Set the output directory for the compiled binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Link the libraries
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
#target_link_libraries(${PROJECT_NAME} SQLite::SQLite3)
#target_link_libraries(${PROJECT_NAME} PRIVATE /mnt/c/Users/matte/Documents/eigen-3.4.0/eigen-3.4.0)