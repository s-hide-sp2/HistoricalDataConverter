#pragma once
#include "hdcCmdFillLackData.h"
class hdcCmdOutLackData :
	public hdcCmdFillLackData
{
public:
	hdcCmdOutLackData();
	virtual ~hdcCmdOutLackData() = default;

protected:

	virtual Hdc::Result Execute(const CString& strPath, const CString& strFileName, const CString& strTitle) override;
};

