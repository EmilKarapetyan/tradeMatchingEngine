#include "TradeEngine.hpp"

#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>

void Trading::TradeEngine::Start()
{
    std::string input;
    std::cerr << "Enter inputs (e.g., T1 B 5 30), one per line. Press Ctrl+D "
                 "to end:"
              << std::endl;

    std::istringstream input_stream(input);
    while (std::getline(std::cin, input))
    {
        decltype(auto) TradeOrder = ParseInput(input);
        ProcessOrder(TradeOrder);
        for (const auto& trade : m_trades)
        {
            std::cout << trade << std::endl;
        }
    }
}

Trading::TradeEngine::TradeOrder Trading::TradeEngine::ParseInput(
    const std::string& input)
{
    if (input.empty())
    {
        throw std::invalid_argument("Input for parseInput is empty");
    }

    std::istringstream iss(input);
    std::string identifier;
    char side{};
    std::string quantity_str;
    std::string price_str;
    std::uint64_t quantity{};
    std::uint64_t price{};

    iss >> identifier >> side >> quantity_str >> price_str;

    if (!std::all_of(identifier.begin(), identifier.end(),
                     [](const char c) { return std::isalnum(c); }))
    {
        throw std::invalid_argument("Invalid identifier passed to parseInput");
    }
    if (!std::isalpha(side))
    {
        throw std::invalid_argument("Invalid side passed to parseInput");
    }

    if (std::all_of(quantity_str.begin(), quantity_str.end(),
                    [](const char c) { return std::isdigit(c); }))
    {
        quantity = std::stoull(quantity_str);
    }
    if (std::all_of(price_str.begin(), price_str.end(),
                    [](const char c) { return std::isdigit(c); }))
    {
        price = std::stoull(price_str);
    }
    return Trading::TradeEngine::TradeOrder{identifier, side, quantity, price};
}

void Trading::TradeEngine::HandleBuy(const std::string& trader,
                                     std::uint64_t quantity,
                                     const std::uint64_t price) noexcept
{
    while (quantity > 0 && !m_sellOrders.empty())
    {
        auto& [bestSellPrice, orders] = *m_sellOrders.begin();
        if (bestSellPrice > price)
        {
            break;
        }

        while (quantity > 0 && !orders.empty())
        {
            auto& restingSellOrder = orders.front();
            const std::uint64_t tradeQty =
                std::min(restingSellOrder.quantity, quantity);
            m_tradeInfo[{trader, '+'}][bestSellPrice] += tradeQty;
            m_tradeInfo[{restingSellOrder.identifier, '-'}][bestSellPrice] +=
                tradeQty;

            quantity -= tradeQty;
            restingSellOrder.quantity -= tradeQty;
            if (restingSellOrder.quantity == 0)
            {
                orders.pop_front();
            }
        }

        if (orders.empty())
        {
            m_sellOrders.erase(bestSellPrice);
        }
    }

    if (quantity > 0)
    {
        m_buyOrders[price].push_back({trader, 'B', quantity, price});
    }
}

void Trading::TradeEngine::HandleSell(const std::string& trader,
                                      std::uint64_t quantity,
                                      const std::uint64_t price) noexcept
{
    while (quantity > 0 && !m_buyOrders.empty())
    {
        auto& [bestBuyPrice, orders] = *m_buyOrders.rbegin();
        if (bestBuyPrice < price)
        {
            break;
        }

        while (quantity > 0 && !orders.empty())
        {
            auto& restingBuyOrder = orders.front();
            const std::uint64_t tradeQty =
                std::min(restingBuyOrder.quantity, quantity);
            m_tradeInfo[{trader, '-'}][bestBuyPrice] += tradeQty;
            m_tradeInfo[{restingBuyOrder.identifier, '+'}][bestBuyPrice] +=
                tradeQty;

            quantity -= tradeQty;
            restingBuyOrder.quantity -= tradeQty;

            if (restingBuyOrder.quantity == 0)
            {
                orders.pop_front();
            }
        }

        if (orders.empty())
        {
            m_buyOrders.erase(bestBuyPrice);
        }
    }

    if (quantity > 0)
    {
        m_sellOrders[price].push_back({trader, 'S', quantity, price});
    }
}

void Trading::TradeEngine::CollectTrades() noexcept
{
    std::vector<std::string> tradeOutput;
	tradeOutput.reserve(m_tradeInfo.size());
    for (const auto& [identifier, sign] : m_tradeInfo)
    {
        const std::string& trader = identifier.first;
        const char trade_sign = identifier.second;
        for (const auto& [price, quantity] : sign)
        {
            tradeOutput.emplace_back(trader + trade_sign +
                                     std::to_string(quantity) + "@" +
                                     std::to_string(price));
        }
    }

    std::sort(tradeOutput.begin(), tradeOutput.end());
    if (!tradeOutput.empty())
    {
        std::stringstream ss;
        for (const std::string& trade : tradeOutput)
        {
            ss << trade << " ";
        }
        std::string result = ss.str();
        result.pop_back();
        m_trades.emplace_back(result);
    }
    m_tradeInfo.clear();
}

void Trading::TradeEngine::ProcessOrder(const TradeOrder& order)
{
    const std::string trader = order.identifier;
    const char side = order.side;
    const std::uint64_t quantity = order.quantity;
    const std::uint64_t price = order.price;

    {
        std::lock_guard lock(m_mutex);
        if (side == 'B')
        {
            HandleBuy(trader, quantity, price);
        }
        else if (side == 'S')
        {
            HandleSell(trader, quantity, price);
        }
        else
        {
            throw std::invalid_argument("ERROR: Unknown side consumed.");
        }
        CollectTrades();
    }
}

const Trading::TradeEngine::ordersMap& Trading::TradeEngine::GetBuyOrders() const noexcept
{
    std::shared_lock lock(m_mutex);
    return m_buyOrders;
}

const Trading::TradeEngine::ordersMap& Trading::TradeEngine::GetSellOrders() const noexcept
{
    std::shared_lock lock(m_mutex);
    return m_sellOrders;
}

const std::vector<std::string>& Trading::TradeEngine::GetTrades() const noexcept
{
    std::shared_lock lock(m_mutex);
    return m_trades;
}