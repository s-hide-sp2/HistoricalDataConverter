#include "stdafx.h"
#include "hdcExcludedPeriod.h"

hdcExcludedPeriod::hdcExcludedPeriod(int beginMonth, int beginDay, int endMonth, int endDay)
	: m_beginMonth(beginMonth)
	, m_beginDay(beginDay)
	, m_endMonth(endMonth)
	, m_endDay(endDay)
{
}

//	‘ÎÛŠúŠÔŠO”»’è
bool hdcExcludedPeriod::isIn(
	const hdcTime& time	//(i)”»’è“ú
	) const
{
	hdcTime begin(time.GetYear(), m_beginMonth, m_beginDay, 0, 0, 0);
	hdcTime end(time.GetYear(), m_endMonth, m_endDay, 0, 0, 0);

	return (begin <= time && time <= end);
}
