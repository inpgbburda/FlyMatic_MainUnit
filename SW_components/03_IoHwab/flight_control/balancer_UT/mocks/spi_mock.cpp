#include "spi.hpp"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

Spi::Spi()
{
}

void Spi::Init(const int hw_spi, int speed)
{
}

void Spi::ReadWriteData(const int channel, unsigned char *buffer, int length)
{
    mock().actualCall("ReadWriteData")
          .withParameter("channel", channel)
          .withMemoryBufferParameter("buffer", buffer, length)
          .withParameter("length", length);
}

Spi::~Spi()
{
}
