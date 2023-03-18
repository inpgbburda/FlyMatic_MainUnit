#include <iostream>
#include "shared_memory.hpp"
//fchmod / fchown - zmiana wlascicielstwa 
namespace Ipc
{
    //todo: przekroczenie zakresu przez lastIndex
    //todo: kontrola przesuwania wskaznika
    //todo: synchronizacja

    SharedMemory::SharedMemory(const std::string &memoryName, const size_t size, const CreationFlag creationFlags) : 
    m_memoryName(memoryName), m_maxSize(size + BytesForCurrentSizeValue + BytesForLastIndexValue),
    m_semaphore(memoryName + std::string{"_sem"}, CreationFlagsSem::Create), m_currentOccupiedMem(0),
    m_lastIndex(0)
    {
        //mode_t parameter is about setting permission to opened memory block. 
        //The value are the same like in file
        
        m_descriptor = OpenMemory(m_memoryName, creationFlags);

        if (ftruncate(m_descriptor, size) == errorValue)
        {
            throw std::runtime_error("Unable to change size of allocated memory. Error: " + std::string{ strerror(errno) } + "(" + std::to_string(errno) + ")");
        }
    
        m_beginPtr = MapMemory(m_descriptor, size);
        m_beginOfDataPtr = m_beginPtr + BytesForCurrentSizeValue + BytesForLastIndexValue;
        m_currentEndPtr = m_beginPtr;
        m_endPtr = m_beginPtr + m_maxSize;
    }

    SharedMemory::SharedMemory(const std::string& memoryName) : m_memoryName(memoryName), m_semaphore(memoryName + std::string{"_sem"}, CreationFlagsSem::Create) // dodac semaphora
    {
        m_descriptor = OpenMemory(m_memoryName, CreationFlagsShm::ReadWrite);
        m_maxSize = GetInformation().st_size;

        if(m_maxSize == 0)
        {
            throw std::runtime_error("Error. Process opened memory with a zero size. Probably, master process haven't initialize shared memory yet.");
        }

        m_beginPtr = MapMemory(m_descriptor, m_maxSize);
        m_beginOfDataPtr = m_beginPtr + BytesForCurrentSizeValue + BytesForLastIndexValue;
        m_currentEndPtr = m_beginPtr + GetCurrentSizeFromMem();
        m_endPtr = m_beginPtr + m_maxSize;
        m_lastIndex = GetLastIndexFromMem();
        m_currentOccupiedMem = GetCurrentSizeFromMem();
    }

    int SharedMemory::OpenMemory(const std::string& name, const CreationFlag& cr)
    {
        int desc = shm_open(m_memoryName.c_str(), static_cast<int>(cr), S_IRUSR | S_IWUSR | S_IXUSR );

        if (desc == errorValue)
        {
            throw std::runtime_error("Unable to create or attach to existing shared memory block. Error: " 
            + std::string{ strerror(errno) } + "(" + std::to_string(errno) + ")");
        }

        return desc;
    }

    PtrToShm SharedMemory::MapMemory(int descriptor, size_t size)
    {
        auto tmpPtr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, descriptor, 0);

        if(tmpPtr == nullptr)
        {
            throw std::runtime_error("Ptr to new allocated memory block is not set properly.");
        }

        return PtrToShm(tmpPtr);
    }

    void SharedMemory::SaveCurrentSizeToMem(const uint32_t currSize)
    {
        memcpy(*m_beginPtr, &currSize, sizeof(currSize));
    }

    void SharedMemory::SaveLastIndexToMem(const uint16_t currIndex)
    {
        memcpy(*m_beginPtr, &currIndex, sizeof(currIndex));
    }

    uint32_t SharedMemory::GetCurrentSizeFromMem()
    {
        std::vector<uint8_t> tmpData (m_beginPtr.getPtr(), (m_beginPtr + BytesForCurrentSizeValue).getPtr());

        return *reinterpret_cast<uint32_t*>(tmpData.data());
    }

    uint16_t SharedMemory::GetLastIndexFromMem()
    {
        std::vector<uint8_t> tmpData ((m_beginPtr + BytesForCurrentSizeValue).getPtr(), (m_beginPtr + BytesForCurrentSizeValue + BytesForLastIndexValue).getPtr());

        return *reinterpret_cast<uint16_t*>(tmpData.data());
    }
    
    void SharedMemory::UpdateShmProperties()
    {
        m_currentOccupiedMem = GetCurrentSizeFromMem();
        m_lastIndex = GetLastIndexFromMem();
        m_currentEndPtr = m_beginPtr + m_currentOccupiedMem;
    }

    Statistics SharedMemory::GetInformation()
    {
        Statistics statStruct;

        if (fstat(m_descriptor, &statStruct) == errorValue)
        {
            throw std::runtime_error("Unable to get shared memory information. Error: " + std::string{ strerror(errno) } + "(" + std::to_string(errno) + ")");
        }

        return statStruct;
    }

    uint16_t SharedMemory::AppendToTheEnd(const std::vector<uint8_t>& data)
    {
        m_semaphore.Wait();
        UpdateShmProperties();
        
        auto size = data.size();
        if(m_currentOccupiedMem + data.size() > m_maxSize)
        {
            throw std::runtime_error("Error. Size of data is greater than allocated and currently ocuppied memory.");
        }

        auto index = GenerateAndSaveIndex();
        AddSize(size);
        memcpy(*m_currentEndPtr, data.data(), size);
        SaveCurrentSizeToMem(m_currentOccupiedMem + size);
        m_currentOccupiedMem += size;
        m_currentEndPtr = m_currentEndPtr + size;

        m_semaphore.Post();
        return index;
    }

    std::vector<uint8_t> SharedMemory::ReadMemory()
    {
        m_semaphore.Wait();
        
        UpdateShmProperties();

        m_semaphore.Post();
        return std::vector<uint8_t> ((m_beginPtr + BytesForCurrentSizeValue).getPtr(), m_currentEndPtr.getPtr());
    }

    uint16_t SharedMemory::GenerateAndSaveIndex()
    {
        m_lastIndex++;
        memcpy(*m_currentEndPtr, &m_lastIndex, sizeof(m_lastIndex));

        m_currentOccupiedMem += BytesForLastIndexValue;
        m_currentEndPtr = m_currentEndPtr + BytesForLastIndexValue;
        SaveCurrentSizeToMem(m_currentOccupiedMem);
        SaveLastIndexToMem(m_lastIndex);
        
        return m_lastIndex;
    }

    void SharedMemory::AddSize(const uint32_t size)
    {
        memcpy(*m_currentEndPtr, &size, sizeof(size));
        m_currentOccupiedMem += BytesForCurrentSizeValue;
        m_currentEndPtr = m_currentEndPtr + BytesForCurrentSizeValue;
        SaveCurrentSizeToMem(m_currentOccupiedMem);
    }

    bool SharedMemory::RemoveDataAttachedToIndex(const uint16_t index)
    {
        m_semaphore.Wait();
        PtrToShm tmpIterationPtr = m_beginOfDataPtr;
        while (tmpIterationPtr != m_currentEndPtr)
        {
            auto indexOfCurrData = GetBytes<uint16_t>(tmpIterationPtr);
            auto sizeOfCurrData = GetBytes<uint32_t>(tmpIterationPtr + BytesForLastIndexValue);
            auto ptrToNextData = tmpIterationPtr + BytesForCurrentSizeValue + BytesForLastIndexValue + sizeOfCurrData;
            
            if(indexOfCurrData == index)
            {
                size_t rangeToMove = m_currentEndPtr - ptrToNextData;
                memmove(*tmpIterationPtr, *ptrToNextData, rangeToMove);
                m_currentEndPtr = *m_currentEndPtr - sizeOfCurrData;
                m_currentOccupiedMem -= sizeOfCurrData;
                SaveCurrentSizeToMem(m_currentOccupiedMem);
            }
            else
            {
                tmpIterationPtr = ptrToNextData;
            }
        }

        m_semaphore.Post();
        return true;
    }

    void SharedMemory::ClearData()
    {
        m_currentOccupiedMem = 0;
        m_currentEndPtr = m_beginOfDataPtr;
        m_lastIndex = 0;

        SaveCurrentSizeToMem(m_currentOccupiedMem);
        SaveLastIndexToMem(m_lastIndex);
    }

    SharedMemory::~SharedMemory()
    {
        if (shm_close(m_memoryName.c_str()) == errorValue)
        {
            std::runtime_error("ERROR!");
        }

        munmap(*m_beginPtr, m_maxSize);
    }
}