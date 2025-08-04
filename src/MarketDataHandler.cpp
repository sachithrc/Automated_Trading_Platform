//Reference for makeapi function: https://medium.com/@codewithbasit/how-do-i-make-http-requests-such-as-get-post-put-and-delete-in-c-393fb5d5d25

#include "MarketDataHandler.h"
#include <iostream>       // For logging and console output
#include <stdexcept>     // For exception handling
#include <cstdlib>      // For getenv
#include <string>
#include<curl/curl.h>
#include <sstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <algorithm>  // for std::sort
using json = nlohmann::json;
using namespace std;

// Constructor: Initialize with the API key and set up global libcurl
MarketDataHandler::MarketDataHandler() {
    const char* key = std::getenv("API_KEY");
    if (key) {
        apiKey = std::string(key);
    } else {
        std::cerr << "[WARN] Environment variable API_KEY is not set. Using empty key – request will fail." << std::endl;
    }

    // Initialise libcurl only once for the lifetime of this object
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

// Destructor: clean up global libcurl state
MarketDataHandler::~MarketDataHandler() {
    curl_global_cleanup();
}

// Method to fetch market data from the API
void MarketDataHandler::fetchMarketData(const string& symbol, vector<MarketData>& marketDataVec) {
    // Construct the API request URL using the symbol and API key
    const string bURL = "https://www.alphavantage.co/query?"; //format of entire url: https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=IBM&interval=5min&apikey=demo
    const string func = "function=TIME_SERIES_INTRADAY";
    const string interval = "&interval=5min";
    const string api = "&apikey=" + apiKey;
    const string url = bURL + func + "&symbol=" + symbol + interval + api; 
    //VERY IMPORTANT: dont forget to add a cin and cout function in int main before you call this function to ask the user what stock they want to trade so we can choose our own symbol! 
    

    // Make the API request using the constructed URL
    string response = makeApiRequest(url);

    // Parse the received JSON response and populate the marketDataVec
    parseMarketData(response, marketDataVec);
}

// Method to distribute the parsed data to the trading engine (or any other consumer)
void MarketDataHandler::distributeData(const vector<MarketData>& marketDataVec) {
    // Iterate over the market data vector and distribute the data
    for (const auto& data: marketDataVec){
        cout << "Timestamp: " << data.timestamp << endl; //all this is data being accessed is from the marketdata struct
        cout << "Symbol: " << data.symbol << endl;
        cout << "Price: " << data.price << endl;
        cout << "Volume: " << data.volume << endl;

    }
} //if needed later add code to send this data to other files such as a logger or to update real time dashboard. You can add this later but not necessary

// Helper method to make HTTP requests
string MarketDataHandler::makeApiRequest(const string& url) {
    // Set up and execute the HTTP GET request:

    //Create a curl handle: A curl handle is used to configure requests. You control how libcurl interacts with remote servers using this handle
    CURL *handle = curl_easy_init(); //Making a pointer named handler of the type CURL (which is a struct)
    if (!handle){
        cerr << "Failed to create handle" << endl;
        return "";
    } //if NULL return error message and clean up resources initialized by curl

    //The curl easy init function initializes the CURL handle which we will use to set options and perform actual HTTP requests with
    //It returns a pointer to the initializes curl object if successful or NULL if it fails



    //curl_easy_setopt is used to tell libcurl how to behave. By setting the appropriate options, the application can change libcurl's behavior. 
    //All options are set with an option followed by a parameter.

    // set request parameters
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

    // Disable HTTP/2 to avoid rare crashes in some libcurl builds on macOS
    curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);

    // Follow HTTP redirects automatically (convenience)
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);



    //callback function: A callback function is a function that is passed as an argument to another function and is expected to be called (or "called back") at a specific point in the execution of that function. 
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, [](char *data, size_t size, size_t nmemb, void *userp){
        ostringstream *response = static_cast<ostringstream*>(userp); //static cast changes void pointer to type ostringstream. also initializes response as pointer of type ostringstream
        response->write(data, size * nmemb); //accessing data 
        return size * nmemb; //return number of bytes to process it
    }); //For this entire lambda function, you are telling the program: "When you receive data from the server, use this callback function to handle that data."
    //It registers the provided callback function with libcurl, so libcurl knows which function to call whenever it receives data from the server.
    
    
    //Set the user pointer to be an ostream to which the response will the written
    ostringstream response;
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response); //CURLOPT_WRITEDATA allows you to specify a pointer to a location where cURL will write the data it receives. Its an option in the cURL library (libcurl), which is used for transferring data with URLs

    //Performing the request
    CURLcode result = curl_easy_perform(handle); //curl_easy_perform() is a function in the cURL library (libcurl) that performs the actual data transfer based on the options that have been set using the curl_easy_setopt() function. It is a key function in the process of using cURL to make HTTP requests, download files, or perform any network operation.
    //curlcode just returns the return value based on whether this fails or succeeds.

    //check result
    if (result != CURLE_OK){
        cerr << "curl_easy_perform failed" << curl_easy_strerror(result) << endl; //printing error message based on return value of CURLcode
        curl_easy_cleanup(handle);
        return ""; //return value
    }

    //cleaning up whatever was in curl library if everything was successful
    curl_easy_cleanup(handle);


    // If successful return the response as a string
    return response.str();
}

// Helper method to parse the JSON response
void MarketDataHandler::parseMarketData(const string& jsonResponse, vector<MarketData>& marketDataVec) {
    try {
        auto j = json::parse(jsonResponse);

        // Alpha Vantage intraday endpoint nests the datapoints under "Time Series (5min)"
        if (!j.contains("Time Series (5min)")) {
            std::cerr << "[WARN] Unexpected JSON format – missing 'Time Series (5min)' field." << std::endl;
            return;
        }

        const auto& timeSeries = j["Time Series (5min)"];
        for (auto it = timeSeries.begin(); it != timeSeries.end(); ++it) {
            MarketData data;
            data.timestamp = it.key();
            data.symbol = ""; // Caller can overwrite if desired

            try {
                // All numeric values are returned as strings – convert them.
                data.price  = std::stod(it.value().value("4. close", "0"));
                data.volume = std::stoi(it.value().value("5. volume", "0"));
            } catch (const std::exception& e) {
                std::cerr << "[WARN] Failed to convert price/volume to numeric type: " << e.what() << std::endl;
                continue;
            }

            marketDataVec.push_back(data);
        }

        // Sort datapoints chronologically (earliest first)
        std::sort(marketDataVec.begin(), marketDataVec.end(),
                  [](const MarketData& a, const MarketData& b) { return a.timestamp < b.timestamp; });
    }
    catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << '\n';
    }
    catch (const std::exception& e) {
        std::cerr << "Exception while parsing market data: " << e.what() << '\n';
    }
}
