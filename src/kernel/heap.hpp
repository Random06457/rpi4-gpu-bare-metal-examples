#pragma once

#include <memory>
#include "bit_utils.hpp"
#include "types.hpp"

#define MALLOC_DEBUG(heap, size, align)                                        \
    (heap)->allocDebug((size), (align), __FILE__, __func__, __LINE__)

class Heap
{
public:
    struct Node
    {
        static constexpr u32 NODE_MAGIC = 0x11223344;

        u32 magic;
        bool is_free;
        std::size_t alloc_size;
        const char* file;
        const char* function;
        std::size_t line;
        Node* next;
        Node* prev;
        Node* next_free;
        Node* prev_free;

        Node() :
            magic(NODE_MAGIC),
            is_free(true),
            alloc_size(0),
            file(nullptr),
            function(nullptr),
            line(0),
            next(nullptr),
            prev(nullptr),
            next_free(nullptr),
            prev_free(nullptr)
        {
        }

        constexpr bool isValid() const { return magic == NODE_MAGIC; }

        static constexpr std::size_t getHeaderSize()
        {
            return bits::align(sizeof(Node), 0x10);
        }

        template<typename T = void>
        constexpr T* getData() const
        {
            return reinterpret_cast<T*>(
                reinterpret_cast<u8*>(const_cast<Node*>(this)) +
                getHeaderSize());
        }

        std::size_t getTotalSize() const
        {
            return reinterpret_cast<u8*>(next) - getData<u8>();
        }

        static Node* getNodeFromData(void* data)
        {
            return reinterpret_cast<Node*>(reinterpret_cast<u8*>(data) -
                                           getHeaderSize());
        }
    };

public:
    Heap(void* addr, std::size_t size);
    ~Heap();

    void* allocDebug(std::size_t size, std::size_t align, const char* file,
                     const char* function, std::size_t line);
    void free(void* ptr);

    Node* findFreeNode(std::size_t size, std::size_t align) const;

    Node* getHead() const { return m_head; }
    Node* getTail() const { return m_tail; }
    Node* getFreeHead() const { return m_free_head; }
    Node* getFreeTail() const { return m_free_tail; }
    void* getDataAddr() const { return m_addr; }
    std::size_t getDataSize() const { return m_size; }

protected:
    void linkNodes(Node* prev, Node* next);
    void linkFreeNodes(Node* prev, Node* next);

    void appendFreeNode(Node* node);
    void removeFreeNode(Node* node);

protected:
    void* m_addr;
    std::size_t m_size;

    Node* m_head;
    Node* m_tail;
    Node* m_free_head;
    Node* m_free_tail;

    // static inline u8 g_main_heap_data[0x10000];
    static u8 g_main_heap_data[];

public:
    static Heap g_main_heap;
};
