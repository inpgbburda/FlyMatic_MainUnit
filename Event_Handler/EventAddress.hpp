#pragma once

#include <string>
#include <unistd.h>
#include <iostream>

#include "AddressLabels.hpp"

namespace system_address
{    

enum class Priority
{
    Low,
    High,
    Emergency
};

using Address = unsigned long long int;

using Parameters = struct
{
    uint32_t modId = 0;
    uint16_t label = 0;
    uint8_t priority = 0;
    uint8_t option = 0;
};


class AddressManager
{

public:

static Address MakeAddress()
{   
    return MakeAddress({});
}

static Address MakeAddress(const Parameters& param)
{
    static_assert(sizeof(Parameters) == sizeof(Address), "Parameters' size must be equal Address size!");
    constexpr int bitOffset = 8;

    Address tmpAddr = 0;
    
    tmpAddr = tmpAddr | static_cast<Address>(param.modId);
    tmpAddr = tmpAddr << sizeof(param.label)*bitOffset | static_cast<Address>(param.label);
    tmpAddr = tmpAddr << sizeof(param.priority)*bitOffset | static_cast<Address>(param.priority);
    tmpAddr = tmpAddr << sizeof(param.option)*bitOffset | static_cast<Address>(param.option);
    return tmpAddr;
}

static std::string printOut(const Address& address)
{
    return std::string{ "ModuleId: "+ std::to_string(GetModId(address)) +"LabelNumber: " + std::to_string(GetLabel(address)) 
    + "Priority: " + std::to_string(GetPriority(address)) + " | Option: " + std::to_string(GetOption(address))};
}

static typeof(Parameters::modId) GetModId(const Address& address)
{
    constexpr Address m_ModIdMask = 0xFFFFFFFF00000000;
    constexpr int bitShift = 32;
    return static_cast<typeof(Parameters::modId)>((address & m_ModIdMask) >> bitShift);
}

static typeof(Parameters::label) GetLabel(const Address& address)
{
    constexpr Address m_labelMask = 0x00000000FFFF0000;
    constexpr int bitShift = 16;
    return static_cast<typeof(Parameters::label)>((address & m_labelMask) >> bitShift);
}

static typeof(Parameters::priority) GetPriority(const Address& address)
{
    constexpr Address m_prorityMask = 0x000000000000FF00;
    constexpr int bitShift = 8;
    return static_cast<typeof(Parameters::priority)>((address & m_prorityMask) >> bitShift);
}

static typeof(Parameters::option) GetOption(const Address& address)
{
    constexpr Address m_optionMask = 0x00000000000000FF;
    return static_cast<typeof(Parameters::option)>(address & m_optionMask);
}

static void SetModId(Address& address, const typeof(Parameters::modId) modId)
{
    Address tmpAddr = address;
    address = MakeAddress({.modId = modId, .label = GetLabel(address), .priority = GetPriority(address), .option = GetOption(address)});
}

static void SetLabel(Address& address, const typeof(Parameters::label) label)
{
    Address tmpAddr = address;
    address = MakeAddress({.modId = GetPriority(address), .label = label, .priority = GetPriority(address), .option = GetOption(address)});
}

static void SetPriority(Address& address, const typeof(Parameters::priority) priority)
{
    Address tmpAddr = address;
    address = MakeAddress({.modId = GetPriority(address), .label = GetLabel(address), .priority = priority, .option = GetOption(address)});
}

static void SetOption(Address& address, const typeof(Parameters::option) option)
{
    Address tmpAddr = address;
    address = MakeAddress({.modId = GetPriority(address), .label = GetLabel(address), .priority = GetPriority(address), .option = option});
}

static typeof(Parameters::label) LabelToParam(const AddressLabels label)
{
    return static_cast<typeof(Parameters::label)>(label);
}

static uint32_t GetPID()
{
    return static_cast<uint32_t>(getppid());
}

};

}