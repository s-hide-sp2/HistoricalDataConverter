#pragma once

#define HDC_FMT_YYYYMMDD_HHMMSS	_T("%Y/%m/%d %H:%M:%S")
#define HDC_FMT_YYYYMMDD_1		_T("%Y/%m/%d")
#define HDC_FMT_YYYYMMDD_2		_T("%Y-%m-%d")
#define HDC_FMT_YYYYMMDD_3		_T("%Y.%m.%d")

#define hdcTime		COleDateTime
#define hdcTimeSpan	COleDateTimeSpan

class hdcBar;
struct hdcTerm;

typedef std::list<hdcTerm> lstTerm;
typedef std::vector<CString> vecString;
typedef std::map<hdcTime, hdcBar> mapBar;

#define HDC_PERIOD_TICK	(0)
#define HDC_PERIOD_M1	(1)
#define HDC_PERIOD_M5	(5)
#define HDC_PERIOD_M15	(15)
#define HDC_PERIOD_M30	(30)
#define HDC_PERIOD_H1	(60)
#define HDC_PERIOD_H4	(240)
#define HDC_PERIOD_D1	(1440)
#define HDC_PERIOD_W1	(10080)
#define HDC_PERIOD_MN1	(43200)

namespace Hdc{
	enum Result{
		rOk,
		rCancel,
		rFail,
		rFileOpenError,
		rInvalidDateTime
	};

	enum BarKind{
		Bid = 0,
		Ask = 4
	};

	//	ÉåÅ[ÉgéÌï 
	enum RateKind{
		Open,
		High,
		Low,
		Close
	};

	enum Day{
		Sunday=1,
		Monday,
		Tuesday,
		Wednsday,
		Thursday,
		Friday,
		Saturday
	};

	enum TimeFormat{
		DDMMYY_HHMMSS,
		YYYYMMDD_HHMMSS
	};

	enum DataForamt {
		Normal,
		ForexTester
	};
};
