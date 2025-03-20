#include "TradeEngine.hpp"

#include <iostream>
#include <cstring>
#include <array>

using namespace Trading;

enum class TestResult
{
    PASSED,
    NOT_COMPLETED,
    FAILED
};

template <std::size_t N, std::size_t M>
TestResult validateTransactions(TradeEngine& engine,
                                const std::array<std::string, N>& transactions,
                                const std::array<std::string, M>& expected,
                                const std::string& testName)
{
    for (const auto& s : transactions)
    {
        auto tradeOrder = engine.ParseInput(s);
        engine.ProcessOrder(tradeOrder);
    }

    decltype(auto) trades = engine.GetTrades();
    if (trades.size() != expected.size())
    {
        std::cerr << testName << " result differs from expected result: "
                  << "Expected " << expected.size() << " trades, got "
                  << trades.size() << std::endl;
        return TestResult::FAILED;
    }

    for (std::size_t i = 0; i < trades.size(); ++i)
    {
        if (trades[i] != expected[i])
        {
            std::cerr << testName << " mismatch. Expected: " << expected[i]
                      << ", Got: " << trades[i] << std::endl;
            return TestResult::FAILED;
        }
    }
    std::cerr << testName << " - Passed\n";
    return TestResult::PASSED;
}

TestResult testParseInputValid()
{
    TradeEngine engine;
    decltype(auto) order = engine.ParseInput("T1 B 5 100");
    engine.ProcessOrder(order);
    decltype(auto) buyOrders = engine.GetBuyOrders();

    if (buyOrders.size() != 1)
    {
        std::cerr << "ParseInput: Valid input failed - Expected 1 price level, got "
                  << buyOrders.size() << std::endl;
        return TestResult::FAILED;
    }
    if (buyOrders.at(100).size() != 1)
    {
        std::cerr << "ParseInput: Valid input failed - Expected 1 order at price 100, got "
                  << buyOrders.at(100).size() << std::endl;
        return TestResult::FAILED;
    }
    std::cout << "ParseInput: Valid input - Passed\n";
    return TestResult::PASSED;
}

TestResult testHandleSellEmptyBook()
{
    TradeEngine engine;
    engine.ProcessOrder(engine.ParseInput("user1 S 10 100"));
    decltype(auto) sellOrders = engine.GetSellOrders();

    if (sellOrders.size() != 1)
    {
        std::cerr << "HandleSell: Empty book failed - Expected 1 price level, got "
                  << sellOrders.size() << std::endl;
        return TestResult::FAILED;
    }
    if (sellOrders.at(100).empty())
    {
        std::cerr << "HandleSell: Empty book failed - No orders at price 100\n";
        return TestResult::FAILED;
    }
    if (sellOrders.at(100).front().quantity != 10)
    {
        std::cerr << "HandleSell: Empty book failed - Expected quantity 10, got "
                  << sellOrders.at(100).front().quantity << std::endl;
        return TestResult::FAILED;
    }
    if (sellOrders.at(100).front().identifier != "user1")
    {
        std::cerr << "HandleSell: Empty book failed - Expected identifier 'user1', got '"
                  << sellOrders.at(100).front().identifier << "'\n";
        return TestResult::FAILED;
    }
    std::cout << "HandleSell: Empty book - Passed\n";
    return TestResult::PASSED;
}

TestResult testHandleSellFullMatch()
{
    TradeEngine engine;
    engine.ProcessOrder(engine.ParseInput("T1 B 5 30"));
    engine.ProcessOrder(engine.ParseInput("T2 S 5 30"));
    decltype(auto) buyOrders = engine.GetBuyOrders();
    decltype(auto) sellOrders = engine.GetSellOrders();
    decltype(auto) trades = engine.GetTrades();

    if (!buyOrders.empty())
    {
        std::cerr << "HandleSell: Full match failed - Buy orders not empty\n";
        return TestResult::FAILED;
    }
    if (!sellOrders.empty())
    {
        std::cerr << "HandleSell: Full match failed - Sell orders not empty\n";
        return TestResult::FAILED;
    }
    if (trades.size() != 1)
    {
        std::cerr << "HandleSell: Full match failed - Expected 1 trade, got "
                  << trades.size() << std::endl;
        return TestResult::FAILED;
    }
    if (trades[0] != "T1+5@30 T2-5@30")
    {
        std::cerr << "HandleSell: Full match failed - Expected trade 'T1+5@30 T2-5@30', got '"
                  << trades[0] << "'\n";
        return TestResult::FAILED;
    }
    std::cout << "HandleSell: Full match - Passed\n";
    return TestResult::PASSED;
}

TestResult testProcessOrderBuy()
{
    TradeEngine engine;
    engine.ProcessOrder(engine.ParseInput("user1 B 10 100"));
    decltype(auto) buyOrders = engine.GetBuyOrders();

    if (buyOrders.size() != 1)
    {
        std::cerr << "ProcessOrder: Buy failed - Expected 1 price level, got "
                  << buyOrders.size() << std::endl;
        return TestResult::FAILED;
    }
    if (buyOrders.at(100).empty())
    {
        std::cerr << "ProcessOrder: Buy failed - No orders at price 100\n";
        return TestResult::FAILED;
    }
    if (buyOrders.at(100).front().quantity != 10)
    {
        std::cerr << "ProcessOrder: Buy failed - Expected quantity 10, got "
                  << buyOrders.at(100).front().quantity << std::endl;
        return TestResult::FAILED;
    }
    std::cout << "ProcessOrder: Buy - Passed\n";
    return TestResult::PASSED;
}

TestResult testProcessOrderSellMatch()
{
    TradeEngine engine;
    engine.ProcessOrder(engine.ParseInput("user1 B 5 100"));
    engine.ProcessOrder(engine.ParseInput("user2 S 5 100"));
    decltype(auto) buyOrders = engine.GetBuyOrders();
    decltype(auto) sellOrders = engine.GetSellOrders();
    decltype(auto) trades = engine.GetTrades();

    if (!buyOrders.empty())
    {
        std::cerr << "ProcessOrder: Sell with match failed - Buy orders not empty\n";
        return TestResult::FAILED;
    }
    if (!sellOrders.empty())
    {
        std::cerr << "ProcessOrder: Sell with match failed - Sell orders not empty\n";
        return TestResult::FAILED;
    }
    if (trades.size() != 1)
    {
        std::cerr << "ProcessOrder: Sell with match failed - Expected 1 trade, got "
                  << trades.size() << std::endl;
        return TestResult::FAILED;
    }
    std::cout << "ProcessOrder: Sell with match - Passed\n";
    return TestResult::PASSED;
}

TestResult testCollectTrades()
{
    TradeEngine engine;
    engine.ProcessOrder(engine.ParseInput("user1 B 5 100"));
    engine.ProcessOrder(engine.ParseInput("user2 S 5 100"));
    decltype(auto) trades = engine.GetTrades();

    if (trades.size() != 1)
    {
        std::cerr << "CollectTrades: Basic trade failed - Expected 1 trade, got "
                  << trades.size() << std::endl;
        return TestResult::FAILED;
    }
    if (trades[0] != "user1+5@100 user2-5@100")
    {
        std::cerr << "CollectTrades: Basic trade failed - Expected trade 'user1+5@100 user2-5@100', got '"
                  << trades[0] << std::endl;
        return TestResult::FAILED;
    }
    std::cout << "CollectTrades: Basic trade - Passed\n";
    return TestResult::PASSED;
}

TestResult testGetOrdersEmpty()
{
    TradeEngine engine;
    decltype(auto) buyOrders = engine.GetBuyOrders();
    decltype(auto) sellOrders = engine.GetSellOrders();
    decltype(auto) trades = engine.GetTrades();

    if (!buyOrders.empty())
    {
        std::cerr << "GetOrders: Empty state failed - Buy orders not empty\n";
        return TestResult::FAILED;
    }
    if (!sellOrders.empty())
    {
        std::cerr << "GetOrders: Empty state failed - Sell orders not empty\n";
        return TestResult::FAILED;
    }
    if (!trades.empty())
    {
        std::cerr << "GetOrders: Empty state failed - Trades not empty\n";
        return TestResult::FAILED;
    }
    std::cout << "GetOrders: Empty state - Passed\n";
    return TestResult::PASSED;
}

TestResult testHandleBuyFullMatch()
{
    TradeEngine engine;
    engine.ProcessOrder(engine.ParseInput("T7 S 1 50"));
    engine.ProcessOrder(engine.ParseInput("T1 B 1 50"));
    decltype(auto) buyOrders = engine.GetBuyOrders();
    decltype(auto) sellOrders = engine.GetSellOrders();
    decltype(auto) trades = engine.GetTrades();

    if (!buyOrders.empty())
    {
        std::cerr << "HandleBuy: Full match failed - Buy orders not empty\n";
        return TestResult::FAILED;
    }
    if (!sellOrders.empty())
    {
        std::cerr << "HandleBuy: Full match failed - Sell orders not empty\n";
        return TestResult::FAILED;
    }
    if (trades.size() != 1)
    {
        std::cerr << "HandleBuy: Full match failed - Expected 1 trade, got "
                  << trades.size() << std::endl;
        return TestResult::FAILED;
    }
    if (trades[0] != "T1+1@50 T7-1@50")
    {
        std::cerr << "HandleBuy: Full match failed - Expected trade 'T1+1@50 T7-1@50', got '"
                  << trades[0] << "'\n";
        return TestResult::FAILED;
    }
    std::cout << "HandleBuy: Full match - Passed\n";
    return TestResult::PASSED;
}

TestResult testTransactions1()
{
    TradeEngine engine;
    std::array<std::string, 12> transactions = {
        "T1 B 5 30", "T2 S 5 70",  "T3 B 1 40", "T4 S 2 60",
        "T5 S 3 70", "T6 S 20 80", "T7 S 1 50", "T2 S 5 70",
        "T1 B 1 50", "T1 B 3 60",  "T7 S 2 50", "T8 B 10 90"};
    std::array<std::string, 4> expected = {
        "T1+1@50 T7-1@50", "T1+2@60 T4-2@60", "T1+1@60 T7-1@60",
        "T2-6@70 T5-3@70 T7-1@50 T8+1@50 T8+9@70"};
    return validateTransactions(engine, transactions, expected, "Transactions_1");
}

TestResult testTransactions2()
{
    TradeEngine engine;
    std::array<std::string, 8> transactions = {
        "T1 B 5 100", "T2 S 3 90",  "T3 S 4 95",  "T4 B 6 105",
        "T5 S 5 100", "T6 B 2 100", "T7 S 3 105", "T8 B 4 110"};
    std::array<std::string, 6> expected = {
        "T1+3@100 T2-3@100", "T1+2@100 T3-2@100", "T3-2@95 T4+2@95",
        "T4+4@105 T5-4@105", "T5-1@100 T6+1@100", "T7-3@105 T8+3@105"};
    return validateTransactions(engine, transactions, expected, "Transactions_2");
}

TestResult testTransactions3()
{
    TradeEngine engine;
    std::array<std::string, 10> transactions = {
        "T1 B 5 100", "T2 S 3 90",  "T3 S 4 95",  "T4 B 6 105",
        "T5 S 5 100", "T6 B 2 100", "T7 S 3 105", "T8 B 4 110",
        "T9 S 2 115", "T10 B 3 120"};
    std::array<std::string, 7> expected = {
        "T1+3@100 T2-3@100", "T1+2@100 T3-2@100", "T3-2@95 T4+2@95",
        "T4+4@105 T5-4@105", "T5-1@100 T6+1@100", "T7-3@105 T8+3@105",
        "T10+2@115 T9-2@115"};
    return validateTransactions(engine, transactions, expected, "Transactions_3");
}

void runTest(TestResult (*testFunc)(), const std::string& testName, int& passedCount, int& notCompletedCount, int& failedCount, int& totalCount)
{
    totalCount++;
    std::cerr << "Running " << testName << "... ";
    TestResult result = testFunc();
    if (result == TestResult::PASSED)
    {
        ++passedCount;
        std::cout << "Passed " << testName << std::endl;
    }
    else if (result == TestResult::NOT_COMPLETED)
    {
        ++notCompletedCount;
        std::cout << "Test failed to complete " << testName << std::endl;
    }
    else if (result == TestResult::FAILED)
    {
        ++failedCount;
        std::cout << "Failed " << testName << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <test_name>\n";
        return 1;
    }

    std::string testName = argv[1];
    int passed_tests = 0;
    int not_completed_tests = 0;
    int failed_tests = 0;
    int total_tests = 0;

    if (testName == "testParseInputValid")
        runTest(testParseInputValid, testName, passed_tests, not_completed_tests, failed_tests, total_tests);
    else if (testName == "testHandleSellEmptyBook")
        runTest(testHandleSellEmptyBook, testName, passed_tests, not_completed_tests, failed_tests, total_tests);
    else if (testName == "testHandleSellFullMatch")
        runTest(testHandleSellFullMatch, testName, passed_tests, not_completed_tests, failed_tests, total_tests);
    else if (testName == "testProcessOrderBuy")
        runTest(testProcessOrderBuy, testName, passed_tests, not_completed_tests, failed_tests, total_tests);
    else if (testName == "testProcessOrderSellMatch")
        runTest(testProcessOrderSellMatch, testName, passed_tests, not_completed_tests, failed_tests, total_tests);
    else if (testName == "testCollectTrades")
        runTest(testCollectTrades, testName, passed_tests, not_completed_tests, failed_tests, total_tests);
    else if (testName == "testGetOrdersEmpty")
        runTest(testGetOrdersEmpty, testName, passed_tests, not_completed_tests, failed_tests, total_tests);
    else if (testName == "testHandleBuyFullMatch")
        runTest(testHandleBuyFullMatch, testName, passed_tests, not_completed_tests, failed_tests, total_tests);
    else if (testName == "testTransactions1")
        runTest(testTransactions1, testName, passed_tests, not_completed_tests, failed_tests, total_tests);
    else if (testName == "testTransactions2")
        runTest(testTransactions2, testName, passed_tests, not_completed_tests, failed_tests, total_tests);
    else if (testName == "testTransactions3")
        runTest(testTransactions3, testName, passed_tests, not_completed_tests, failed_tests, total_tests);
    else
    {
        std::cerr << "Unknown test: " << testName << "\n";
        return 1;
    }

    return (passed_tests == 1 && failed_tests == 0 && not_completed_tests == 0) ? 0 : 1;
}