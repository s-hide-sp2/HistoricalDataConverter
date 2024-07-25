
// HistoricalDataConverterDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "atlcomtime.h"


// CHistoricalDataConverterDlg �_�C�A���O
class CHistoricalDataConverterDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CHistoricalDataConverterDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_HISTORICALDATACONVERTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCnvPeriod();
private:
	// �f�[�^�t�H���_
	CString m_strDataFolder;
	// �o�̓t�H���_
	CString m_strOutputFolder;
	// ���Ԙg
	int m_nPeriod;
	// �V�t�g����
	int m_nShiftTime;
	// �o�͎��Ԙg
	int m_nOutputPeriod;
	// �J�n����
	COleDateTime m_timeBegin;
	// �I������
	COleDateTime m_timeEnd;
public:
	afx_msg void OnBnClickedBtnExtract();
	// ���t����
//	int m_nTimeFormat;
private:
	// ���t����
	int m_nTimeFormat;
public:
	afx_msg void OnBnClickedBtnFillLackData();
private:
	BOOL m_bSkipFirstLow;
public:
	afx_msg void OnBnClickedBtnOutLackData();
	CComboBox m_cmbDataFormat;
};
