#pragma once

#include <string>
#include <string.h>

#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <vector>
#include <stdexcept>
#include <errno.h>
#include <unistd.h>
#include <memory>
#include <cstddef>
#include <semaphore.h>

#include "semaphore.hpp"
#include "shared_memory_utils.hpp"
#include "../utils/flag.hpp"
#include "../utils/PtrUtils.hpp"

namespace Ipc
{
    /*
        Protocol of storage in shared memory:

            [4 bytes - size of current occupied bytes][2 bytes - last index][2 bytes - index][2 bytes - size of element]
    
    */
    using namespace fl;
    using Statistics = struct stat;  

    class SharedMemory
    {
        public:
            SharedMemory(const std::string& memoryName);
            SharedMemory(const std::string& memoryName, const size_t size, const CreationFlag creationFlags);
            uint16_t AppendToTheEnd(const std::vector<uint8_t>& data);
            bool RemoveDataAttachedToIndex(const uint16_t Index);
            std::vector<uint8_t> ReadMemory();           
            Statistics GetInformation();
            void ClearData();
            ~SharedMemory();

        private:
            uint32_t m_currentOccupiedMem;
            PtrToShm m_currentEndPtr;
            Semaphore m_semaphore;
            uint16_t m_lastIndex;
            PtrToShm m_beginPtr;
            PtrToShm m_beginOfDataPtr;
            PtrToShm m_endPtr;

            PtrToShm MapMemory(int descriptor, size_t size);
            int OpenMemory(const std::string &name, const CreationFlag &cr);
            void SaveCurrentSizeToMem(const uint32_t currSize);
            void UpdateShmProperties();
            uint32_t GetCurrentSizeFromMem();
            uint16_t GetLastIndexFromMem();
            uint16_t GenerateAndSaveIndex();
            void SaveLastIndexToMem(const uint16_t currIndex);
            void AddSize(const uint32_t size);

            template <typename T>
            T GetBytes(const PtrToShm start)
            {
                if (start.m_shm >= m_currentEndPtr.m_shm ||
                    start.m_shm + sizeof(T) >= m_currentEndPtr.m_shm)
                {
                    throw std::invalid_argument("There's no data to retrive. Given pointer is out of scope.");
                }

                std::vector<uint8_t> tmp(reinterpret_cast<uint8_t *>(start.m_shm), reinterpret_cast<uint8_t *>(start.m_shm) + sizeof(T));

                return *reinterpret_cast<T *>(tmp.data());
            }

            std::string m_memoryName;
            int m_descriptor;
            const size_t BytesForCurrentSizeValue = 4;
            const size_t BytesForLastIndexValue = 2;
            size_t m_maxSize = 0;
    };
}