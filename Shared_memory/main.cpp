#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>

constexpr int DESCRIPTOR_SIZE = 2048;

int main()
{

    int descriptor = shm_open("EventTab", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    ftruncate(descriptor, DESCRIPTOR_SIZE);

    auto addr = mmap(NULL, DESCRIPTOR_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, descriptor, 0);

    if (addr == MAP_FAILED)
        std::cout<<"Error"<<std::endl;

    struct stat descInfo;
    fstat(descriptor, &descInfo);

    std::cout<<"Allocated size: "<<descInfo.st_size<<" Owner: "<<descInfo.st_uid<<std::endl;

    std::string napis{"janek marysia"};

    memcpy(addr, napis.c_str(), napis.size()+1);

    sleep(30000);

    return 0;
}