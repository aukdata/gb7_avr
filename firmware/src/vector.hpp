#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <stddef.h>
#include "utils.hpp"

namespace gb7
{
    template<class T, size_t N = 16>
    class vector
    {
        T arr[N];
        size_t top = 0;

    public:
        bool push(const T& v) noexcept
        {
            if (top >= N) return false;

            arr[top] = v;
            top++;
            return true;
        }

        bool pop(T& ret) noexcept
        {
            if (top == 0) return false;

            top--;
            ret = move(arr[top]);
            return true;
        }

        const T& operator[](size_t i) const noexcept
        {
            return arr[i];
        }

        T& operator[](size_t i) noexcept
        {
            return arr[i];
        }

        template<class Function>
        size_t find(Function comparator) const noexcept
        {
            for (size_t i=0, s=size(); i < s; i++)
            {
                if (comparator((*this)[i]))
                {
                    return i;
                }
            }
            return -1;
        }

        size_t size() const noexcept
        {
            return top;
        }
    };
} // namespace gb7

#endif // VECTOR_HPP
