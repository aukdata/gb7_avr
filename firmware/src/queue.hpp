#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stddef.h>

namespace gb7
{
    template<class T, size_t N = 16>
    class queue
    {
        T arr[N];
        size_t head = 0, tail = 0, m_size = 0;

    public:
        bool push(const T&& v) noexcept
        {
            if (m_size >= N) return false;
            
            arr[tail] = v;
            tail = (tail + 1) % N;
            m_size++;
            return true;
        }

        bool pop(T& ret) noexcept
        {
            if (m_size == 0) return false;

            auto head_temp = head;
            head = (head + 1) % N;
            ret = static_cast<T&&>(arr[head_temp]);
            m_size--;
            return true;
        }

        size_t size() const noexcept
        {
            return m_size;
        }

        template<class Func>
        void each_of(Func func)
        {
            for (size_t i=0; i < m_size; i++)
            {
                func(arr[(head + i) % N]);
            }
        }
    };    
} // namespace gb7

#endif // QUEUE_HPP