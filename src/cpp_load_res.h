/*
* cpp_load_res.h ~ loads resources contained within the .exe file (internal use)
* author: Ben Staehle
* date: 6/20/23
*/

#pragma once
#pragma warning(disable : 4996)

#include<windows.h>
#include<cstdint>
#include<cstdlib>
#include<concepts>

#define WIN32_LEAN_AND_MEAN 

namespace utils
{
	template<typename T>
	concept integral = std::is_integral_v<T>;

	class Resource
	{
	public:
		struct params
		{
			std::size_t size;
			void* ptr = nullptr;
		};

	private:
		HRSRC hResource = nullptr;
		HGLOBAL hMemory = nullptr;

		params p;

	public:
		Resource(int res_id, int res_class) {
			hResource = FindResource(nullptr, MAKEINTRESOURCE(res_id), MAKEINTRESOURCE(res_class));
			hMemory = LoadResource(nullptr, hResource);

			p.size = SizeofResource(nullptr, hResource);
			p.ptr = LockResource(hMemory);
		}

		template<typename T>
			requires integral<T>
		auto getResourceAsPtr() {
			T* dst;
			dst = reinterpret_cast<T*>(p.ptr);
			return dst;
		}

		auto getResourceAsCharPtr() {
			uint8_t* dst;
			dst = reinterpret_cast<uint8_t*>(p.ptr);
			return dst;
		}

		auto getResourceAsVoidPtr() {
			return p.ptr;
		}

		std::size_t getResourceSize() {
			return p.size;
		}

	};
}