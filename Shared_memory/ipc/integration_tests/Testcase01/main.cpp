
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include<unistd.h>

#include "../../shared_memory/shared_memory.hpp"

int main(int argc, char *argv[])
{
    try
    {
        /* code */
    
    if(argc < 2 || argc > 2)
    {
        std::cout<<"Invalid number of parameters: "<<argc<<std::endl;
        return 0;
    }

    

    std::string sharedMemName { argv[1] };

    Ipc::SharedMemory shm(sharedMemName);
    std::string info {"Info"};

    pid_t pid = fork();

    if (pid == -1)
    {
        std::cout<<"error"<<std::endl;
    }
    else if (pid == 0)
    {   
        std::cout<<"Start child"<<std::endl;
        std::vector<uint8_t> infoToBytes(info.begin(), info.end());    
        shm.AppendToTheEnd(infoToBytes);
    
        std::chrono::seconds duration(10);
        std::this_thread::sleep_for(duration);

        //auto memory = shm.ReadMemory();

        //std::cout<<memory.data()<<std::endl;
    }
    else if (pid > 0)
    {
        std::cout<<"Start parent"<<std::endl;
        std::chrono::seconds duration(5);
        std::this_thread::sleep_for(duration);
        auto memory = shm.ReadMemory();

        std::string j{(char*)&(memory.data()[2])};
        for (auto &&i : memory)
        {
            std::cout<<(int) i<<std::endl;
        }
        
        std::cout<<"size: "<<memory.size()<<j<<std::endl;
        shm.ClearData();

    }

    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }

    return 0;
}