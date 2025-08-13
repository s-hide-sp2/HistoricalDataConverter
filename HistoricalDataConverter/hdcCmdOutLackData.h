#pragma once
#include "hdcCmdFillLackData.h"
class hdcCmdOutLackData :
	public hdcCmdFillLackData
{
public:
	hdcCmdOutLackData();
	virtual ~hdcCmdOutLackData() = default;

	//	データフォーマット
	Hdc::DataForamt DataFormat() const {
		return m_dataFormat;
	}

	void SetDataFormat(Hdc::DataForamt value) {
		m_dataFormat = value;
	}

	void SetShiftTime(int value){
		m_shiftTime = value;
	}

	int ShiftTime() const{
		return m_shiftTime;
	}

protected:

	virtual Hdc::Result Execute(const CString& strPath, const CString& strFileName, const CString& strTitle) override;

private:
	//	データフォーマット
	Hdc::DataForamt m_dataFormat;

	//	シフト時間
	int m_shiftTime;
};
