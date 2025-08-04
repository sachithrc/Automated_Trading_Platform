cmake_minimum_required(VERSION 3.10)

project(AutomatedTradingPlatform LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

# ---------------------------------------------------------------------------
# Target: trading_app
# ---------------------------------------------------------------------------
add_executable(trading_app
    src/Main.cpp
    src/Algorithm.cpp
    src/MarketDataHandler.cpp
)

target_include_directories(trading_app PRIVATE include)

target_link_libraries(trading_app PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)
