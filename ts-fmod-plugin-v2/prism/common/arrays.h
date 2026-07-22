#pragma once
#include <cstdint>
#include <stdexcept>

#pragma pack(push, 1)
namespace prism
{
	class array_t
	{
	public:
		void* items;      //0x0008

		uint64_t size;     //0x0010
		uint64_t capacity; //0x0018

		void* unk; // 0x0020

		// Unknown
		virtual void destructor() {}

		// Delete all items
		virtual uint64_t clear() { return 0; }

		// Moves the array items list to a different memory location
		virtual uint64_t reallocate() { return 0; }

		virtual bool allocate(uint64_t new_size) { return 0; }
	};

	template<typename T = void*>
	class array_dyn_t : public array_t // Size: 0x0028
	{
	public:
		array_dyn_t(void* vtable_address)
		{
			*(void**)this = vtable_address;
		}

		void push_back(T item)
		{
			if (size == capacity) {
				allocate(size + 1);
			}

			reinterpret_cast<T*>(items)[size] = item;
			++size;
		}

		T& operator[](uint64_t index)
		{
			if (index >= size)
				throw std::out_of_range("index");

			return reinterpret_cast<T*>(items)[index];
		}

		const T& operator[](uint64_t index) const
		{
			if (index >= size)
				throw std::out_of_range("index");

			return reinterpret_cast<const T*>(items)[index];
		}

		struct iterator {
			T* ptr;

			T& operator*() const { return *ptr; }
			iterator& operator++() { ++ptr; return *this; }
			bool operator!=(const iterator& other) const { return ptr != other.ptr; }
		};

		iterator begin() {
			return { reinterpret_cast<T*>(items) };
		}

		iterator end() {
			return { reinterpret_cast<T*>(items) + size };
		}
	};
	static_assert(sizeof(array_dyn_t<>) == 0x28);
};
#pragma pack(pop)