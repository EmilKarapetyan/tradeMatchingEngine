cmake_minimum_required(VERSION 3.22)
project(TradeMatchingEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

add_library(trade_engine STATIC 
    ${SRC_DIR}/TradeEngine.hpp
    ${SRC_DIR}/TradeEngine.cc
)

set_target_properties(trade_engine PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR}
    OUTPUT_NAME "tradeengine"
)

target_include_directories(trade_engine PUBLIC ${SRC_DIR})

set(SOURCE_FILES
    ${SRC_DIR}/main.cc
)

find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    message(STATUS "clang-format found at: ${CLANG_FORMAT}")
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${SOURCE_FILES} ${SRC_DIR}/TradeEngine.hpp ${CMAKE_SOURCE_DIR}/tests/test_trading_engine.cpp
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
else()
    message(WARNING "clang-format not found, skipping formatting.")
endif()

add_executable(TradeMatchingEngine ${SOURCE_FILES})
target_link_libraries(TradeMatchingEngine PRIVATE trade_engine)

set_target_properties(TradeMatchingEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy)
if(NOT CLANG_TIDY_EXECUTABLE)
    message(WARNING "clang-tidy not found, skipping checks.")
endif()

add_custom_target(clang-format-check
    COMMAND ${CLANG_FORMAT} -n --Werror ${SOURCE_FILES} ${SRC_DIR}/TradeEngine.hpp ${CMAKE_SOURCE_DIR}/tests/test_trading_engine.cpp
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

enable_testing()
add_subdirectory(tests)