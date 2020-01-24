#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <stddef.h>
#include "vector.hpp"

namespace gb7
{
    template<class T, size_t N = 16, template<class, size_t> class Container = vector>
    class priority_queue
    {
        struct item
        {
            T data;
            int id;
        };
        Container<item, N> arr;
        int count = 1;

    public:
        int push(const T& v) noexcept
        {
            int child = arr.size();
            if (!arr.push({ v, count })) return 0;

            while (child != 0)
            {
                int parent = (child - 1) / 2;

                if (arr[child].data < arr[parent].data)
                {
                    swap(arr[child], arr[parent]);
                }
                child = parent;
            }
            return count++;
        }

        bool pop() noexcept
        {
            if (empty()) return false;

            {
                item temp;
                arr.pop(temp);
                arr[0] = move(temp);
            }

            int n = arr.size();
            int parent = 0, child;
            while ((child = 2 * parent + 1) < n)
            {
                if (child + 1 < n && arr[child].data > arr[child + 1].data)
                    child++;
                
                if (arr[parent].data > arr[child].data)
                    swap(arr[parent], arr[child]);

                parent = child;
            }
            return true;
        }

        bool update_top(const T& v) noexcept
        {
            if (empty()) return false;

            arr[0].data = move(v);

            int n = arr.size();
            int parent = 0, child;
            while ((child = 2 * parent + 1) < n)
            {
                if (child + 1 < n && arr[child].data > arr[child + 1].data)
                    child++;
                
                if (arr[parent].data > arr[child].data)
                    swap(arr[parent], arr[child]);

                parent = child;
            }
            return true;
        }

        bool top(T& ret) const noexcept
        {
            if (empty()) return false;

            ret = arr[0].data;
            return true;
        }

        bool erase(int id) noexcept
        {
            int i = arr.find([id](const item& v) { return v.id == id; });
            if (i == -1) return false;

            {
                item temp;
                arr.pop(temp);
                arr[i] = move(temp);
            }

            int n = arr.size();
            int parent = i, child;
            while ((child = 2 * parent + 1) < n)
            {
                if (child + 1 < n && arr[child].data > arr[child + 1].data)
                    child++;
                
                if (arr[parent].data > arr[child].data)
                    swap(arr[parent], arr[child]);

                parent = child;
            }

            return true;
        }

        size_t size() const noexcept
        {
            return arr.size();
        }

        bool empty() const noexcept
        {
            return size() == 0;
        }
    };
} // namespace gb7

#endif // PRIORITY_QUEUE_HPP
