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
	#define ValidateObject(o, so, ret, vret, methode, ... )		if (o == nullptr || o->so == nullptr) { \
																	return vret; \
																} \
																if (ret) { \
																	return o->so->methode(__VA_ARGS__); \
																} else { \
																	o->so->methode(__VA_ARGS__); \
																}
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

        private:
    };
} // namespace nkentseu

#endif    // __NKENTSEU_MEMORY_H__