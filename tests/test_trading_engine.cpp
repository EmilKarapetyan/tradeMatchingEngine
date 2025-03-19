#include "TradeEngine.hpp"
#include <array>
#include <iostream>

using namespace Trading;

bool validateTradeMatchingEngine()
{
    {
        TradeEngine engine;

        std::array<std::string, 12> transactions_1 = {
            "T1 B 5 30", "T2 S 5 70",  "T3 B 1 40", "T4 S 2 60",
            "T5 S 3 70", "T6 S 20 80", "T7 S 1 50", "T2 S 5 70",
            "T1 B 1 50", "T1 B 3 60",  "T7 S 2 50", "T8 B 10 90"};

        std::array<std::string, 4> transactions_1_res = {
            "T1+1@50 T7-1@50",
            "T1+2@60 T4-2@60",
            "T1+1@60 T7-1@60",
            "T2-6@70 T5-3@70 T7-1@50 T8+1@50 T8+9@70"};

        for (const auto& s : transactions_1) {
            auto tradeOrder = engine.ParseInput(s);
            engine.ProcessOrder(tradeOrder);
        }

        const auto& trades = engine.GetTrades();
        std::cout<<std::endl;
        if (trades.size() != transactions_1_res.size()) {
            std::cerr << "Transactions_1 result differs from expected result: "
                    << "Expected " << transactions_1_res.size() << " trades, got " << trades.size()
                    << std::endl;
            return false;
        }

        for (size_t i = 0; i < trades.size(); ++i) {
            if (trades[i] != transactions_1_res[i]) {
                std::cerr << "Transactions_1 mismatch. Expected: " << transactions_1_res[i]
                        << ", Got: " << trades[i] << std::endl;
                return false;
            }
        }
    }

    {
        TradeEngine engine;

        std::array<std::string, 8> transactions_2 = {
            "T1 B 5 100", "T2 S 3 90",  "T3 S 4 95",  "T4 B 6 105",
            "T5 S 5 100", "T6 B 2 100", "T7 S 3 105", "T8 B 4 110"};

        std::array<std::string, 6> transactions_2_res = {
            "T1+3@100 T2-3@100",
            "T1+2@100 T3-2@100",
            "T3-2@95 T4+2@95",
            "T4+4@105 T5-4@105",
            "T5-1@100 T6+1@100",
            "T7-3@105 T8+3@105"
        };

        for (const auto& s : transactions_2) {
            auto tradeOrder = engine.ParseInput(s);
            engine.ProcessOrder(tradeOrder);
        }

        const auto& trades2 = engine.GetTrades();
        if (trades2.size() != transactions_2_res.size()) {
            std::cerr << "Transactions_2 result differs from expected result: "
                    << "Expected " << transactions_2_res.size() << " trades, got " << trades2.size()
                    << std::endl;
            return false;
        }

        for (size_t i = 0; i < trades2.size(); ++i) {
            if (trades2[i] != transactions_2_res[i]) {
                std::cerr << "Transactions_2 mismatch. Expected: " << transactions_2_res[i]
                        << ", Got: " << trades2[i] << std::endl;
                return false;
            }
        }
    }

    {
        TradeEngine engine;

        std::array<std::string, 9> transactions_3 = {
            "T1 B 5 100",
            "T2 S 3 90",  
            "T3 S 4 95",   
            "T4 B 6 105",
            "T5 S 5 100",
            "T6 B 2 100",
            "T7 S 3 105",
            "T8 B 4 110",
            "T9 S 2 120"
        };
        
        std::array<std::string, 6> transactions_3_res = {
            "T1+3@100 T2-3@100",
            "T1+2@100 T3-2@100",
            "T3-2@95 T4+2@95",  
            "T4+4@105 T5-4@105",
            "T5-1@100 T6+1@100",
            "T7-3@105 T8+3@105" 
        };
            
        for (const auto& s : transactions_3) {
            auto tradeOrder = engine.ParseInput(s);
            engine.ProcessOrder(tradeOrder);
        }

        const auto& trades3 = engine.GetTrades();
        if (trades3.size() != transactions_3_res.size()) {
            std::cerr << "Transactions_3 result differs from expected result: "
                    << "Expected " << transactions_3_res.size() << " trades, got " << trades3.size()
                    << std::endl;
            return false;
        }

        for (size_t i = 0; i < trades3.size(); ++i) {
            if (trades3[i] != transactions_3_res[i]) {
                std::cerr << "Transactions_3 mismatch. Expected: " << transactions_3_res[i]
                        << ", Got: " << trades3[i] << std::endl;
                return false;
            }
        }
    }

    return true;
}

int main() {
    std::cerr << "Running tests...\n";
    if (validateTradeMatchingEngine()) {
        std::cerr << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << "One or more tests failed\n";
        return -1;
    }
}