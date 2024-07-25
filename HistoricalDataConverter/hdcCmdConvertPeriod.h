#pragma once
#include "hdccommand.h"
class hdcCmdConvertPeriod :
	public hdcCommand
{
public:
	hdcCmdConvertPeriod(void);
	virtual ~hdcCmdConvertPeriod(void);

	//////////////////////////////////////////////////
	//
	//	Get/Set

	void SetPeriod( int value );
	int Period() const;

	void SetOutputPeriod( int value );
	int OutputPeriod() const;

	void SetShiftTime( int value );
	int ShiftTime() const;

	//	�擪�s�ǂݔ�΂��t���O
	bool SkipFirstRow() const;
	void SetSkipFirstRow(bool value);

	//	�f�[�^�t�H�[�}�b�g
	Hdc::DataForamt DataFormat() const;
	void SetDataFormat(Hdc::DataForamt value);

protected:

	//	�R�}���h�����s����
	virtual Hdc::Result Execute();

	//	�R�}���h�����s����
	virtual Hdc::Result Execute(const CString& strPath, const CString& strFileName, const CString& strTitle );

private:

	//	���Ԙg��ϊ�����
	Hdc::Result ConvertTimePeriod( const CString& strPath, const CString& strFileName, const CString& strSymbol ) const;

	//	�e�B�b�N�f�[�^����Ɏ��Ԙg��ϊ�����
	Hdc::Result ConvertTimePeriodFromTick( const CString& strPath, const CString& strFileName, const CString& strSymbol ) const;

private:

	//	���Ԙg
	int m_nPeriod;

	//	�o�͎��Ԙg
	int m_nOutputPeriod;

	//	�V�t�g����
	int m_nShiftTime;

	//	�擪�s�ǂݔ�΂��t���O
	bool m_bSkipFirstRow;

	//	�f�[�^�t�H�[�}�b�g
	Hdc::DataForamt m_dataFormat;
};

inline void hdcCmdConvertPeriod::SetPeriod( int value )
{
	m_nPeriod = value;
}

inline int hdcCmdConvertPeriod::Period() const
{
	return m_nPeriod;
}

inline void hdcCmdConvertPeriod::SetOutputPeriod( int value )
{
	m_nOutputPeriod = value;
}

inline int hdcCmdConvertPeriod::OutputPeriod() const
{
	return m_nOutputPeriod;
}

inline void hdcCmdConvertPeriod::SetShiftTime( int value )
{
	m_nShiftTime = value;
}

inline int hdcCmdConvertPeriod::ShiftTime() const
{
	return m_nShiftTime;
}

inline bool hdcCmdConvertPeriod::SkipFirstRow() const
{
	return m_bSkipFirstRow;
}

inline void hdcCmdConvertPeriod::SetSkipFirstRow(bool value)
{
	m_bSkipFirstRow = value;
}

//	�f�[�^�t�H�[�}�b�g
inline Hdc::DataForamt hdcCmdConvertPeriod::DataFormat() const
{
	return m_dataFormat;
}

inline void hdcCmdConvertPeriod::SetDataFormat(Hdc::DataForamt value)
{
	m_dataFormat = value;
}
