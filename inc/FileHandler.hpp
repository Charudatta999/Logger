#ifndef HEADER_FILE_HANDLER
#define HEADER_FILE_HANDLER
#include <string>
#include <chrono>
#include <mutex>
#include <thread>

namespace FileOps
{
    class FileHandler
    {
        private:
            std::string filePath_;
            std::string directoryPath_;
            size_t writeCount_;
            int fd_;
            std::chrono::steady_clock::time_point lastWriteTime_;
            std::mutex fileMutex_;
            bool stopRequested_;
            std::thread monitorThread_;
            

            void ExtractDirectoryPath(); // done
            bool CheckIfFileExists() const; // done
            bool HasWritePermission() const; //done
            bool CheckForDirPermission() const; //done
            void MonitorThread();

            

        public:

            //constuctor and destructor
            FileHandler() = delete;
            FileHandler(std::string FilePath);
            ~FileHandler();

            //public functions
            bool Write(const std::string& message);
            bool CleanFile();
            bool MoveAndOverwriteFile(const std::string& name);
            size_t GetFileSize() const;
            
    };
}
#endif