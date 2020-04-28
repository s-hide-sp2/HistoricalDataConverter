#pragma once

#include "hdcExcludedPeriod.h"

class hdcExcludedPeriodManager
{
public:
	static hdcExcludedPeriodManager& instance();

	//	対象期間外判定
	bool isIn(
		const hdcTime& time	//(i)判定日
	) const;

private:
	hdcExcludedPeriodManager();
	~hdcExcludedPeriodManager() = default;

	//	初期化
	void init();

	//	対象外期間を追加する
	void add(int beginMonth, int beginDay, int endMonth, int endDay);

private:
	//	対象期間外コンテナ
	using ExcludedPeriodContainer = std::list<hdcExcludedPeriod>;	

private:

	//	対象期間外コンテナ
	ExcludedPeriodContainer m_excludedPeriodContainer;
};

