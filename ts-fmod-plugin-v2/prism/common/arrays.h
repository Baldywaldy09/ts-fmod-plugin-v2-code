#pragma once

#include <stdexcept>

#pragma pack(push, 1)
namespace prism
{
	class array_dyn_t // Size: 0x0020
	{
		void* items;      //0x0008 (0x08)
	public:
		uint64_t size;     //0x0010 (0x08)
		uint64_t capacity; //0x0018 (0x08)

		void* operator[](uint64_t index) {
			if (!has_index(index))
				throw std::out_of_range("Index out of range");

			printf("[Prism3D] array_dyn_t: Getting index: %llu\n", index);

			return static_cast<void**>(items)[index];
		}

		// Unknown
		virtual void initialize();

		// literally deletes all items
		virtual uint64_t clear();

		// Moves the array items list to a different memory location
		virtual uint64_t reallocate();

		// Returns true or false depending on if a index is out of range or not
		virtual bool has_index(uint64_t index);


		virtual void Function4();
		virtual void Function5();
		virtual void Function6();


		virtual void get_parent_unit_descriptor();
		virtual void Function8();
		virtual void set_parent_unit_attributes();
		virtual void Function10();
		virtual void Function11();
		virtual void Function12();
		virtual void Function13();
		virtual void Function14();
		virtual void Function15();
		virtual void Function16();
		virtual void Function17();
		virtual void Function18();
		virtual void Function19();
		virtual void Function20();
		virtual void Function21();
		virtual void Function22();
		virtual void Function23();
		virtual void Function24();
		virtual void Function25();
		virtual void Function26();
		virtual void Function27();
		virtual void Function28();
		virtual void Function29();
	};
	static_assert(sizeof(array_dyn_t) == 0x20);
};
#pragma pack(pop)