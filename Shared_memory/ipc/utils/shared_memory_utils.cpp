#include <fcntl.h>
#include "shared_memory_utils.hpp"

namespace Ipc
{
    const fl::CreationFlag CreationFlagsShm::Create = O_CREAT;
    const fl::CreationFlag CreationFlagsShm::Exclusive = O_EXCL;
    const fl::CreationFlag CreationFlagsShm::ReadOnly = O_RDONLY;
    const fl::CreationFlag CreationFlagsShm::ReadWrite = O_RDWR;
    const fl::CreationFlag CreationFlagsShm::Truncate = O_TRUNC;

    const fl::CreationFlag CreationFlagsSem::Create = O_CREAT;
    const fl::CreationFlag CreationFlagsSem::Exclusive = O_EXCL;
}
