#include <iostream>
#include <vector>
#include <cstdlib>        // For getenv
#include "MarketDataHandler.h"
#include "Algorithm.h"

int main() {
    // ------------------------------------------------------------------
    // 1. Obtain API key from environment and perform a basic sanity check
    // ------------------------------------------------------------------
    const char* key = std::getenv("API_KEY");
    if (key == nullptr) {
        std::cerr << "[ERROR] Environment variable API_KEY is not set.\n";
        std::cerr << "        Export your AlphaVantage key before running the application, e.g.\n";
        std::cerr << "        export API_KEY=YOUR_KEY_HERE\n";
        return EXIT_FAILURE;
    }

    //-------------------------------------------------------------------
    // 2. Ask the user which symbol they would like to trade
    //-------------------------------------------------------------------
    std::string symbol;
    std::cout << "Enter the ticker symbol you would like to trade (e.g. AAPL): ";
    std::cin >> symbol;

    //-------------------------------------------------------------------
    // 3. Fetch the latest intraday market data for the selected symbol
    //-------------------------------------------------------------------
    MarketDataHandler mdHandler; // API key picked up in the ctor
    std::vector<MarketData> marketData;

    std::cout << "Fetching market data for " << symbol << " ..." << std::endl;
    mdHandler.fetchMarketData(symbol, marketData);

    if (marketData.empty()) {
        std::cerr << "[ERROR] Failed to fetch market data or received empty dataset.\n";
        return EXIT_FAILURE;
    }

    //-------------------------------------------------------------------
    // 4. Instantiate trading engine and plug-in the strategies we want to run
    //-------------------------------------------------------------------
    trading::TradingEngine engine;
    engine.addStrategy(new trading::TrendFollowingStrategy());
    engine.addStrategy(new trading::MeanReversionStrategy());
    engine.addStrategy(new trading::ArbitrageStrategy());

    //-------------------------------------------------------------------
    // 5. Feed each datapoint to the trading engine (simulating a live feed)
    //-------------------------------------------------------------------
    for (const auto& dataPoint : marketData) {
        engine.processData(dataPoint);
    }

    //-------------------------------------------------------------------
    // 6. Optionally distribute the raw data somewhere else (here we just print)
    //-------------------------------------------------------------------
    mdHandler.distributeData(marketData);

    //-------------------------------------------------------------------
    // 7. Graceful shutdown
    //-------------------------------------------------------------------
    std::cout << "Finished processing. Exiting application.\n";
    return EXIT_SUCCESS;
}

