#include "stdafx.h"
#include "hdcCmdOutLackData.h"
#include "hdcChart.h"

hdcCmdOutLackData::hdcCmdOutLackData()
{
}

//	コマンドを実行する
Hdc::Result hdcCmdOutLackData::Execute(const CString& strPath, const CString& strFileName, const CString& strTitle)
{
	Hdc::Result result = Hdc::rOk;
	hdcChart chart;
	CString strOutputPath, strSuffix;

	chart.SetSymbol(strTitle.Left(6));
	strOutputPath = GetOutputFilePath(strFileName, strSuffix);
	result = chart.OutputLackData(strPath, Period(), strOutputPath, SkipFirstRow());

	return result;
}
