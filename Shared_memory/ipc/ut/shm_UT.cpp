#include <catch2/catch_all.hpp>
#include <iostream>

#include "../shared_memory/shared_memory.hpp"


namespace
{
    // std::vector<uint8_t> parseDataIntoBytesVec(const std::string& data)
    // {
    //     std::vector<uint8_t> parsedData;

    //     std::for_each(data.begin(), data.end(), [&parsedData](const char& x)
    //     {
    //         parsedData.push_back(x);
    //     });

    //     return parsedData;
    // }

    // template<class T>
    // T parseBytesIntoData(const std::vector<uint8_t>& bytesToParse)
    // {
        
    // }

    // template<>
    // std::string parseBytesIntoData<std::string>(const std::vector<uint8_t>& bytesToParse)
    // {
    //     std::string result;
    //     for (auto &i : bytesToParse)
    //     {
    //         result.push_back(i);
    //     }
        
    //     return result;
    // }
}


TEST_CASE("Double addition to memory")
{
    // using namespace Ipc;
    // std::string name { "MemoryShared" };
    // size_t sizeOfMem = 20;
    // std::string dataToSave { "Information" };
    // std::string dataToSave2 {"About"};
    // SharedMemory shm(name, sizeOfMem, CreationFlagsShm::Truncate | CreationFlagsShm::Create | CreationFlagsShm::ReadWrite);

    // shm.AppendToTheEnd(parseDataIntoBytesVec(dataToSave));

    // auto datafromMem = parseBytesIntoData<std::string>(shm.ReadMem());
    // auto info = shm.GetInformation();

    // REQUIRE(info.st_size == sizeOfMem);
    // REQUIRE(datafromMem == dataToSave);

    // shm.AppendToTheEnd(parseDataIntoBytesVec(dataToSave2));
    // datafromMem = parseBytesIntoData<std::string>(shm.ReadMem());
    // info = shm.GetInformation();

    // REQUIRE(info.st_size == sizeOfMem);
    // REQUIRE(datafromMem == (dataToSave + dataToSave2));
}