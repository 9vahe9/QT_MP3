#include <iostream>
#include <type_traits>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <exception>

template <typename T>
class Allocator
{
private:
    using value_type = T;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_referece = const value_type&;
    using size_type = std::size_t;

private:
    struct MemoryBlock
    {
    private:
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using size_type = std::size_t;

    public:
        pointer ptr;
        size_type size;
    };

private:
    pointer m_Buffer = nullptr;
    size_type m_Size = 0;
    std::vector<MemoryBlock> m_FreeList;
    std::vector<MemoryBlock> m_Pointers;

public:
    Allocator(const size_type sizeInBytes) : m_Size(sizeInBytes)
    {
        m_Buffer = (pointer)std::malloc(sizeInBytes);
    }
    ~Allocator()
    {
        if (m_Buffer)
        {
            std::free(m_Buffer);
            m_Buffer = nullptr;
            m_Size = 0;
        }
    }

public:
    pointer allocate(const size_type count = 1)
    {
        if (m_FreeList.empty())
        {
            new (m_Buffer) value_type[count];
            m_Pointers.push_back(MemoryBlock{ .ptr = m_Buffer, .size = count * sizeof(value_type) });
            m_FreeList.push_back(MemoryBlock{ .ptr = m_Buffer + count, .size = m_Size - count * sizeof(value_type) });
            return m_Buffer;
        }
        else
        {
            // Grab the very first free pointer in our list.
            auto it = std::find_if(m_FreeList.rbegin(), m_FreeList.rend(), 
                [count](const auto& e)
                {
                    return e.size >= count * sizeof(value_type);
                });

            // This should be true unless our allocator has run out of memory.
            if (it != m_FreeList.rend())
            {
                pointer ptr = nullptr;
                if (m_FreeList.size() > 1)
                {
                    if (it->size == count * sizeof(value_type))
                    {
                        const auto block = *it;
                        m_FreeList.erase(std::next(it).base());
                        ptr = block.ptr;
                    }
                    else
                    {
                        ptr = it->ptr;
                        it->ptr += count;
                        it->size -= count * sizeof(value_type);
                    }
                }
                else
                {
                    ptr = it->ptr;
                    it->ptr += count;
                    it->size -= count * sizeof(value_type);
                }

                // Construct our object.
                new (ptr) value_type[count];

                m_Pointers.push_back(MemoryBlock{ .ptr = ptr, .size = count * sizeof(value_type) });
                return ptr;
            }
            else
                throw std::runtime_error("Allocator full.");
        }
    }
    void deallocate(const pointer ptr)
    {
        // Call the destructor if it's a class.
        if constexpr (std::is_class_v<T>)
            ptr->~T();

        // Find the active pointer in our list.
        auto it = std::find_if(m_Pointers.begin(), m_Pointers.end(), 
            [ptr](const auto& e)
            {
                return e.ptr == ptr;
            });
        
        // This should always be true.
        if (it != m_Pointers.end())
        {
            const auto block = *it;
            m_Pointers.erase(it);

            if (m_FreeList.front().ptr - block.size / sizeof(value_type) == block.ptr)
            {
                auto& free_block = m_FreeList.front();
                free_block.ptr = block.ptr;
                free_block.size += block.size;
            }
            else
            {
                // It's a gap.
                m_FreeList.push_back(block);
            }
        }
        else
            throw std::runtime_error("Bad pointer.");
    }
};

struct MyClass
{
    MyClass() { std::cout << "ctor\n"; }
    ~MyClass() { std::cout << "dtor\n"; }
};

int main()
{
    Allocator<int> alloc(1024);
    auto* ptr = alloc.allocate(5);
    auto* ptr2 = alloc.allocate(2);
    alloc.deallocate(ptr);
    alloc.deallocate(ptr2);
    auto* ptr3 = alloc.allocate(10);
    alloc.deallocate(ptr3);
    auto* ptr4 = alloc.allocate(5);
    alloc.deallocate(ptr4);
    return 0;
}
