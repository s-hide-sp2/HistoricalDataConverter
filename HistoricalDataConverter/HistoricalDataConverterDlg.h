
// HistoricalDataConverterDlg.h : ヘッダー ファイル
//

#pragma once
#include "atlcomtime.h"


// CHistoricalDataConverterDlg ダイアログ
class CHistoricalDataConverterDlg : public CDialogEx
{
// コンストラクション
public:
	CHistoricalDataConverterDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_HISTORICALDATACONVERTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCnvPeriod();
private:
	// データフォルダ
	CString m_strDataFolder;
	// 出力フォルダ
	CString m_strOutputFolder;
	// 時間枠
	int m_nPeriod;
	// シフト時間
	int m_nShiftTime;
	// 出力時間枠
	int m_nOutputPeriod;
	// 開始期間
	COleDateTime m_timeBegin;
	// 終了期間
	COleDateTime m_timeEnd;
public:
	afx_msg void OnBnClickedBtnExtract();
	// 日付書式
//	int m_nTimeFormat;
private:
	// 日付書式
	int m_nTimeFormat;
public:
	afx_msg void OnBnClickedBtnFillLackData();
private:
	BOOL m_bSkipFirstLow;
public:
	afx_msg void OnBnClickedBtnOutLackData();
	CComboBox m_cmbDataFormat;
};
