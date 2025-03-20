#include <iostream>

#include "TradeEngine.hpp"

int main()
{
    Trading::TradeEngine engine;

    try
    {
        engine.Start();
    }
    catch (const std::invalid_argument& arg)
    {
        std::cerr << "Error: " << arg.what()
                  << ". Resetting and moving to the next line." << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Error: Internal error." << std::endl;
    }
    return 0;
}
