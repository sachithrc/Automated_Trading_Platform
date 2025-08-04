# Automated Trading Platform

A lightweight, educational high-frequency trading (HFT) simulator written in modern C++. The project demonstrates how to:

1. Fetch real-time market data from the [Alpha Vantage](https://www.alphavantage.co/) REST API (over HTTPS with libcurl)
2. Parse JSON responses with **nlohmann/json**
3. Feed the data into a pluggable trading engine
4. Implement and evaluate multiple trading strategies (Trend-Following, Mean-Reversion, Arbitrage)

The goal is not to create a production-ready trading system, but rather a concise project that touches on networking, data-parsing, algorithmic decision-making, and clean software design.

---

## Build & Run

The project uses **CMake** (≥3.10) and requires **libcurl** and **nlohmann/json** to be installed on your system.

```bash
# Clone the repo
$ git clone <repo>
$ cd HFT_Project

# Configure & build (Release by default)
$ cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
$ cmake --build build -j

# Export your Alpha Vantage key (free signup)
$ export API_KEY=YOUR_ALPHA_VANTAGE_KEY

# Run the trading simulator
$ ./build/trading_app
```

### Installing nlohmann/json & libcurl (macOS example)

```bash
brew install curl nlohmann-json
```

On Linux, use your distro’s package manager (e.g. `apt` or `dnf`).

---

## Project Structure

```
include/          # Public headers
src/              # Implementation files
config/           # (Optional) runtime configs
logs/             # Runtime logs (git-ignored)
tests/            # Future unit tests
```

---

## Next Steps / Ideas

* Add unit tests with GoogleTest
* Persist trade-logs to a PostgreSQL database
* Write a small Python dashboard to plot live PnL and price series
* Implement WebSocket support for true real-time streaming data

---

### Contributors

* Sachith Chandran
* Saahil Vohra
