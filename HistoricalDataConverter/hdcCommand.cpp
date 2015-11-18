#include "StdAfx.h"
#include "hdcUtility.h"
#include "hdcCommand.h"


hdcCommand::hdcCommand(void)
{
}


hdcCommand::~hdcCommand(void)
{
}

//	コマンドを実行する
Hdc::Result hdcCommand::Execute( 
	hdcCommand& command	//(i)コマンド 
	)
{
	Hdc::Result result = Hdc::rOk;

	result = command.Execute();

	if( result != Hdc::rOk && result != Hdc::rCancel ){
		CString strErrorMsg = hdcUtility::GetErrorMsg( result );

		AfxMessageBox( strErrorMsg );
	}
	
	return result;
}

//	コマンドを実行する
Hdc::Result hdcCommand::FindFile()
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
			result = Execute( ff.GetFilePath(), ff.GetFileName(), ff.GetFileTitle() );
		}
	}

	return result;
}

//	フォルダを返す
CString hdcCommand::GetFolder(
	const CString& str	//(i)フォルダ名
	) const
{
	CString strFolder;

	if( str.Right(1) == _T("\\") )
		strFolder = str.Left( str.GetLength()-1 );
	else
		strFolder = str;

	return strFolder;
}

//	出力ファイルパスを返す
CString hdcCommand::GetOutputFilePath( 
	const CString& strFileName, 
	LPCTSTR lpszSuffix 
	) const
{
	CString strFilePath;
	CString strOutputFolder;
	int nLoc = strFileName.Find( _T('.') );

	strFilePath.Format( _T("%s\\%s%s.%s"), GetFolder(OutputFolder()), strFileName.Left(nLoc), lpszSuffix, strFileName.Mid(nLoc+1) );

	return strFilePath;
}
