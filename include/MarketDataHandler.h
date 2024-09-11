#ifndef MARKET_DATA_HANDLER_H
#define MARKET_DATA_HANDLER_H
using namespace std;
#include <string>
#include <cstdlib> //for getenv
#include <vector>  // For storing market data

// Define a struct for market data
struct MarketData {
    string timestamp;  // Timestamp of the data point
    string symbol;     // Stock symbol (e.g., AAPL)
    double price;           // Price of the stock
    int volume;             // Volume traded
};

// Class to handle market data operations
class MarketDataHandler {
public:
    // Constructor: Initialize with the API key in src file not her 
    MarketDataHandler();

    // Method to fetch market data from the API
    void fetchMarketData(const string& symbol, vector<MarketData>& marketDataVec);

    // Method to distribute the parsed data to the trading engine (or any other consumer)
    void distributeData(const vector<MarketData>& marketDataVec);

private:

    string apiKey;  // Stores the API key

    // Helper method to make HTTP requests
    string makeApiRequest(const string& url);

    // Helper method to parse the JSON response
    void parseMarketData(const string& jsonResponse, vector<MarketData>& marketDataVec);
};

#endif // MARKET_DATA_HANDLER_H
