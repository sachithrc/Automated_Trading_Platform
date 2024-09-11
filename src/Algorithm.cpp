#include "Algorithm.h"
#include <iostream>
#include <algorithm> // for std::remove

namespace trading {

// ------ Trend Following Strategy ------
TrendFollowingStrategy::TrendFollowingStrategy() : movingAverage(0) {}

void TrendFollowingStrategy::analyzeData(const MarketData& data) {
    // Simple moving average calculation for demonstration
    const int windowSize = 10; // Size of the moving window for average calculation
    priceHistory.push_back(data.price);
    if (priceHistory.size() > windowSize) {
        priceHistory.erase(priceHistory.begin()); // Maintain window size
    }

    double sum = 0;
    for (auto price : priceHistory) {
        sum += price;
    }
    movingAverage = sum / priceHistory.size();
}

void TrendFollowingStrategy::executeDecision() {
    if (priceHistory.size() < 2) return; // Not enough data to make a decision

    // Buy if price is above moving average, sell if below
    if (priceHistory.back() > movingAverage) {
        std::cout << "Trend Following Strategy: Buying, as price is above the moving average." << std::endl;
    } else {
        std::cout << "Trend Following Strategy: Selling, as price is below the moving average." << std::endl;
    }
}

// ------ Mean Reversion Strategy ------
MeanReversionStrategy::MeanReversionStrategy() : meanPrice(0) {}

void MeanReversionStrategy::analyzeData(const MarketData& data) {
    const int maxHistory = 30; // Max history to keep
    priceHistory.push_back(data.price);
    if (priceHistory.size() > maxHistory) {
        priceHistory.erase(priceHistory.begin()); // Maintain history size
    }

    double total = 0;
    for (auto p : priceHistory) {
        total += p;
    }
    meanPrice = priceHistory.empty() ? 0 : total / priceHistory.size();
}

void MeanReversionStrategy::executeDecision() {
    if (priceHistory.empty()) return;

    // Action based on deviation from the mean
    if (priceHistory.back() > meanPrice * 1.05) { // Example threshold
        std::cout << "Mean Reversion Strategy: Selling, as price is significantly above the mean." << std::endl;
    } else if (priceHistory.back() < meanPrice * 0.95) {
        std::cout << "Mean Reversion Strategy: Buying, as price is significantly below the mean." << std::endl;
    }
}

// ------ Arbitrage Strategy ------
ArbitrageStrategy::ArbitrageStrategy() {}

void ArbitrageStrategy::analyzeData(const MarketData& data) {
    // Arbitrage analysis would involve comparing prices across different markets
    // Here we just simulate a scenario for demonstration.
    std::cout << "Arbitrage Strategy: Analyzing market data for arbitrage opportunities." << std::endl;
}

void ArbitrageStrategy::executeDecision() {
    // Example: Check for price discrepancies
    std::cout << "Arbitrage Strategy: Executing trades based on detected price discrepancies." << std::endl;
}

// ------ Trading Engine ------
TradingEngine::TradingEngine() {}

TradingEngine::~TradingEngine() {
    for (auto strategy : strategies) {
        delete strategy; // Clean up strategies
    }
    strategies.clear();
}

void TradingEngine::addStrategy(TradingStrategy* strategy) {
    strategies.push_back(strategy);
}

void TradingEngine::removeStrategy(TradingStrategy* strategy) {
    strategies.erase(std::remove(strategies.begin(), strategies.end(), strategy), strategies.end());
}

void TradingEngine::processData(const MarketData& data) {
    for (auto strategy : strategies) {
        strategy->analyzeData(data);
        strategy->executeDecision();
    }
}

} // namespace trading
