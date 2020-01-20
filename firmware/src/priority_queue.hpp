#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <stddef.h>
#include "vector.hpp"

namespace gb7
{
    template<class T, size_t N = 16, template<class, size_t> class Container = vector>
    class priority_queue
    {
        Container<T, N> arr;

    public:
        bool push(const T& v) noexcept
        {
            int child = arr.size();
            if (!arr.push(v)) return false;

            while (child != 0)
            {
                int parent = (child - 1) / 2;

                if (arr[child] < arr[parent])
                {
                    swap(arr[child], arr[parent]);
                }
                child = parent;
            }
            return true;
        }

        bool pop() noexcept
        {
            if (size() == 0) return false;

            {
                T temp;
                arr.pop(temp);
                arr[0] = move(temp);
            }

            int n = arr.size();
            int parent = 0, child;
            while ((child = 2 * parent + 1) < n)
            {
                if (child + 1 < n && arr[child] > arr[child + 1])
                    child++;
                
                if (arr[parent] > arr[child])
                    swap(arr[parent], arr[child]);

                parent = child;
            }
            return true;
        }

        bool top(T& ret) const noexcept
        {
            if (size() == 0) return false;

            ret = arr[0];
            return true;
        }

        size_t size() const noexcept
        {
            return arr.size();
        }
    };
} // namespace gb7

#endif // PRIORITY_QUEUE_HPP
