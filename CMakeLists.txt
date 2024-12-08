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
        headers/date.h
        headers/calendar.h
        headers/options.h
        headers/black_scholes.h
        headers/math.h
        headers/payoff.h

        # Source Files
        sources/date.cpp
        sources/calendar.cpp
        sources/options.cpp
        sources/black_scholes.cpp
        sources/math.cpp

        main.cpp
)

set(TESTING
        headers/date.h
        headers/calendar.h
        headers/options.h
        headers/black_scholes.h
        headers/math.h

        sources/date.cpp
        sources/calendar.cpp
        sources/options.cpp
        sources/black_scholes.cpp
        sources/math.cpp

        unit_tests/test_framework.h
        testing.cpp
)

#if(NOT EXAMPLE_TO_COMPILE)
#    set(EXAMPLE_TO_COMPILE main.cpp) #default
#endif()

#list(APPEND SOURCES ${EXAMPLE_TO_COMPILE})

# Set the output directory for the compiled binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCES})
add_executable(test testing.cpp ${TESTING})

# Link the libraries
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
#target_link_libraries(${PROJECT_NAME} SQLite::SQLite3)
#target_link_libraries(${PROJECT_NAME} PRIVATE /mnt/c/Users/matte/Documents/eigen-3.4.0/eigen-3.4.0)