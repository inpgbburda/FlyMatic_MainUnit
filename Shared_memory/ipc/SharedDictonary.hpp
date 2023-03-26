#pragma once

#include <map>
#include <exception>
#include <algorithm>
#include <optional>

#include "shared_memory.hpp"


namespace Ipc
{

namespace
{
    struct ElementInformation
    {
        SharedMemPtr ptr;
        size_t size;
    };
}


template<typename Key, typename T>
class SharedDictonary
{
public:
    SharedDictonary(const std::string& memoryName, const size_t size, const CreationFlag flags): 
    m_sharedMemory(memoryName, size, flags), m_offset(sizeof(Key))
    {

    }

    T operator[](const Key& key)
    {
        auto element = getItemFromLocalDict(key);

        if (element == std::nullopt)
        {
            throw std::out_of_range(std::string{ "Unable to find element with given key." });
        }

        std::vector<uint8_t> data(element.value()->second.ptr, element.value()->second.ptr + element.value()->second.size);

        return *reinterpret_cast<T*>(element.value()->second.ptr + m_offset); //need to take values not key
    }
    
    bool Add(const Key& key, const T& value)
    {
        if (getItemFromLocalDict(key) != std::nullopt)
        {
            return false;
        }

        try
        {
            size_t keySize = sizeof(key);
            size_t valueSize = sizeof(value);
            const uint8_t* tmpKeyPtr = reinterpret_cast<const uint8_t*>(key);
            const uint8_t* tmpValuePtr = reinterpret_cast<const uint8_t*>(value);
            auto begginingOfNewValueInShm = m_lastPositionInShm;
            std::vector<uint8_t> tmpData;

            std::copy(std::begin(tmpKeyPtr), std::end(tmpKeyPtr), tmpData.end());
            std::copy(std::begin(tmpValuePtr), std::end(tmpValuePtr), tmpData.end());

            m_lastPositionInShm = m_sharedMemory.AppendToTheEnd(tmpData);

            PositionOfTheKey.emplace({key, ElementInformation{.ptr = begginingOfNewValueInShm,
                                                              .size = keySize + valueSize}});

            return true;
        }
        catch (const std::exception& e)
        {
            throw e;
        }
    }

    bool Remove(const Key& key)
    {
        auto element = getItemFromLocalDict(key);
        
        if (element == std::nullopt)
        {
            return false;
        }
        
        auto startOfRemovinglData = element.value()->second.ptr;
        auto startOfNextData = element.value()->second.ptr + m_offset + element.value()->second.size;

        std::vector<uint8_t> tmpDataHolder (startOfNextData, m_lastPositionInShm);

        m_sharedMemory.AppendToTheEnd();



    }

    ~SharedDictonary(){}

private:
    auto getItemFromLocalDict(const Key& key)
    {
        auto element = PositionOfTheKey.find(key);

        if (element != PositionOfTheKey.end())
        {
            return std::optional<std::map<Key, ElementInformation>::iterator> {element};
        }

        return std::nullopt;
    }

    std::map<Key, ElementInformation> PositionOfTheKey;
    //throw std::invalid_argument(std::string{ "Key is not present in the dictonary." });
    SharedMemory m_sharedMemory; 
    SharedMemPtr m_lastPositionInShm;
    size_t m_offset;
};

}