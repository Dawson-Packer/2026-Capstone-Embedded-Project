#pragma once
#include <stdint.h>

namespace fs
{

    class Filesystem
    {
      public:
        Filesystem();
        ~Filesystem();

      private:
        uint8_t    _initialized = 0;
        const char datafile[9]  = "data.csv";
    };
} // namespace fs
