#ifndef MARKET_DATA_HANDLER_H
#define MARKET_DATA_HANDLER_H

#include <string>
#include <cstdlib>  // for getenv
#include <vector>   // for std::vector

// ---------------------------------------------------------------------------
// Data structures
// ---------------------------------------------------------------------------
struct MarketData {
    std::string timestamp;  // Timestamp of the data point
    std::string symbol;     // Stock symbol
    double      price;      // Price of the stock
    int         volume;     // Volume traded
};

// ---------------------------------------------------------------------------
// MarketDataHandler
// ---------------------------------------------------------------------------
class MarketDataHandler {
public:
    // Constructor (retrieves API key from environment)
    MarketDataHandler();

    // Destructor (cleans up libcurl global state)
    ~MarketDataHandler();

    // Fetch intraday market data for a given symbol. Results are appended to
    // 'marketDataVec'.
    void fetchMarketData(const std::string& symbol, std::vector<MarketData>& marketDataVec);

    // Print or forward the data to any downstream consumer (currently console)
    void distributeData(const std::vector<MarketData>& marketDataVec);

private:
    std::string apiKey; // AlphaVantage API key

    // Perform a blocking HTTP GET request and return the raw response body
    std::string makeApiRequest(const std::string& url);

    // Parse raw JSON into a vector of MarketData objects
    void parseMarketData(const std::string& jsonResponse, std::vector<MarketData>& marketDataVec);
};

#endif // MARKET_DATA_HANDLER_H
