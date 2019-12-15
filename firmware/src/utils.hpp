#include <stddef.h>

#ifndef UTILS_HPP
#define UTILS_HPP

__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C"
{
    int __cxa_guard_acquire(__guard* g);
    void __cxa_guard_release(__guard* g);
    void __cxa_guard_abort(__guard*);
    void __cxa_pure_virtual();
}


void* operator new(size_t size);
void operator delete(void* ptr, size_t size);

void* operator new[](size_t size);
void operator delete[](void* ptr, size_t size);


template<typename T>
inline T min(T a, T b)
{
    return a < b ? a : b;
}

template<typename T>
inline T max(T a, T b)
{
    return a > b ? a : b;
}

#endif // UTILS_HPP
