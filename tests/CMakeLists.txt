# Add test executable
add_executable(TradeMatchingEngineTests test_trading_engine.cpp)

# Set output directory for the test executable
set_target_properties(TradeMatchingEngineTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

# Link the test executable with the trade_engine library
target_link_libraries(TradeMatchingEngineTests PRIVATE trade_engine)

# Add the test to CTest
add_test(NAME TradeMatchingTest 
         COMMAND TradeMatchingEngineTests 
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
