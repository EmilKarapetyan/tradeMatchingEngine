#ifndef TRADE_ENGINE_H
#define TRADE_ENGINE_H

#include <cctype>
#include <cstdint>
#include <map>
#include <vector>
#include <list>
#include <string>

namespace Trading
{
	class TradeEngine;
}

class Trading::TradeEngine final
{
public:

	//The only available constructor.
	explicit TradeEngine() = default;

	//Removed copy and move constructors/assignment operators.
	TradeEngine(const TradeEngine&) = delete;
	TradeEngine(TradeEngine&&) = delete;
	TradeEngine& operator=(const TradeEngine&) = delete;
	TradeEngine& operator=(TradeEngine&&) = delete;

	//Default destructor.
	~TradeEngine() = default;

public:
	struct TradeOrder
	{
		std::string identifier;
		char side;
		std::uint64_t quantity;
		std::uint64_t price;
	};

	using ordersMap = std::map<const std::uint64_t, std::list<TradeOrder>>;

public:
	void Start();

	[[nodiscard]] TradeEngine::TradeOrder ParseInput(const std::string& input);

    void ProcessOrder(const TradeOrder& order);

	[[nodiscard]] const ordersMap& GetBuyOrders() const noexcept;

	[[nodiscard]] const ordersMap& GetSellOrders() const noexcept;

	[[nodiscard]] const std::vector<std::string>& GetTrades() const noexcept;

private:
	void HandleSell(const std::string& trader, 
					std::uint64_t quantity, 
					const std::uint64_t price) noexcept;

	void HandleBuy(const std::string& trader, 
				   std::uint64_t quantity, 
				   const std::uint64_t price) noexcept;

	void CollectTrades() noexcept;

private:
	ordersMap m_buyOrders;
	ordersMap m_sellOrders;
	std::vector<std::string> m_trades;
    std::map<std::pair<std::string, char>, std::map<int, int>> m_tradeInfo;
};

#endif //TRADE_ENGINE_H
