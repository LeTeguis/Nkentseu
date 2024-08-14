//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 11:29:15 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_LOG_H__
#define __NKENTSEU_LOG_H__

#pragma once

#include "NTSCore/System.h"
#include <memory>
#include <NTSLogger/Logger.h>
#include "NTSCore/Epoch/Date.h"
#include "NTSCore/Epoch/Time.h"

namespace nkentseu {
	class NKENTSEU_API LoggerBase {
	public:
		static LoggerBase& Instance();

		std::shared_ptr<Logger> Get() {
			return m_Logger;
		}

		~LoggerBase() = default;
	private:
		std::shared_ptr<Logger> m_Logger;

		LoggerBase();
	};

	#define SystemNameLogger "Log"
	#define LogBase(name)	 LoggerBase::Instance().Get()->SetName(name).Details(__FILE__, __LINE__, __FUNCTION__, nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent())
	#define LogBaseReset(name, file, line, function)	 LoggerBase::Instance().Get()->SetName(name).Details(file, line, function, nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent())

	#define sLog			 LogBase(SystemNameLogger)
} // nkentseu


#endif // __NKENTSEU_LOG_H__