
#include "catch.hpp"
#include "../Event_Handler/EventAddress.hpp"

using namespace system_address;

TEST_CASE("MakeAddress", "Address")
{
    const uint32_t modId = 0xAFFB4365;
    const uint8_t priority = 0xBA;
    const uint8_t option = 0xDE;

    const Address address = 0xAFFB43650000BADE;

    auto resultAddress = 
                AddressManager::MakeAddress({.modId = modId, .label = AddressManager::LabelToParam(AddressLabels::InvalidLabel), 
                .priority = priority, .option = option});

    REQUIRE(resultAddress == address);
}

TEST_CASE("GetModId", "Address")
{
    const uint32_t expectedModId = 0xAEEB7788;
    
    const Address expectedAddress = 0xAEEB778800000000;

    auto resultAddr = AddressManager::MakeAddress({.modId = expectedModId});

    REQUIRE(expectedAddress == resultAddr);
    REQUIRE(expectedModId == AddressManager::GetModId(resultAddr));
}

TEST_CASE("GetLabel", "Address")
{
    const uint16_t expectedLabel = AddressManager::LabelToParam(AddressLabels::TestLabel);
    
    const Address expectedAddress = 0x0000000000010000;

    auto resultAddr = AddressManager::MakeAddress({.label= expectedLabel});

    REQUIRE(expectedAddress == resultAddr);
    REQUIRE(expectedLabel == AddressManager::GetLabel(resultAddr));
}

TEST_CASE("GetPriority", "Address")
{
    const uint8_t expectedPriority = 0x23;
    
    const Address expectedAddress = 0x0000000000002300;

    auto resultAddr = AddressManager::MakeAddress({.priority = expectedPriority});

    REQUIRE(expectedAddress == resultAddr);
    REQUIRE(expectedPriority == AddressManager::GetPriority(resultAddr));
}


TEST_CASE("GetOption", "Address")
{
    const uint8_t expectedOption = 0x21;
    
    const Address expectedAddress = 0x0000000000000021;

    auto resultAddr = AddressManager::MakeAddress({.option = expectedOption});

    REQUIRE(expectedAddress == resultAddr);
    REQUIRE(expectedOption == AddressManager::GetOption(resultAddr));
}

TEST_CASE("SetModId", "Address")
{
    const uint32_t expectedModId = 0xAEEB7788;
    
    const Address expectedAddress = 0xAEEB778800000000;

    auto resultAddr = AddressManager::MakeAddress();

    AddressManager::SetModId(resultAddr, expectedModId);

    REQUIRE(expectedAddress == resultAddr);
}

TEST_CASE("SetLabel", "Address")
{
    const uint16_t expectedLabel = AddressManager::LabelToParam(AddressLabels::TestLabel);
    
    const Address expectedAddress = 0x0000000000010000;

    auto resultAddr = AddressManager::MakeAddress();

    AddressManager::SetLabel(resultAddr, expectedLabel);

    REQUIRE(expectedAddress == resultAddr);
}

TEST_CASE("SetPriority", "Address")
{
    const uint8_t expectedPriority = 0x23;
    
    const Address expectedAddress = 0x0000000000002300;

    auto resultAddr = AddressManager::MakeAddress();

    AddressManager::SetPriority(resultAddr, expectedPriority);

    REQUIRE(expectedAddress == resultAddr);
}


TEST_CASE("SetOption", "Address")
{
    const uint8_t expectedOption = 0x21;
    
    const Address expectedAddress = 0x0000000000000021;

    auto resultAddr = AddressManager::MakeAddress();

    AddressManager::SetOption(resultAddr, expectedOption);

    REQUIRE(expectedAddress == resultAddr);
}