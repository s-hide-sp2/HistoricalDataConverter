#include "StdAfx.h"
#include "hdcCmdConvertPeriod.h"
#include "hdcChart.h"


hdcCmdConvertPeriod::hdcCmdConvertPeriod(void)
	: m_nPeriod(0)
	, m_nOutputPeriod(1)
	, m_nShiftTime(0)
	, m_bSkipFirstRow(true)
	, m_dataFormat(Hdc::DataForamt::Normal)
{
}

hdcCmdConvertPeriod::~hdcCmdConvertPeriod(void)
{
}

/*
//	�R�}���h�����s����
Hdc::Result hdcCmdConvertPeriod::Execute()
{
	Hdc::Result result = Hdc::rOk;
	CFileFind ff;
	BOOL bFlg = TRUE;
	CString strFindFile;
	CWaitCursor wait;

	strFindFile.Format( _T("%s\\*.*"), GetFolder( DataFolder() ) );

	bFlg = ff.FindFile( strFindFile );
	while( bFlg && result == Hdc::rOk ){
		bFlg = ff.FindNextFile();

		if( !ff.IsDots() && !ff.IsDirectory() ){
			if( Period() == 0 )
				result = ConvertTimePeriodFromTick( ff.GetFilePath(), ff.GetFileName(), ff.GetFileTitle() );
		}
	}

	return result;
}
*/

//	�R�}���h�����s����
Hdc::Result hdcCmdConvertPeriod::Execute()
{
	Hdc::Result result = Hdc::rOk;

	result = FindFile();

	return result;
}

Hdc::Result hdcCmdConvertPeriod::Execute(const CString& strPath, const CString& strFileName, const CString& strTitle )
{
	Hdc::Result result = Hdc::rOk;

	if( Period() == 0 )
		result = ConvertTimePeriodFromTick( strPath, strFileName, strTitle );
	else
		result = ConvertTimePeriod( strPath, strFileName, strTitle );

	return result;
}

//	���Ԙg��ϊ�����
Hdc::Result hdcCmdConvertPeriod::ConvertTimePeriod(
	const CString& strPath,		//(i)�t�@�C���p�X
	const CString& strFileName,	//(i)�t�@�C����
	const CString& strSymbol	//(i)�ʉ݃y�A��
	) const
{
	Hdc::Result result = Hdc::rOk;
	hdcChart chart;
	CString strOutputPath, strSuffix;

	strSuffix.Format( _T("-%d"), OutputPeriod() );
	strOutputPath = GetOutputFilePath( strFileName, strSuffix );

	chart.SetSymbol( strSymbol );
	chart.SetShiftTime( ShiftTime() );
	chart.SetOutputPeriod( OutputPeriod() );
	result = chart.Generate( strPath, Period(), strOutputPath, OutputPeriod(), SkipFirstRow(), DataFormat() );

	return result;
}

//	�e�B�b�N�f�[�^����Ɏ��Ԙg��ϊ�����
Hdc::Result hdcCmdConvertPeriod::ConvertTimePeriodFromTick(
	const CString& strPath,		//(i)�t�@�C���p�X
	const CString& strFileName,	//(i)�t�@�C����
	const CString& strSymbol	//(i)�ʉ݃y�A��
	) const
{
	Hdc::Result result = Hdc::rOk;
	CString strPathBid = GetOutputFilePath( strFileName, _T("-Bid") );
	CString strPathAsk = GetOutputFilePath( strFileName, _T("-Ask") );
	hdcChart chart;

	chart.SetSymbol( strSymbol );
	chart.SetShiftTime( ShiftTime() );
	chart.SetOutputPeriod( OutputPeriod() );
	result = chart.GenerateFromTick( strPath, strPathBid, strPathAsk, OutputPeriod() );

	return result;
}


