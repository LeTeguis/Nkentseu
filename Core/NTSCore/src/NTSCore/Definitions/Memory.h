//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/14/2024 at 10:19:02 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_MEMORY_H__
#define __NKENTSEU_MEMORY_H__

#pragma once

#include "NTSCore/Platform/Exports.h"
#include "NTSCore/Nature/Base.h"

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

	class NKENTSEU_API AutoDestroyMemory {
	public:
		virtual bool Destroy() = 0;
	};

    class NKENTSEU_API Memory
    {
        public:
			template <typename T> using Shared = std::shared_ptr<T>;
			template <typename T> using Unique = std::unique_ptr<T>;
			template <typename T> using Raw = T*;
			
			#define null_pointer nullptr

			template <typename T, typename... Args>
			static Shared<T> AllocateShared(Args&&... args) {
				return std::make_shared<T>(std::forward<Args>(args)...);
			}

			template <typename T, typename... Args>
			static Unique<T> AllocateUnique(Args&&... args) {
				return std::make_unique<T>(std::forward<Args>(args)...);
			}

			template <typename T, typename... Args>
			static Raw<T> AllocateRaw(Args&&... args) {
				return new T(std::forward<Args>(args)...);
			}

			static bool FreeMemory(AutoDestroyMemory& memory) {
				return memory.Destroy();
			}

			static bool FreeMemory(std::vector<AutoDestroyMemory*>& memory) {
				bool success = true;
				for (auto adm : memory) {
					success = adm->Destroy() && success;
				}
				return success;
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
			static bool Reset(Raw<T> data) {
				if (data != nullptr) {
					delete data;
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
			static Raw<child> Cast(Raw<parent> data) {
				if (data != nullptr) {
					return dynamic_cast<child*>(data);
				}
				return nullptr;
			}

			template <typename child, typename parent>
			static Shared<child> Cast(Shared<parent>& data) {
				if (data != nullptr) {
					return std::dynamic_pointer_cast<child>(data);
				}
				return nullptr;
			}

			template <typename child, typename parent>
			static Unique<child> Cast(Unique<parent> data) {
				if (data != nullptr) {
					child* ptr = dynamic_cast<child*>(data.get());
					if (ptr != nullptr) {
						return Unique<child>(ptr);
					}
				}
				return nullptr;
			}

        private:
    };

} // namespace nkentseu

#endif    // __NKENTSEU_MEMORY_H__