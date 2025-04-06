// "bmem" - Baldy-Memory 
// Custom memory editing functions

#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include <vector>

#pragma comment(lib, "../Lib/minhook.x64.lib")
#include "MinHook.h"

namespace bmem
{
	namespace
	{
		// custom byte type
		class pattern_byte
		{
		public:
			uint8_t byte;
			bool ignore;
		};

		inline uintptr_t moduleBase;
		inline uint64_t moduleSize;

		inline bool wasModuleSet = false;
	}

	static bool setModule(const char* module)
	{
		if (std::string(module) == "current")
		{
			if (!wasModuleSet)
				moduleBase = (uintptr_t)GetModuleHandleA(nullptr);
			else
				return true;
		}
		else
			moduleBase = (uintptr_t)GetModuleHandleA(module);


		if (!moduleBase)
		{
			printf("[BMEM] Failed to set module to: '%s'. Module not found! | Did you mean '%s.dll' or '%s.exe'?\n", module, module, module);
			return false;
		}

		const auto* header = (IMAGE_DOS_HEADER*)moduleBase;
		const auto* nt_header = (IMAGE_NT_HEADERS64*)((uint8_t*)header + header->e_lfanew);

		moduleSize = nt_header->OptionalHeader.SizeOfImage;

		printf("[BMEM] Module set to: '%s' (Base: 0x%llx, Size: %llu)\n", module, moduleBase, moduleSize);

		wasModuleSet = true;
		return true;
	}



	static uintptr_t patternScan(const char* patternSTR, const char* moduleToSet = "current")
	{
		//printf("[BMEM] bmem::patternScan: Finding pattern '%s'\n", patternSTR);

		if (!setModule(moduleToSet))
		{
			printf("[BMEM] bmem::patternScan: Failed to initialize a module\n");
			return 0;
		}

		std::vector<pattern_byte> pattern;

		std::istringstream stream(patternSTR);
		std::string token;
		while (stream >> token) {
			pattern_byte pbyte;

			if (token == "??" || token == "?") {
				pbyte.ignore = true;
			}
			else
			{
				if (token.length() > 2 || token.length() < 2)
				{
					printf("[BMEM] bmem::patternScan: Invalid token: %s\n", token.c_str());
					return 0;
				}

				pbyte.ignore = false;

				unsigned int byte;
				std::istringstream(token) >> std::hex >> byte;
				pbyte.byte = (uint8_t)byte;
			}

			pattern.push_back(pbyte);
		}


		if (pattern[0].ignore)
		{
			//printf("[BMEM] bmem::patternScan: Invalid byte (0)! | The first byte should never be a wildcard!\n");
			return 0;
		}


		bool foundFirstByte = false;
		int patternIndex = 0;
		uintptr_t patternStart = 0;
		for (uint64_t i = 0; i < moduleSize; i++)
		{
			uintptr_t currentAddress = moduleBase + i;
			uint8_t currentByte = *reinterpret_cast<uint8_t*>(currentAddress);

			if (!foundFirstByte)
			{
				if (currentByte == pattern[patternIndex].byte)
				{
					patternStart = currentAddress;
					patternIndex++;
					foundFirstByte = true;

					//printf("[BMEM] Found first byte at: 0x%llx\n", patternStart);
				}
			}
			else
			{
				if (currentByte == pattern[patternIndex].byte) {
					//printf("[BMEM] Found next byte at: 0x%llx\n", currentAddress);
					patternIndex++;
				}
				else if (pattern[patternIndex].ignore)
				{
					//printf("[BMEM] Skipping byte at: 0x%llx\n", currentAddress);
					patternIndex++;
				}
				else
				{
					//printf("[BMEM] Next byte doesnt match pattern! restarting from next byte\n\n");

					i = (patternStart - moduleBase);
					patternStart = 0;
					patternIndex = 0;
					foundFirstByte = false;
				}

				if (patternIndex == pattern.size())
				{
					break;
				}
			}
		}

		if (patternStart != NULL)
		{
			printf("[BMEM] bmem::patternScan: Found At: 0x%llx\n", patternStart);
		}
		else
		{
			printf("[BMEM] bmem::patternScan: Pattern not found!\n");
		}

		return patternStart;
	}

	static bool is_address_valid(uint64_t address, const char* moduleToSet = "current")
	{
		if (!setModule(moduleToSet))
		{
			printf("[BMEM] bmem::is_address_valid: Failed to initialize a module\n");
			return false;
		}

		setModule(moduleToSet);

		if (!address) return false;
		if (address == NULL) return false;

		if (address > moduleBase + moduleSize) return false;
		if (address < moduleBase) return false;

		return true;
	}

	static uintptr_t relativeToAbsolute(uintptr_t address, int addressOffset, int instructionCount)
	{
		return (uintptr_t)(address + instructionCount + *reinterpret_cast<std::int32_t*>(address + addressOffset));
	}
}