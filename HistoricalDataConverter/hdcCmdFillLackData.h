#pragma once
#include "hdccommand.h"
class hdcCmdFillLackData :
	public hdcCommand
{
public:
	hdcCmdFillLackData(void);
	virtual ~hdcCmdFillLackData(void);

	void SetPeriod( int value );
	int Period() const;

	//	先頭行読み飛ばしフラグ
	bool SkipFirstRow() const;
	void SetSkipFirstRow(bool value);

protected:

	//	コマンドを実行する
	virtual Hdc::Result Execute();

	//	コマンドを実行する
	virtual Hdc::Result Execute(const CString& strPath, const CString& strFileName, const CString& strTitle );

private:
	//	時間枠
	int m_nPeriod;

	//	先頭行読み飛ばしフラグ
	bool m_bSkipFirstRow;
};

inline void hdcCmdFillLackData::SetPeriod( int value )
{
	m_nPeriod = value;
}

inline int hdcCmdFillLackData::Period() const
{
	return m_nPeriod;
}

//	先頭行読み飛ばしフラグ
inline bool hdcCmdFillLackData::SkipFirstRow() const
{
	return m_bSkipFirstRow;
}

inline void hdcCmdFillLackData::SetSkipFirstRow(bool value)
{
	m_bSkipFirstRow = value;
}

