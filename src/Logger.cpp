#include "../inc/Logger.hpp"
#include <time.h>
#include <iostream>
namespace LOGGER
{
    Logger::Logger(const std::string& filePath, const size_t maxRotationCount, const size_t fileSizeLimit ) :   
                                                filePath_(filePath),
                                                logFileSize_(0),
                                                rotationCount_(0),
                                                fileSizeLimit_(fileSizeLimit),
                                                maxRotationCount_(maxRotationCount),
                                                shutdown_(false)
                                            
                                                
    {
        try 
        {
            fileHandler_ = std::make_unique<FileOps::FileHandler>(filePath_);
        } 
        catch (const std::exception& e) {
            std::cerr << "Error initializing file handler: " << e.what() << std::endl;
            throw;
        }
        LogManager_ = std::thread(&Logger::Manager, this);
    }
    
    void Logger::RotateFile()
    {
        if(rotationCount_ > maxRotationCount_)
        {
            rotationCount_ = 0;
        }

        std::string rotatfileName = filePath_ + static_cast<char>(rotationCount_);

        std::lock_guard<std::mutex> lock(loggerMutex_);
        fileHandler_->MoveAndOverwriteFile(rotatfileName);
        ++rotationCount_;
    }

    void Logger::GetFileSize()
    {
        std::lock_guard<std::mutex> lock(loggerMutex_);
        logFileSize_ = fileHandler_->GetFileSize();

    }

    void Logger::Manager()
    {
        
        while(true)
        {       
                if (shutdown_)
                {
                    break;
                }

                if (!filePath_.empty()) 
                {
                    std::cout << "Checking log file size..." << std::endl; // Debugging log
                    GetFileSize();

                    if (logFileSize_ > fileSizeLimit_)
                    {
                        std::cout << "Log file size exceeds limit, rotating..." << std::endl; // Debugging log
                        RotateFile();
                    }
                }
                std::this_thread::sleep_for(std::chrono::seconds(10));
        }
        
    }

    
    const std::string  Logger::GetCurrentTime()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm* parts = std::localtime(&now_c);
        char buffer[100];
        strftime(buffer, sizeof(buffer), "%d:%m:%Y:%H:%M:%S", parts);
        return std::string(buffer);

    }
    
    void Logger::Log(LogLevel level,const std::string& message)
    {
        std::cout << "writing Log" << std::endl;
           std::string levelStr;
            switch (level)
            {
            case FATAL:
                levelStr = "FATAL";
                break;
            case ERROR:
                levelStr = "ERROR";
                break;
            case WARN:
                levelStr = "WARN";
                break;
            case INFO:
                levelStr = "INFO";
                break;
            case DEBUG:
                levelStr = "DEBUG";
                break;
            default:
                levelStr = "UNKNOWN";
                break;
            }
            std::string logLine = "[ " + GetCurrentTime() + " ] " + "[ " + levelStr + " ] : " + message + "\n";
            fileHandler_->Write(logLine);
    }
    void Logger::CleanLogs()
    {
        fileHandler_->CleanFile();
    }

    Logger::~Logger()
    {
        std::cout << "Logger destructor called, shutting down..." << std::endl;  // Debugging log
        {
            std::lock_guard<std::mutex> lock(loggerMutex_);
            shutdown_ = true;  // Set shutdown flag to true
            
        }
        if (LogManager_.joinable())
        {
            LogManager_.join();  // Wait for the thread to finish
        }
        fileHandler_.reset();
    }

}