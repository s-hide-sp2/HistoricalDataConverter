#pragma once

#include "hdcBar.h"

struct hdcTerm{
	hdcTime m_timeBegin;
	hdcTime m_timeEnd;

	hdcTerm(const hdcTime& begin, const hdcTime& end) : m_timeBegin(begin), m_timeEnd(end){}

	bool IsIn( const hdcTime& time ) const
	{
		return ( m_timeBegin <= time && time <= m_timeEnd );
	}
};

class hdcChart
{
public:
	hdcChart(void);
	virtual ~hdcChart(void);

	//	�`���[�g�f�[�^�𐶐�����
	Hdc::Result Generate(LPCTSTR lpszSrcPath, int nSrcPeriod, LPCTSTR lpszOutPath, int nOutputPeriod, bool bSkipFirstRow, Hdc::DataForamt dataFormat);

	//	�e�B�b�N�f�[�^�����Ƀ`���[�g�f�[�^�𐶐�����
	Hdc::Result GenerateFromTick(LPCTSTR lpszSrcPath, LPCTSTR lpszOutPathBid, LPCTSTR lpszOutPathAsk, int nOutputPeriod);

	//	���o����
	Hdc::Result Extract( LPCTSTR lpszSrcPath, LPCTSTR lpszOutPath, const hdcTime& timeBegin, const hdcTime& timeEnd, Hdc::TimeFormat timeFormat );

	//	�����f�[�^�𖄂߂�
	Hdc::Result FillLackData( LPCTSTR lpszSrcPath, int nSrcPeriod, LPCTSTR lpszOutPath, bool bSkipFirstRow );

	//	�����f�[�^���o�͂���
	Hdc::Result OutputLackData(LPCTSTR lpszSrcPath, int nSrcPeriod, LPCTSTR lpszOutPath, bool bSkipFirstRow);

	///////////////////////////////////////////////////
	//
	//	Get/Set

	void SetSymbol( LPCTSTR value );
	LPCTSTR Symbol() const;

	void SetPeriod(int value);
	int Period() const;

	void SetOutputPeriod(int value);
	int OutputPeriod() const;

	void SetShiftTime(int nTotalMinutes);
	const hdcTimeSpan& ShiftTime() const;

protected:

	//	���o�[��������
	Hdc::Result IsNextBarTime( bool& bNext, const hdcTime& time ) const;

	//	��Ɨp�o�[�Ƀf�[�^��ǉ�����
	Hdc::Result AddBarTemp( bool bNextBarTime, const hdcTime& time, double dBid, double dAsk);

	//	��Ɨp�o�[�Ƀf�[�^��ǉ�����
	Hdc::Result AddBarTemp( bool bNextBarTime, const hdcTime& time, const double dRates[], int volume );

	//	�o�[��ǉ�����
	void AddBar( const hdcBar& bar );

	//	�o�[�f�[�^�o��
	Hdc::Result Write( CStdioFile& cf, const hdcBar& bar, Hdc::BarKind barKind );

	//	�����f�[�^�𖄂߂�
	//Hdc::Result FillLackData( hdcTime& timeNext, const hdcTime& timeCur, CStdioFile& cfBid, CStdioFile& cfAsk );

	//	�����f�[�^���o�͂���
	void GetLackData( mapBar& bars, const hdcBar& barAfter, const hdcBar& barBefore );

	//	�v���C�X�̏���������Ԃ�
	int Decimal() const;

	//	���Ԙg�̎��ԊԊu��Ԃ�
	hdcTimeSpan GetPeriodSpan(int nPeriod) const;

	//	�o�[�I�[�v��������Ԃ�
	hdcTime GetOpenTime(const hdcTime& time, int nPeriod) const;

	//	���o�[�̃I�[�v��������Ԃ�
	bool GetNextOpenTime(hdcTime& timeNextOpen) const;

	//	���O�o�͊��ԏ�����
	void InitOutputLogTerm();

	//	���O�o�͔���
	bool IsOutputLog( const hdcTime& time ) const;

	//	���O�o�̓t�@�C���p�X��Ԃ�
	CString OutputLogFilePath() const;

private:
	//	�ʉ݃y�A
	CString m_strSymbol;

	//	���Ԙg
	int m_nPeriod;

	//	�`���[�g�f�[�^�������Ԙg
	int m_nOutputPeriod;

	//	�V�t�g����
	hdcTimeSpan m_spanShift;

	//	�v���C�X�̏�������
	int m_nDecimal;

	//	�o�[�}�b�v
	mapBar m_mapBar;

	//	�`���[�g�f�[�^�����p��ƃo�[
	hdcBar m_barTemp;

	//	���O�o�͊���
	lstTerm m_termOutputLog;
};

inline LPCTSTR hdcChart::Symbol() const
{
	return m_strSymbol;
}

inline void hdcChart::SetPeriod(int value)
{
	m_nPeriod = value;
}

inline int hdcChart::Period() const
{
	return m_nPeriod;
}

inline void hdcChart::SetOutputPeriod(int value)
{
	m_nOutputPeriod = value;
}

inline int hdcChart::OutputPeriod() const
{
	return m_nOutputPeriod;
}

//	�o�[��ǉ�����
inline void hdcChart::AddBar( const hdcBar& bar )
{
	m_mapBar[bar.Time()] = bar;
}

//	�v���C�X�̏���������Ԃ�
inline int hdcChart::Decimal() const
{
	return m_nDecimal;
}

inline const hdcTimeSpan& hdcChart::ShiftTime() const
{
	return m_spanShift;
}
