cmake_minimum_required(VERSION 3.22)
project(TradeMatchingEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)  # Output directory for libraries
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)  # Output directory for executables

# Define the static library
add_library(trade_engine STATIC 
    ${SRC_DIR}/TradeEngine.hpp
    ${SRC_DIR}/TradeEngine.cc
)

# Set the output directory and custom library name
set_target_properties(trade_engine PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR}
    OUTPUT_NAME "tradeengine"  # Results in libmytrade.a
)

target_include_directories(trade_engine PUBLIC ${SRC_DIR})

set(SOURCE_FILES
    ${SRC_DIR}/main.cc
)

# Clang-format setup
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    message(STATUS "clang-format found at: ${CLANG_FORMAT}")
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${SOURCE_FILES} ${SRC_DIR}/TradeEngine.hpp ${CMAKE_SOURCE_DIR}/tests/test_trading_engine.cpp
        COMMENT "Running clang-format on source files"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
else()
    message(WARNING "clang-format not found, skipping formatting.")
endif()

# Define the executable
add_executable(TradeMatchingEngine ${SOURCE_FILES})
target_link_libraries(TradeMatchingEngine PRIVATE trade_engine)

# Set the output directory for the executable
set_target_properties(TradeMatchingEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

# Clang-tidy setup (commented out as in original)
find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy)
if(NOT CLANG_TIDY_EXECUTABLE)
    message(WARNING "clang-tidy not found, skipping checks.")
endif()

# Uncomment and adjust if needed
# if(CMAKE_BUILD_TYPE MATCHES Debug AND CLANG_TIDY_EXECUTABLE)
#     add_custom_target(clang-tidy-check
#         COMMAND ${CLANG_TIDY_EXECUTABLE}
#             -checks=-*,modernize-use-override,-warnings-as-errors
#             -fix
#             -format-style=file
#             -p ${CMAKE_BINARY_DIR}
#             ${SOURCE_FILES} ${SRC_DIR}/TradeEngine.hpp ${CMAKE_SOURCE_DIR}/tests/test_trading_engine.cpp
#         COMMENT "Running clang-tidy checks"
#         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#     )
#     add_dependencies(TradeMatchingEngine clang-tidy-check)
# endif()

add_custom_target(clang-format-check
    COMMAND ${CLANG_FORMAT} -n --Werror ${SOURCE_FILES} ${SRC_DIR}/TradeEngine.hpp ${CMAKE_SOURCE_DIR}/tests/test_trading_engine.cpp
    COMMENT "Checking clang-format compliance"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

enable_testing()
add_subdirectory(tests)