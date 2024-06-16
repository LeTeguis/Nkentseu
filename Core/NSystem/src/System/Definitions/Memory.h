//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/14/2024 at 10:19:02 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_MEMORY_H__
#define __NKENTSEU_MEMORY_H__

#pragma once

#include "System/System.h"
#include <vector>
#include <string>
#include <memory>

namespace nkentseu {
	#define ValidateObject(o, ret, vret, methode, ... )		if (o == nullptr) { \
																return vret; \
															} \
															if (ret) { \
																return o->methode(__VA_ARGS__); \
															} else { \
																o->methode(__VA_ARGS__); \
															}

	/*class NKENTSEU_API MemoryAllocator {
		template<typename T, typename... Args>
		void* Alloc(Args&&... args) = 0;

		template<typename T, typename... Args>
		void* AllocUnique(Args&&... args) = 0;

		template<typename T, typename... Args>
		void* Reset(Args&&... args) = 0;

		template<typename T, typename... Args>
		void* ResetUnique(Args&&... args) = 0;

		template<typename T, typename... Args>
		void* Alloc(Args&&... args) = 0;
	};*/

    class NKENTSEU_API Memory
    {
        public:
			template <typename T> using Shared = std::shared_ptr<T>;
			template <typename T> using Unique = std::unique_ptr<T>;
			
			#define null_pointer nullptr

			template <typename T, typename... Args>
			static Shared<T> Alloc(Args&&... args) {
				return std::make_shared<T>(args...);
			}

			template <typename T, typename... Args>
			static Unique<T> AllocUnique(Args&&... args) {
				return std::make_unique<T>(args...);
			}

			template <typename T>
			static bool Reset(Shared<T> data) {
				if (data != nullptr) {
					data.reset();
					data = nullptr;
					return true;
				}
				return false;
			}

			template <typename T>
			static bool Reset(Unique<T> data) {
				if (data != nullptr) {
					data.reset();
					data = nullptr;
					return true;
				}
				return false;
			}

			template <typename child, typename parent>
			static Shared<child> SharedCast(Shared<parent> data) {
				if (data != nullptr) {
					return std::dynamic_pointer_cast<child>(data);
				}
				return nullptr;
			}

			template <typename child, typename parent>
			static Unique<child> UniqueCast(Unique<parent> data) {
				if (data != nullptr) {
					if (dynamic_cast<child*>(data.get()) != nullptr) {
						static_cast<std::unique_ptr<child>>(std::move(data));
					}
				}
				return nullptr;
			}

        private:
    };
} // namespace nkentseu

#endif    // __NKENTSEU_MEMORY_H__