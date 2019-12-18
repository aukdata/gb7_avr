#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <stdlib.h>

#include <avr/eeprom.h> 

static uint32_t seed EEMEM;

namespace gb7
{
    class random
    {
    public:
        random() // this takes some milliseconds
        {
            eeprom_busy_wait();
            srand(eeprom_read_dword(&seed));

            eeprom_busy_wait();
            eeprom_write_dword(&seed, static_cast<uint32_t>(rand()));
        }

        inline int operator()(int max_val)
        {
            return rand() % max_val;
        }
    };
}

#endif // RANDOM_HPP
