#pragma once

#include "flag.hpp"

namespace Ipc
{
    constexpr int errorValue = -1;

    class CreationFlagsShm
    {
    public:
        static const fl::CreationFlag Create;
        static const fl::CreationFlag Exclusive;
        static const fl::CreationFlag ReadOnly;
        static const fl::CreationFlag ReadWrite;
        static const fl::CreationFlag Truncate;
    };

    class CreationFlagsSem
    {
    public:
        static const fl::CreationFlag Create;
        static const fl::CreationFlag Exclusive;
    };
}