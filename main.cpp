#include "Logger.hpp"
#include <iostream>
#include <memory> // Include for std::unique_ptr

using namespace LOGGER;

int main() {
    
    std::unique_ptr<Logger> myLogger = std::make_unique<Logger>("./logger.log", 5, 1024 * 1024); // Example: 5 rotations, 1MB limit

    if (myLogger) 
    {
        myLogger->Log(Logger::LogLevel::DEBUG, "This is a debug message.");
        myLogger->Log(Logger::LogLevel::INFO, "This is an info message.");
        myLogger->Log(Logger::LogLevel::WARN, "This is a warning message.");
        myLogger->Log(Logger::LogLevel::ERROR, "This is an error message.");
        myLogger->Log(Logger::LogLevel::FATAL, "This is a fatal message.");

        std::cout << "Logger used successfully. Check my_app.log." << std::endl;
    } 
    else 
    {
        std::cerr << "Failed to create logger!" << std::endl;
        return 1;
    }

    return 0;
}
