#pragma once

#include <cstdint>
#include <cstdio>

namespace Ipc
{

  class PtrToShm
        {
        public:
            PtrToShm() {}

            PtrToShm(void *shm) : m_shm(shm) {}

            PtrToShm(const PtrToShm& ptrToShm)
            {
                this->m_shm = ptrToShm.m_shm;
            }

            PtrToShm& operator=(void* ptrToShm)
            {
                this->m_shm = ptrToShm;
                return *this;
            }

            PtrToShm& operator=(const PtrToShm& ptrToShm)
            {
                this->m_shm = ptrToShm.m_shm;
                return *this;
            }

            uint8_t *operator->() const
            {
                return reinterpret_cast<uint8_t*>(m_shm);
            }

            void* operator*()
            {
                return m_shm;
            }

            PtrToShm operator+(const int64_t shift)
            {
                auto tmpPtr = reinterpret_cast<uint8_t*>(m_shm) + shift;
                return PtrToShm(reinterpret_cast<void*>(tmpPtr));
            }

            PtrToShm operator-(const int64_t shift)
            {
                auto tmpPtr = reinterpret_cast<uint8_t*>(m_shm) - shift;
                return PtrToShm(reinterpret_cast<void*>(tmpPtr));
            }
            
            size_t operator-(const PtrToShm& ptr)
            {
                if (ptr.m_shm >= this->m_shm)
                {
                    return 0;
                }

                return reinterpret_cast<uint8_t*>(this->m_shm) - reinterpret_cast<uint8_t*>(ptr.m_shm);
            }
            
            bool operator!=(const PtrToShm& ptr)
            {
                return this->m_shm != ptr.m_shm;
            }

            bool operator==(const PtrToShm& ptr)
            {
                return this->m_shm == ptr.m_shm;
            }

            uint8_t* getPtr()
            {
                return reinterpret_cast<uint8_t*>(m_shm);
            }

            void *m_shm;
        };
}