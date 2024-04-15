//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 11:29:15 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_LOG_H__
#define __NKENTSEU_LOG_H__

#pragma once

#include "System/System.h"
#include <memory>
#include <Logger/Logger.h>
#include "System/Epoch/Date.h"
#include "System/Epoch/Time.h"

namespace nkentseu {
	class NKENTSEU_API MLog {
	public:
		static MLog& Instance();

		std::shared_ptr<Logger> Get() {
			return m_Logger;
		}

		~MLog() = default;
	private:
		std::shared_ptr<Logger> m_Logger;

		MLog();
	};

	#define Log_base			MLog::Instance().Get()->Details(__FILE__, __LINE__, __FUNCTION__, nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent())
} // nkentseu


#endif // __NKENTSEU_LOG_H__