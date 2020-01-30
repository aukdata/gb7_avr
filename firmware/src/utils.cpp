#include <stdlib.h>
#include <util/delay.h>

#include "utils.hpp"


int __cxa_guard_acquire(__guard* g)
{
    return !*reinterpret_cast<char*>(g);
}
void __cxa_guard_release(__guard* g)
{
    *reinterpret_cast<char*>(g) = 1;
}
void __cxa_guard_abort(__guard*g ) {}
void __cxa_pure_virtual() {}


void* operator new(size_t size)
{
    return malloc(size);
}
void operator delete(void* ptr, size_t size)
{
    free(ptr);
}

void* operator new[](size_t size)
{
    return malloc(size);
}
void operator delete[](void* ptr, size_t)
{
    free(ptr);
}

void delay_ms(int miliseconds) noexcept
{
    for (int i = 0; i < miliseconds; i++)
        _delay_ms(1);
}
