#include "StdAfx.h"
#include "hdcChart.h"
#include "hdcCmdExtract.h"


hdcCmdExtract::hdcCmdExtract(void)
	: m_timeFormat(Hdc::DDMMYY_HHMMSS)
{
}


hdcCmdExtract::~hdcCmdExtract(void)
{
}

//	コマンドを実行する
Hdc::Result hdcCmdExtract::Execute()
{
	Hdc::Result result = Hdc::rOk;

	result = FindFile();

	return result;
}

Hdc::Result hdcCmdExtract::Execute(
	const CString& strPath,		//(i)ファイルパス
	const CString& strFileName, //(i)ファイル名
	const CString& strTitle		//(i)ファイルタイトル
	)
{
	Hdc::Result result = Hdc::rOk;
	hdcChart chart;
	CString strOutputPath = GetOutputFilePath( strFileName, GetTermString() );

	result = chart.Extract( strPath, strOutputPath, TimeBegin(), TimeEnd(), TimeFormat() );

	return result;
}

//	期間文字を返す
CString hdcCmdExtract::GetTermString() const
{
	CString str = _T("_");
	
	str += TimeBegin().Format( HDC_FMT_YYYYMMDD_2 );
	str += _T("_");
	str += TimeEnd().Format( HDC_FMT_YYYYMMDD_2 );

	return str;
}
