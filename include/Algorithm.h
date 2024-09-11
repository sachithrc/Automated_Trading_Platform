#ifndef TRADING_ALGORITHMS_H
#define TRADING_ALGORITHMS_H

#include <vector>
#include <string>
#include "MarketDataHandler.h"

namespace trading {

// Forward declarations
class TradingStrategy;

// Base class for trading strategies
class TradingStrategy {
public:
    virtual ~TradingStrategy() {}
    virtual void analyzeData(const MarketData& data) = 0;
    virtual void executeDecision() = 0;
};

// Trend Following Strategy
class TrendFollowingStrategy : public TradingStrategy {
private:
    double movingAverage;
    std::vector<double> priceHistory;
public:
    TrendFollowingStrategy();
    virtual void analyzeData(const MarketData& data) override;
    virtual void executeDecision() override;
};

// Mean Reversion Strategy
class MeanReversionStrategy : public TradingStrategy {
private:
    double meanPrice;
    std::vector<double> priceHistory;
public:
    MeanReversionStrategy();
    virtual void analyzeData(const MarketData& data) override;
    virtual void executeDecision() override;
};

// Arbitrage Strategy
class ArbitrageStrategy : public TradingStrategy {
public:
    ArbitrageStrategy();
    virtual void analyzeData(const MarketData& data) override;
    virtual void executeDecision() override;
};

// Trading Engine to manage and execute strategies
class TradingEngine {
public:
    TradingEngine();
    ~TradingEngine();
    void addStrategy(TradingStrategy* strategy);
    void removeStrategy(TradingStrategy* strategy);
    void processData(const MarketData& data);
private:
    std::vector<TradingStrategy*> strategies;
};

} // namespace trading

#endif // TRADING_ALGORITHMS_H
