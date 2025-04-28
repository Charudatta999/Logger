
#include "../inc/FileHandler.hpp"


#include <fcntl.h>   // For open() and related flags
#include <unistd.h>  // For close() and other system calls
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <iostream>
#include <cstring>


namespace FileOps
{
    FileHandler::FileHandler(std::string filePath) : filePath_(filePath)
    {   
        stopRequested_ = false;
        ExtractDirectoryPath();
        if (!CheckForDirPermission())
        {
            throw std::runtime_error("No write permission to directory: " + directoryPath_);
        }
        else
        {
            std::cout << "We have writing Permission for dir.! Yay!"  << std::endl;
        }

        if (!HasWritePermission())
        {
            std::cerr << "Ahh! Not enough permission to write in the file : " << filePath_;
            throw std::runtime_error("No write permission for log file.");
        }
        else
        {
            std::cout << "We have writing Permission! Yay!"  << std::endl;
            fd_ = open(filePath_.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd_ == -1) 
            {
                std::cerr << "Failed to open file: " << filePath_.c_str() << " Error: "<< strerror(errno) << std::endl;
                throw std::runtime_error("File open failed");
            }
            else
            {
                std::string message = "Started \n";
                ssize_t bytesWritten = write(fd_, message.c_str(), message.size());
                if (bytesWritten == -1)
                {
                    std::cerr << "Failed to write in the File: " << filePath_.c_str() << " Error: " << strerror(errno) << std::endl;
                }
            }
            close(fd_);
        }
        
        std::cout << "Starting Monitor Thread"  << std::endl;
        
        
        monitorThread_ = std::thread(&FileHandler::MonitorThread, this);

    }

    FileHandler::~FileHandler()
    {
        std::cout << "File Handler destructor called, shutting down..." << std::endl;  // Debugging log
        {
            std::lock_guard<std::mutex> lock(fileMutex_);
            std::cout << "shutting down Singal Sent to Monitor thread of fileHandler..." << std::endl;
            stopRequested_ = true;
        }
        std::cout << "Joining Thread.." << std::endl;
        if (monitorThread_.joinable())
        {
            monitorThread_.join();
        }
        std::cout << "Thread Exited.." << std::endl;
        
        if (fd_ != -1)
        {
            close(fd_);
            fd_ = -1;
        }

    }

    bool FileHandler::HasWritePermission() const
    {
        if(!access(filePath_.c_str(),W_OK))
        {
            return false;
        }
        return true;
    }

    void FileHandler::ExtractDirectoryPath()
    {
        size_t pos = filePath_.rfind("/");
        if (pos != std::string::npos)
        {
            directoryPath_ = filePath_.substr(0, pos);
        }
        else
        {
            directoryPath_ = ".";
        }
    }

    bool FileHandler::CheckForDirPermission() const
    {
        return (access(directoryPath_.c_str(),W_OK) == 0);
    }

    bool FileHandler::CheckIfFileExists() const
    {
        return (access(filePath_.c_str(), F_OK) == 0);
    }

    bool FileHandler::Write(const std::string& message)
    {
        std::cout << "Inside Write of filehandler" << std::endl;
        std::lock_guard<std::mutex> lock(fileMutex_);
        
        if(fd_ != -1)
        {
            ssize_t bytesWritten = write(fd_, message.c_str(), message.size());
            if (bytesWritten == -1)
            {
                std::cerr << "Failed to write in the File: " << filePath_.c_str() << " Error: " << strerror(errno) << std::endl;
                return false;
            }
            writeCount_ ++;
            lastWriteTime_ = std::chrono::steady_clock::now();
            return true;
        }
        fd_ = open(filePath_.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0666);  
        ssize_t bytesWritten = write(fd_, message.c_str(), message.size());
        if (bytesWritten == -1)
        {
            std::cerr << "Failed to write in the File: " << filePath_.c_str() << " Error: " << strerror(errno) << std::endl;
            return false;
        }   
        writeCount_ ++;
        lastWriteTime_ = std::chrono::steady_clock::now();
        return true;
    }

    void  FileHandler::MonitorThread()
   {
    while (true)
    {       

        if(stopRequested_)
        {
            break;
        }
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastWriteTime_);
        std::lock_guard<std::mutex> lock(fileMutex_);
        if (writeCount_ >= 1000 || elapsed.count() >= 60)
        {
            if (fd_ != -1)
            {
                close(fd_);
                
                fd_ = -1;
                writeCount_ = 0;
            }
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(50));
   }
    
    size_t FileHandler::GetFileSize() const
    {
        
        struct stat fileStat{};
        if (stat(filePath_.c_str(), &fileStat) != 0) 
        {
            throw std::runtime_error("Failed to get file size: " + std::string(strerror(errno)));
        }
        return static_cast<size_t>(fileStat.st_size);
    }

    bool FileHandler::MoveAndOverwriteFile(const std::string& name)
    {
        std::lock_guard<std::mutex> lock(fileMutex_);
        
        if(access(name.c_str(), F_OK) == 0)
        {
            if (remove(name.c_str()) != 0)
            {
                std::cerr << "Error removing existing file: " << strerror(errno) << std::endl;
                throw std::runtime_error("Failed to remove the existing file.");
            }
        }
        if(rename(filePath_.c_str(),name.c_str()) != 0)
        {
                std::cerr << "Error renaming file!" << std::endl;
                return false;
        }
        return true;
    }

    bool FileHandler::CleanFile()
    {
        if(fd_ == -1)
            fd_ =open(filePath_.c_str(), O_WRONLY | O_TRUNC);

        if (fd_ == -1)
        {
            std::cerr << "Failed to open file for truncation: " << strerror(errno) << std::endl;
            throw std::runtime_error("File truncation failed");
        }
        return true;
    }
}