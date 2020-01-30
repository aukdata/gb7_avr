#ifndef UTILS_HPP
#define UTILS_HPP

#include <stddef.h>
#include <stdint.h>

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


template<class T>
inline T min(T a, T b)
{
    return a < b ? a : b;
}

template<class T>
inline T max(T a, T b)
{
    return a > b ? a : b;
}

template<class T>
struct remove_reference { typedef T type; };

template<class T>
constexpr typename remove_reference<T>::type&& move(T&& t) noexcept
{
    return static_cast<typename remove_reference<T>::type&&>(t);
}

template <class T>
inline void swap(T& a, T& b) noexcept
{
  T t = move(a);
  a = move(b);
  b = move(t);
}

inline int popcount(uint32_t x)
{
    auto bit = x - ((x >> 1) & 0x5555u);
    bit = (bit & 0x3333) + ((bit >> 2) & 0x3333);
    bit = (bit + (bit >> 4)) & 0x0f0f;
    bit = (bit + (bit >> 8)) & 0x00ff;
    return bit;
}

void delay_ms(int miliseconds) noexcept;

#endif // UTILS_HPP
