#include <array>
#include <iostream>

#include "TradeEngine.hpp"

using namespace Trading;

template <size_t N, size_t M>
bool validateTransactions(TradeEngine& engine,
                          const std::array<std::string, N>& transactions,
                          const std::array<std::string, M>& expected,
                          const char* testName)
{
    for (const auto& s : transactions)
    {
        auto tradeOrder = engine.ParseInput(s);
        engine.ProcessOrder(tradeOrder);
    }

    const auto& trades = engine.GetTrades();
    if (trades.size() != expected.size())
    {
        std::cerr << testName << " result differs from expected result: "
                  << "Expected " << expected.size() << " trades, got "
                  << trades.size() << std::endl;
        return false;
    }

    for (size_t i = 0; i < trades.size(); ++i)
    {
        if (trades[i] != expected[i])
        {
            std::cerr << testName << " mismatch. Expected: " << expected[i]
                      << ", Got: " << trades[i] << std::endl;
            return false;
        }
    }
    std::cerr << testName << " - Passed\n";
    return true;
}

bool testParseInputValid()
{
    TradeEngine engine;
    auto order = engine.ParseInput("T1 B 5 100");
    engine.ProcessOrder(order);
    const auto& buyOrders = engine.GetBuyOrders();

    if (buyOrders.size() != 1)
    {
        std::cerr
            << "ParseInput: Valid input failed - Expected 1 price level, got "
            << buyOrders.size() << std::endl;
        return false;
    }
    if (buyOrders.at(100).size() != 1)
    {
        std::cerr << "ParseInput: Valid input failed - Expected 1 order at "
                     "price 100, got "
                  << buyOrders.at(100).size() << std::endl;
        return false;
    }
    std::cout << "ParseInput: Valid input - Passed\n";
    return true;
}

bool testHandleSellEmptyBook()
{
    TradeEngine engine;
    engine.ProcessOrder(engine.ParseInput("user1 S 10 100"));
    const auto& sellOrders = engine.GetSellOrders();

    if (sellOrders.size() != 1)
    {
        std::cerr
            << "HandleSell: Empty book failed - Expected 1 price level, got "
            << sellOrders.size() << std::endl;
        return false;
    }
    if (sellOrders.at(100).empty())
    {
        std::cerr << "HandleSell: Empty book failed - No orders at price 100\n";
        return false;
    }
    if (sellOrders.at(100).front().quantity != 10)
    {
        std::cerr
            << "HandleSell: Empty book failed - Expected quantity 10, got "
            << sellOrders.at(100).front().quantity << std::endl;
        return false;
    }
    if (sellOrders.at(100).front().identifier != "user1")
    {
        std::cerr << "HandleSell: Empty book failed - Expected identifier "
                     "'user1', got '"
                  << sellOrders.at(100).front().identifier << "'\n";
        return false;
    }
    std::cout << "HandleSell: Empty book - Passed\n";
    return true;
}

bool testHandleSellFullMatch()
{
    TradeEngine engine;
    engine.ProcessOrder(engine.ParseInput("T1 B 5 30"));
    engine.ProcessOrder(engine.ParseInput("T2 S 5 30"));
    const auto& buyOrders = engine.GetBuyOrders();
    const auto& sellOrders = engine.GetSellOrders();
    const auto& trades = engine.GetTrades();

    if (!buyOrders.empty())
    {
        std::cerr << "HandleSell: Full match failed - Buy orders not empty\n";
        return false;
    }
    if (!sellOrders.empty())
    {
        std::cerr << "HandleSell: Full match failed - Sell orders not empty\n";
        return false;
    }
    if (trades.size() != 1)
    {
        std::cerr << "HandleSell: Full match failed - Expected 1 trade, got "
                  << trades.size() << std::endl;
        return false;
    }
    if (trades[0] != "T1+5@30 T2-5@30")
    {
        std::cerr << "HandleSell: Full match failed - Expected trade 'T1+5@30 "
                     "T2-5@30', got '"
                  << trades[0] << "'\n";
        return false;
    }
    std::cout << "HandleSell: Full match - Passed\n";
    return true;
}

bool testProcessOrderBuy()
{
    TradeEngine engine;
    engine.ProcessOrder(engine.ParseInput("user1 B 10 100"));
    const auto& buyOrders = engine.GetBuyOrders();

    if (buyOrders.size() != 1)
    {
        std::cerr << "ProcessOrder: Buy failed - Expected 1 price level, got "
                  << buyOrders.size() << std::endl;
        return false;
    }
    if (buyOrders.at(100).empty())
    {
        std::cerr << "ProcessOrder: Buy failed - No orders at price 100\n";
        return false;
    }
    if (buyOrders.at(100).front().quantity != 10)
    {
        std::cerr << "ProcessOrder: Buy failed - Expected quantity 10, got "
                  << buyOrders.at(100).front().quantity << std::endl;
        return false;
    }
    std::cout << "ProcessOrder: Buy - Passed\n";
    return true;
}

bool testProcessOrderSellMatch()
{
    TradeEngine engine;
    engine.ProcessOrder(engine.ParseInput("user1 B 5 100"));
    engine.ProcessOrder(engine.ParseInput("user2 S 5 100"));
    const auto& buyOrders = engine.GetBuyOrders();
    const auto& sellOrders = engine.GetSellOrders();
    const auto& trades = engine.GetTrades();

    if (!buyOrders.empty())
    {
        std::cerr
            << "ProcessOrder: Sell with match failed - Buy orders not empty\n";
        return false;
    }
    if (!sellOrders.empty())
    {
        std::cerr
            << "ProcessOrder: Sell with match failed - Sell orders not empty\n";
        return false;
    }
    if (trades.size() != 1)
    {
        std::cerr
            << "ProcessOrder: Sell with match failed - Expected 1 trade, got "
            << trades.size() << std::endl;
        return false;
    }
    std::cout << "ProcessOrder: Sell with match - Passed\n";
    return true;
}

bool testCollectTrades()
{
    TradeEngine engine;
    engine.ProcessOrder(engine.ParseInput("user1 B 5 100"));
    engine.ProcessOrder(engine.ParseInput("user2 S 5 100"));
    const auto& trades = engine.GetTrades();

    if (trades.size() != 1)
    {
        std::cerr
            << "CollectTrades: Basic trade failed - Expected 1 trade, got "
            << trades.size() << std::endl;
        return false;
    }
    if (trades[0] != "user1+5@100 user2-5@100")
    {
        std::cerr << "CollectTrades: Basic trade failed - Expected trade "
                     "'user1+5@100 user2-5@100', got '"
                  << trades[0] << std::endl;
        return false;
    }
    std::cout << "CollectTrades: Basic trade - Passed\n";
    return true;
}

bool testGetOrdersEmpty()
{
    TradeEngine engine;
    const auto& buyOrders = engine.GetBuyOrders();
    const auto& sellOrders = engine.GetSellOrders();
    const auto& trades = engine.GetTrades();

    if (!buyOrders.empty())
    {
        std::cerr << "GetOrders: Empty state failed - Buy orders not empty\n";
        return false;
    }
    if (!sellOrders.empty())
    {
        std::cerr << "GetOrders: Empty state failed - Sell orders not empty\n";
        return false;
    }
    if (!trades.empty())
    {
        std::cerr << "GetOrders: Empty state failed - Trades not empty\n";
        return false;
    }
    std::cout << "GetOrders: Empty state - Passed\n";
    return true;
}

bool testHandleBuyFullMatch()
{
    TradeEngine engine;
    engine.ProcessOrder(engine.ParseInput("T7 S 1 50"));
    engine.ProcessOrder(engine.ParseInput("T1 B 1 50"));
    const auto& buyOrders = engine.GetBuyOrders();
    const auto& sellOrders = engine.GetSellOrders();
    const auto& trades = engine.GetTrades();

    if (!buyOrders.empty())
    {
        std::cerr << "HandleBuy: Full match failed - Buy orders not empty\n";
        return false;
    }
    if (!sellOrders.empty())
    {
        std::cerr << "HandleBuy: Full match failed - Sell orders not empty\n";
        return false;
    }
    if (trades.size() != 1)
    {
        std::cerr << "HandleBuy: Full match failed - Expected 1 trade, got "
                  << trades.size() << std::endl;
        return false;
    }
    if (trades[0] != "T1+1@50 T7-1@50")
    {
        std::cerr << "HandleBuy: Full match failed - Expected trade 'T1+1@50 "
                     "T7-1@50', got '"
                  << trades[0] << std::endl;
        return false;
    }
    std::cout << "HandleBuy: Full match - Passed\n";
    return true;
}

bool validateTradeMatchingEngine()
{
    bool passed = true;

    passed &= testParseInputValid();
    passed &= testHandleSellEmptyBook();
    passed &= testHandleSellFullMatch();
    passed &= testProcessOrderBuy();
    passed &= testProcessOrderSellMatch();
    passed &= testCollectTrades();
    passed &= testGetOrdersEmpty();
    passed &= testHandleBuyFullMatch();

    {
        TradeEngine engine;
        std::array<std::string, 12> transactions_1 = {
            "T1 B 5 30", "T2 S 5 70",  "T3 B 1 40", "T4 S 2 60",
            "T5 S 3 70", "T6 S 20 80", "T7 S 1 50", "T2 S 5 70",
            "T1 B 1 50", "T1 B 3 60",  "T7 S 2 50", "T8 B 10 90"};
        std::array<std::string, 4> transactions_1_res = {
            "T1+1@50 T7-1@50", "T1+2@60 T4-2@60", "T1+1@60 T7-1@60",
            "T2-6@70 T5-3@70 T7-1@50 T8+1@50 T8+9@70"};
        passed &= validateTransactions(engine, transactions_1,
                                       transactions_1_res, "Transactions_1");
    }

    {
        TradeEngine engine;
        std::array<std::string, 8> transactions_2 = {
            "T1 B 5 100", "T2 S 3 90",  "T3 S 4 95",  "T4 B 6 105",
            "T5 S 5 100", "T6 B 2 100", "T7 S 3 105", "T8 B 4 110"};
        std::array<std::string, 6> transactions_2_res = {
            "T1+3@100 T2-3@100", "T1+2@100 T3-2@100", "T3-2@95 T4+2@95",
            "T4+4@105 T5-4@105", "T5-1@100 T6+1@100", "T7-3@105 T8+3@105"};
        passed &= validateTransactions(engine, transactions_2,
                                       transactions_2_res, "Transactions_2");
    }

    {
        TradeEngine engine;
        std::array<std::string, 10> transactions_3 = {
            "T1 B 5 100", "T2 S 3 90",  "T3 S 4 95",  "T4 B 6 105",
            "T5 S 5 100", "T6 B 2 100", "T7 S 3 105", "T8 B 4 110",
            "T9 S 2 115", "T10 B 3 120"};
        std::array<std::string, 7> transactions_3_res = {
            "T1+3@100 T2-3@100", "T1+2@100 T3-2@100", "T3-2@95 T4+2@95",
            "T4+4@105 T5-4@105", "T5-1@100 T6+1@100", "T7-3@105 T8+3@105",
            "T10+2@115 T9-2@115"};
        passed &= validateTransactions(engine, transactions_3,
                                       transactions_3_res, "Transactions_3");
    }

    return passed;
}

int main()
{
    std::cerr << "Running tests...\n";
    const bool allPassed = validateTradeMatchingEngine();
    if (allPassed)
    {
        std::cerr << "ALL TESTS PASSED\n";
    }
    else
    {
        std::cerr << "One or more tests failed\n";
    }
    return allPassed ? 0 : -1;
}