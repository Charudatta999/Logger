#ifndef HEADER_LOGGER
#define HEADER_LOGGER

#include "FileHandler.hpp"

#include <string>

namespace LOGGER
{
    class Logger
    {
        
        private:
            std::string filePath_;
            size_t logFileSize_;
            size_t rotationCount_;
            size_t fileSizeLimit_;
            size_t maxRotationCount_;
            bool shutdown_;
            std::thread LogManager_;
            std::unique_ptr<FileOps::FileHandler> fileHandler_;
            std::mutex loggerMutex_;

            void Manager();
            void RotateFile();
            void GetFileSize();
            

        public:

            // Constructor
            Logger() = delete;
            Logger(const std::string& filepath, const size_t maxRotationCount, const size_t fileSizeLimit);

            //destructor
            ~Logger();


            enum LogLevel
            {
                FATAL,
                ERROR,
                WARN,
                INFO,
                DEBUG
            };
            void Log(LogLevel level,const std::string& message);
            void CleanLogs();
            const std::string GetCurrentTime();
            friend class LoggerTest; 
    };
}
#endif