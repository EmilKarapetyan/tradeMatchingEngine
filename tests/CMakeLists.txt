cmake_minimum_required(VERSION 3.22)

add_executable(TradeMatchingEngineTests
    test_trading_engine.cpp
)

target_link_libraries(TradeMatchingEngineTests PRIVATE trade_engine)

set_target_properties(TradeMatchingEngineTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_test(NAME TestParseInputValid COMMAND TradeMatchingEngineTests "testParseInputValid")
add_test(NAME TestHandleSellEmptyBook COMMAND TradeMatchingEngineTests "testHandleSellEmptyBook")
add_test(NAME TestHandleSellFullMatch COMMAND TradeMatchingEngineTests "testHandleSellFullMatch")
add_test(NAME TestProcessOrderBuy COMMAND TradeMatchingEngineTests "testProcessOrderBuy")
add_test(NAME TestProcessOrderSellMatch COMMAND TradeMatchingEngineTests "testProcessOrderSellMatch")
add_test(NAME TestCollectTrades COMMAND TradeMatchingEngineTests "testCollectTrades")
add_test(NAME TestGetOrdersEmpty COMMAND TradeMatchingEngineTests "testGetOrdersEmpty")
add_test(NAME TestHandleBuyFullMatch COMMAND TradeMatchingEngineTests "testHandleBuyFullMatch")
add_test(NAME TestTransactions1 COMMAND TradeMatchingEngineTests "testTransactions1")
add_test(NAME TestTransactions2 COMMAND TradeMatchingEngineTests "testTransactions2")
add_test(NAME TestTransactions3 COMMAND TradeMatchingEngineTests "testTransactions3")