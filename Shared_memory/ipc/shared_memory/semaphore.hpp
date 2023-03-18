#pragma once

#include "../utils/flag.hpp"
#include "shared_memory_utils.hpp"

namespace Ipc
{

class Semaphore
    {
        public:
            Semaphore(const std::string name, const fl::CreationFlag& creationFlags) : name(name)
            {
                semaphore = std::make_shared<sem_t*>(sem_open(name.c_str(), static_cast<int>(creationFlags), S_IRUSR | S_IWUSR | S_IXUSR, 0));
                if(*semaphore.get() == SEM_FAILED)
                {
                    throw std::runtime_error(std::string { "Unable to create semaphore: " } + std::string { strerror(errno) } );
                }
            }

            void Post()
            {
                if (sem_post(*semaphore.get()) == errorValue)
                {
                    throw std::runtime_error("Unable to post semaphore.");
                }
            }

            void Wait(bool nonBlocking = false, const long milisecTimeout = 0)
            {   
                if (!nonBlocking)
                {
                    if (sem_wait(*semaphore.get()) == errorValue)
                    {
                        throw std::runtime_error("Unable to wait blocking semaphore.");
                    }
                }
                else if (milisecTimeout == 0)
                {
                    auto result = sem_trywait(*semaphore.get());
                    if (result != EAGAIN && result != 0)
                    {
                        throw std::runtime_error("Unable to wait nonblocking semaphore.");
                    }
                }
                else
                {
                    struct timespec abs_timeout;
                    abs_timeout.tv_nsec = milisecTimeout;
                    auto result = sem_timedwait(*semaphore.get(), &abs_timeout);

                    if (result != EAGAIN && result != 0)
                    {
                        throw std::runtime_error("Unable to wait nonblocking semaphore.");
                    }
                }
            }

            ~Semaphore()
            {
                if (sem_unlink(name.c_str()) == errorValue)
                {
                    throw std::runtime_error(std::string{ "Unable to unlink semaphore. " } + std::string{ strerror(errno) });
                }
            }

            private:

            std::shared_ptr<sem_t*> semaphore;
            std::string name;
    };
}