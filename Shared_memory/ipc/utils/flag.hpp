#pragma once

#include <bitset>
#include <cstddef>
#include <type_traits>
#include <cassert>
#include <iostream>
#include <exception>
#include <experimental/source_location>

namespace fl
{
    template<typename T, class Result, typename = std::enable_if<std::is_integral<T>::value>>
    class IFlag;

    template<typename Ty, typename R>
    R const operator|(const fl::IFlag<Ty,R> lhs, const fl::IFlag<Ty,R> rhs);

    template<typename Ty, typename R>
    R const operator&(const fl::IFlag<Ty, R> lhs, const fl::IFlag<Ty, R> rhs);

    template<typename T, class Result, typename>
    class IFlag
    {
        public:
            IFlag(){}

            void Set(const T value)
            {
                m_value.set() = std::bitset<sizeof(T)* m_bitsInBytes>(value);
            }

            IFlag& operator=(const IFlag& flag)
            {
                this->m_value = flag.m_value;
                return *this;
            }

            IFlag& operator=(const T value)
            {
                this->m_value = value;
                return *this;
            }

            auto operator|(const IFlag& rhs)
            {
                this->m_value |= rhs.m_value;
                return Result(this->m_value);
            }
            
            auto operator&(const IFlag& rhs)
            {
                this->m_value &= rhs.m_value;
                return Result(this->m_value);
            }

            auto& operator<<(const unsigned int offset)
            {
                this->m_value<<offset;
                return *this;
            }

            auto& operator>>(const unsigned int offset)
            {
                this->m_value>>offset;
                return *this;
            }

            operator int() const
            {
                ControlFunc<int>();
                return static_cast<int>(m_value.to_ulong());
            }

            operator uint32_t() const
            {
                ControlFunc<uint32_t>();
                return static_cast<uint32_t>(m_value.to_ulong());
            }

            operator uint8_t() const
            {
                ControlFunc<uint8_t>();
                return static_cast<uint8_t>(m_value.to_ulong());
            }

            operator uint16_t() const
            {
                ControlFunc<uint16_t>();
                return static_cast<uint16_t>(m_value.to_ulong());
            }

            operator std::byte() const
            {
                ControlFunc<std::byte>();
                return static_cast<std::byte>(m_value.to_ulong());
            }

            template<typename Ty, typename R>
            friend R const operator|(const fl::IFlag<Ty,R> lhs, const fl::IFlag<Ty,R> rhs);

            template<typename Ty, typename R>
            friend R const operator&(const fl::IFlag<Ty,R> lhs, const fl::IFlag<Ty,R> rhs);

        protected:
            static constexpr int m_bitsInBytes = 8;
            std::bitset<sizeof(T) * m_bitsInBytes> m_value;

        private:
        
            template<typename type>
            void ControlFunc() const throw()
            {
                if(sizeof(T) > sizeof(type))
                {
                    throw std::runtime_error("Casting with loosing data!");
                }
            }

    };
    
    template<typename Ty, typename R>
    R const operator|(const fl::IFlag<Ty,R> lhs, const fl::IFlag<Ty,R> rhs)
    {
        R tmp((lhs.m_value | rhs.m_value).to_ulong()); 
        return tmp;
    }
    
    template<typename Ty, typename R>
    R const operator&(const fl::IFlag<Ty, R> lhs, const fl::IFlag<Ty, R> rhs)
    {
        R tmp((lhs.m_value & rhs.m_value).to_ulong());
        return tmp;        
    }

    class CreationFlag final : public IFlag<uint8_t, CreationFlag>
    {
        public:
            CreationFlag(const uint8_t value) : IFlag()
            {
                Set(value);
            }

            CreationFlag(const CreationFlag& creationFlag)
            {
                this->m_value = creationFlag.m_value;
            }

            CreationFlag(const std::bitset<sizeof(uint8_t) * m_bitsInBytes>& bitset)
            {
                m_value = bitset;
            }
    };
}