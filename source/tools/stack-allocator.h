#pragma once

class StackAllocator;

typedef unsigned char byte;

#include <utility>

class StackAllocator {
	private:
	byte* m_stackBuffer;
	uint32_t m_size;
	uint32_t m_currentCapacity;

public:
	StackAllocator() = delete;
	StackAllocator(const StackAllocator& other) = delete;
	StackAllocator(StackAllocator&& other) = delete;
	~StackAllocator() { }
	StackAllocator(byte* buffer, uint32_t size) : m_stackBuffer(buffer), m_size(size) { }

	template <typename T>
	inline T* construct(T&& obj) {
		T* alloc = new(m_stackBuffer + m_currentCapacity) T(std::move(obj));
		m_currentCapacity += sizeof(T);
		return alloc;
	}

	template <typename T>
	void deallocate(T* obj, bool destruct) {
		if (destruct) {
			obj->~T();
		}
	}

	inline void clearMemory() {
		m_currentCapacity = 0;
	}
};
