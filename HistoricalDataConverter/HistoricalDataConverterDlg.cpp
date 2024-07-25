
// HistoricalDataConverterDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HistoricalDataConverter.h"
#include "HistoricalDataConverterDlg.h"
#include "afxdialogex.h"
#include "hdcCmdConvertPeriod.h"
#include "hdcCmdExtract.h"
#include "hdcCmdFillLackData.h"
#include "hdcCmdOutLackData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHistoricalDataConverterDlg ダイアログ




CHistoricalDataConverterDlg::CHistoricalDataConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistoricalDataConverterDlg::IDD, pParent)
	, m_strDataFolder(_T("C:\\data\\HistoricalData\\Debug"))
	, m_strOutputFolder(_T("C:\\data\\HistoricalData\\Debug\\Out"))
	, m_nPeriod(1)
	//, m_nShiftTime(420)
	, m_nShiftTime(0)
	, m_nOutputPeriod(1)
	, m_timeBegin(COleDateTime::GetCurrentTime())
	, m_timeEnd(COleDateTime::GetCurrentTime())
	, m_nTimeFormat(0)
	, m_bSkipFirstLow(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHistoricalDataConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_DATA_FOLDER, m_strDataFolder);
	DDV_MaxChars(pDX, m_strDataFolder, 255);
	DDX_Text(pDX, IDC_EDT_OUTPUT_FOLDER, m_strOutputFolder);
	DDV_MaxChars(pDX, m_strOutputFolder, 255);
	DDX_Text(pDX, IDC_EDT_PERIOD, m_nPeriod);
	DDV_MinMaxInt(pDX, m_nPeriod, 0, 1440);
	DDX_Text(pDX, IDC_EDT_SHIFT_TIME, m_nShiftTime);
	DDV_MinMaxInt(pDX, m_nShiftTime, 0, 1440);
	DDX_Text(pDX, IDC_EDT_OUTPUT_PERIOD, m_nOutputPeriod);
	DDV_MinMaxInt(pDX, m_nOutputPeriod, 1, 1440);
	DDX_DateTimeCtrl(pDX, IDC_DTP_BEGIN, m_timeBegin);
	DDX_DateTimeCtrl(pDX, IDC_DTP_END, m_timeEnd);
	//  DDX_Text(pDX, IDC_CMB_TIME_FORMAT, m_nTimeFormat);
	//DDX_CBString(pDX, IDC_CMB_TIME_FORMAT, m_nTimeFormat);
	DDX_CBIndex(pDX, IDC_CMB_TIME_FORMAT, m_nTimeFormat);
	DDX_Check(pDX, IDC_CHK_SKIP_FIRST_LOW, m_bSkipFirstLow);
	DDX_Control(pDX, IDC_CMB_DATA_FORMAT, m_cmbDataFormat);
}

BEGIN_MESSAGE_MAP(CHistoricalDataConverterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CNV_PERIOD, &CHistoricalDataConverterDlg::OnBnClickedBtnCnvPeriod)
	ON_BN_CLICKED(IDC_BTN_EXTRACT, &CHistoricalDataConverterDlg::OnBnClickedBtnExtract)
	ON_BN_CLICKED(IDC_BTN_FILL_LACK_DATA, &CHistoricalDataConverterDlg::OnBnClickedBtnFillLackData)
	ON_BN_CLICKED(IDC_BTN_OUT_LACK_DATA, &CHistoricalDataConverterDlg::OnBnClickedBtnOutLackData)
END_MESSAGE_MAP()


// CHistoricalDataConverterDlg メッセージ ハンドラー

BOOL CHistoricalDataConverterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	m_cmbDataFormat.SetCurSel(0);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CHistoricalDataConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CHistoricalDataConverterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CHistoricalDataConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHistoricalDataConverterDlg::OnBnClickedBtnCnvPeriod()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if( UpdateData(TRUE) ){
		hdcCmdConvertPeriod cmd;
		Hdc::DataForamt dataFormat = static_cast<Hdc::DataForamt>(m_cmbDataFormat.GetCurSel());

		cmd.SetDataFolder( m_strDataFolder );
		cmd.SetOutputFolder( m_strOutputFolder );
		cmd.SetPeriod( m_nPeriod );
		cmd.SetOutputPeriod( m_nOutputPeriod );
		cmd.SetShiftTime( m_nShiftTime );
		cmd.SetSkipFirstRow(m_bSkipFirstLow);
		cmd.SetDataFormat(dataFormat);

		hdcCommand::Execute( cmd );
	}
}



void CHistoricalDataConverterDlg::OnBnClickedBtnExtract()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	if( UpdateData(TRUE) ){
		hdcCmdExtract cmd;

		m_timeBegin.SetDate( m_timeBegin.GetYear(), m_timeBegin.GetMonth(), m_timeBegin.GetDay() );
		m_timeEnd.SetDateTime( m_timeEnd.GetYear(), m_timeEnd.GetMonth(), m_timeEnd.GetDay(), 23, 59, 59 );

		cmd.SetDataFolder( m_strDataFolder );
		cmd.SetOutputFolder( m_strOutputFolder );
		cmd.SetTimeBegin( m_timeBegin );
		cmd.SetTimeEnd( m_timeEnd );
		cmd.SetTimeFormat( static_cast<Hdc::TimeFormat>(m_nTimeFormat) );

		hdcCommand::Execute( cmd );
	}
	
}


void CHistoricalDataConverterDlg::OnBnClickedBtnFillLackData()
{
	if( UpdateData(TRUE) ){
		hdcCmdFillLackData cmd;

		cmd.SetDataFolder( m_strDataFolder );
		cmd.SetOutputFolder( m_strOutputFolder );
		cmd.SetPeriod( m_nPeriod );
		cmd.SetSkipFirstRow( m_bSkipFirstLow );

		hdcCommand::Execute( cmd );
	}

}


void CHistoricalDataConverterDlg::OnBnClickedBtnOutLackData()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (UpdateData(TRUE)) {
		hdcCmdOutLackData cmd;

		cmd.SetDataFolder(m_strDataFolder);
		cmd.SetOutputFolder(m_strOutputFolder);
		cmd.SetPeriod(m_nPeriod);
		cmd.SetSkipFirstRow(m_bSkipFirstLow);

		hdcCommand::Execute(cmd);
	}
}
