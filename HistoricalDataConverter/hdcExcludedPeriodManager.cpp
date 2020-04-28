#include "stdafx.h"
#include "hdcExcludedPeriodManager.h"


hdcExcludedPeriodManager::hdcExcludedPeriodManager()
{
	init();
}

hdcExcludedPeriodManager& hdcExcludedPeriodManager::instance()
{
	static hdcExcludedPeriodManager singleton;

	return singleton;
}

//	‘ÎÛŠúŠÔŠO”»’è
bool hdcExcludedPeriodManager::isIn(const hdcTime& time) const
{
	bool result = false;

	for (const auto& exPeriod : m_excludedPeriodContainer) {
		result = exPeriod.isIn(time);
		
		if (result) {
			break;
		}
	}

	return result;
}

//	‰Šú‰»
void hdcExcludedPeriodManager::init() 
{
	m_excludedPeriodContainer.clear();

	add(12, 20, 12, 31);
	add(1, 1, 1, 5);
}

//	‘ÎÛŠOŠúŠÔ‚ğ’Ç‰Á‚·‚é
void hdcExcludedPeriodManager::add(int beginMonth, int beginDay, int endMonth, int endDay)
{
	m_excludedPeriodContainer.push_back(
		hdcExcludedPeriod(
			beginMonth,
			beginDay,
			endMonth,
			endDay
		)
	);
}