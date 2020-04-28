#pragma once

#include "hdcTypeDef.h"

//	対象期間外クラス
class hdcExcludedPeriod
{
public:
	hdcExcludedPeriod(int beginMonth, int beginDay, int endMonth, int endDay);
	~hdcExcludedPeriod() = default;

	//	対象期間外判定
	bool isIn(
		const hdcTime& time	//(i)判定日
	) const;

private:
	//	対象期間開始月
	int m_beginMonth;
	
	//	対象期間開始日
	int m_beginDay;

	//	対象期間終了月
	int m_endMonth;

	//	対象期間終了日
	int m_endDay;
};

