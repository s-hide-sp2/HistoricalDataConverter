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
};
