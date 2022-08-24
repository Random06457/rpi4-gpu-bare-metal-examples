#include <new>
#include "kernel/heap.hpp"

void* operator new(std::size_t size)
{
    return MALLOC_DEBUG(&Heap::g_main_heap, size, 1);
}

void* operator new(std::size_t size, std::align_val_t align)
{
    return MALLOC_DEBUG(&Heap::g_main_heap, size, static_cast<size_t>(align));
}

void* operator new[](std::size_t size)
{
    return MALLOC_DEBUG(&Heap::g_main_heap, size, 1);
}

void* operator new[](std::size_t size, std::align_val_t align)
{
    return MALLOC_DEBUG(&Heap::g_main_heap, size, static_cast<size_t>(align));
}

void operator delete[](void* ptr)
{
    Heap::g_main_heap.free(ptr);
}

void operator delete(void* ptr)
{
    Heap::g_main_heap.free(ptr);
}

void operator delete[](void* ptr, std::size_t size)
{
    Heap::g_main_heap.free(ptr);
}

void operator delete(void* ptr, std::size_t size)
{
    Heap::g_main_heap.free(ptr);
}
