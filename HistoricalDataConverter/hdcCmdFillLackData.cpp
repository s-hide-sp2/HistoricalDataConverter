#include "StdAfx.h"
#include "hdcCmdFillLackData.h"
#include "hdcChart.h"

hdcCmdFillLackData::hdcCmdFillLackData(void)
	: m_nPeriod(HDC_PERIOD_M1)
	, m_bSkipFirstRow(true)
{
}

hdcCmdFillLackData::~hdcCmdFillLackData(void)
{
}

//	コマンドを実行する
Hdc::Result hdcCmdFillLackData::Execute()
{
	Hdc::Result result = Hdc::rOk;

	result = FindFile();

	return result;
}

//	コマンドを実行する
Hdc::Result hdcCmdFillLackData::Execute(const CString& strPath, const CString& strFileName, const CString& strTitle )
{
	Hdc::Result result = Hdc::rOk;
	hdcChart chart;
	CString strOutputPath, strSuffix;

	chart.SetSymbol( strTitle.Left(6) );
	strOutputPath = GetOutputFilePath( strFileName, strSuffix );
	result = chart.FillLackData( strPath, Period(), strOutputPath, SkipFirstRow() );

	return result;
}
