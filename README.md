# Trade Matching Engine

The Trade Matching Engine is designed to automate the process of matching buy and sell orders in a financial market. It ensures that orders are executed fairly and efficiently, following a set of rules that prioritize the best prices and the order in which orders are placed. This engine helps create a smooth, real-time transaction process by matching incoming orders (aggressors) with existing orders (resting orders) to complete trades automatically. It's used in stock markets, cryptocurrency exchanges, or any system where buyers and sellers need to match orders based on specific pricing conditions.

To ensure the engine operates correctly, it includes a set of test cases that verify its functionality. These tests simulate different market scenarios, checking if the engine matches orders properly, respects price-time priority, and handles edge cases like no matching orders or partial order fulfillment.

## Installation

**Prerequisites:**
- A C++ compiler supporting C++17 (e.g., GCC, Clang)
- [CMake](https://cmake.org/) (version 3.22 or higher)
- `clang-format` and `clang-tidy` for code formatting and static analysis

## Project Structure
   ```
TradeMatchingEngine/
├── bin/
│   └── TradeMatchingEngine  # Executable output
├── lib/
│   └── libtradeengine.a     # Static library output
├── src/
│   ├── main.cc              # Program entry point
│   ├── TradeEngine.hpp      # Core trade matching logic header
│   └── TradeEngine.cc       # Core trade matching logic implementation
├── tests/
│   ├── test_trading_engine.cpp  # Test cases
│   └── CMakeLists.txt       # Test CMake configuration
├── CMakeLists.txt           # Root CMake configuration
└── README.md                # Project documentation
```

**Build:**
1. **Clone the repository:**
   ```bash
   git clone https://github.com/EmilKarapetyan/tradeMatchingEngine.git
   cd tradeMatchingEngine
   ```

2. **Build the project:**
   ```bash
   cmake -B build -S .
   cmake --build build
   ```

## Execute and test functionality
1. **Execute:**
   ```bash
   ./bin/TradeMatchingEngine
   ```

2. **Testing:**
   Unit tests are located in the `tests/` directory. To run them:
   ```bash
   cd build
   ctest
   ```

