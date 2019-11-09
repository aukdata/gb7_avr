#include <stddef.h>

inline void* operator new(size_t size);
inline void operator delete(void* ptr, size_t size);

inline void* operator new[](size_t size);
inline void operator delete[](void* ptr, size_t size);
