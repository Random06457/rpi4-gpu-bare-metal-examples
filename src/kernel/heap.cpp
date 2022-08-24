#include "heap.hpp"
#include <cassert>
#include <cstring>

Heap Heap::g_main_heap = Heap(Heap::g_main_heap_data, 0x10000000);

Heap::Heap(void* addr, size_t size) :
    m_addr(addr),
    m_size(size),
    m_head(reinterpret_cast<Node*>(addr)),
    m_tail(reinterpret_cast<Node*>(reinterpret_cast<u8*>(addr) + size)),
    m_free_head(reinterpret_cast<Node*>(addr)),
    m_free_tail(reinterpret_cast<Node*>(addr))

{
    m_head->magic = Node::NODE_MAGIC;
    m_head->is_free = true;
    m_head->alloc_size = 0;
    m_head->file = nullptr;
    m_head->function = nullptr;
    m_head->line = 0;
    m_head->next = m_tail;
    m_head->prev = nullptr;
    m_head->next_free = nullptr;
    m_head->prev_free = nullptr;
}

Heap::~Heap()
{
}

Heap::Node* Heap::findFreeNode(std::size_t size, std::size_t align) const
{
    for (auto node = m_free_head; node != nullptr; node = node->next_free)
    {
        assert(node->is_free && "HEAP CORRUPTION");

        auto data = node->getData();
        std::size_t space = node->getTotalSize();
        if (std::align(align, size, data, space) != nullptr)
            return node;
    }
    return nullptr;
}

static bool isValidAlign(std::size_t align)
{
    for (std::size_t i = 0; i < sizeof(std::size_t) * 8; i++)
    {
        if ((1ull << i) == align)
            return true;
    }
    return false;
}

void* Heap::allocDebug(std::size_t size, std::size_t align, const char* file,
                       const char* function, size_t line)
{
    assert(isValidAlign(align));

    if (align < 0x10)
        align = 0x10;

    Node* node = findFreeNode(size, align);

    assert(node && "ran out of memory");
    assert(node->is_free && node->isValid() && "HEAP CORRUPTION");

    // align node
    void* data = node->getData();
    std::size_t space = node->getTotalSize();
    auto aligned_data =
        reinterpret_cast<u8*>(std::align(align, size, data, space));
    assert(aligned_data && "???");
    Node* aligned_node = Node::getNodeFromData(aligned_data);
    std::size_t diff = aligned_data - node->getData<u8>();

    // copy header content
    std::memmove(aligned_node, node, sizeof(Node));

    // create new node if diff is big enough
    if (diff > Node::getHeaderSize())
    {
        linkNodes(aligned_node, node->next);
        linkNodes(node, aligned_node);
        node = aligned_node;
    }
    // adjust links
    else
    {
        node = aligned_node;
        removeFreeNode(node);
        linkNodes(node, node->next);
        linkNodes(node->prev, node);
    }

    // split the node if necessary
    std::size_t aligned_size = bits::align(size, 0x10);
    if (node->getTotalSize() > aligned_size + Node::getHeaderSize())
    {
        auto new_node =
            reinterpret_cast<Node*>(node->getData<u8>() + aligned_size);
        *new_node = Node();

        // link new node
        linkNodes(new_node, node->next);
        linkNodes(node, new_node);
        appendFreeNode(new_node);
    }

    node->prev_free = nullptr;
    node->next_free = nullptr;
    node->is_free = false;
    node->alloc_size = size;
    node->file = file;
    node->function = function;
    node->line = line;

    return node->getData();
}

void Heap::free(void* ptr)
{
    Node* node = Node::getNodeFromData(ptr);
    assert(node->isValid());

    // if prev is free
    if (node->prev != nullptr && node->prev->is_free)
    {
        removeFreeNode(node->prev);
        // link node
        linkNodes(node->prev, node->next);
        node = node->prev;
    }

    // if next is free
    if (node->next != m_tail && node->next->is_free)
    {
        removeFreeNode(node->next);
        linkNodes(node, node->next->next);
    }

    // if head was shifted
    if (node == m_head && node != m_addr)
    {
        std::memmove(m_addr, m_head, sizeof(Node));
        m_head = reinterpret_cast<Node*>(m_addr);
        linkNodes(m_head, m_head->next);
        node = m_head;
    }

    appendFreeNode(node);

    node->is_free = true;
    node->line = 0;
    node->function = nullptr;
    node->file = nullptr;
    node->alloc_size = 0;
}

void Heap::linkNodes(Node* prev, Node* next)
{
    if (prev != nullptr)
        prev->next = next;
    else
        m_head = next;
    if (next != m_tail)
        next->prev = prev;
    // else
    // m_tail = prev;
}

void Heap::appendFreeNode(Node* node)
{
    linkFreeNodes(m_free_tail, node);
    m_free_tail = node;
}

void Heap::removeFreeNode(Node* node)
{
    linkFreeNodes(node->prev_free, node->next_free);
}

void Heap::linkFreeNodes(Node* prev, Node* next)
{
    if (prev != nullptr)
        prev->next_free = next;
    else
        m_free_head = next;
    if (next != nullptr)
        next->prev_free = prev;
    else
        m_free_tail = prev;
}