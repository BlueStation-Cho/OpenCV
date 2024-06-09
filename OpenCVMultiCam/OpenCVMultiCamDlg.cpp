
// OpenCVMultiCamDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "OpenCVMultiCam.h"
#include "OpenCVMultiCamDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenCVMultiCamDlg 대화 상자



COpenCVMultiCamDlg::COpenCVMultiCamDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPENCVMULTICAM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	streamAddress_.emplace_back("rtsp://210.99.70.120:1935/live/cctv031.stream");
	streamAddress_.emplace_back("rtsp://210.99.70.120:1935/live/cctv032.stream");
	streamAddress_.emplace_back("rtsp://210.99.70.120:1935/live/cctv036.stream");
	streamAddress_.emplace_back("rtsp://210.99.70.120:1935/live/cctv045.stream");
	//streamAddress_.emplace_back("rtsp://210.99.70.120:1935/live/cctv026.stream");
}

void COpenCVMultiCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i{}; i < 12; ++i) {
		DDX_Control(pDX, IDC_CAM1 + i, m_pictures[i]);
	}
}

BEGIN_MESSAGE_MAP(COpenCVMultiCamDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// COpenCVMultiCamDlg 메시지 처리기

BOOL COpenCVMultiCamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	for (int i{}; i < 12; ++i) {
		m_pictures[i].ShowWindow(SW_HIDE);
	}

	int col = 2;

	size_t i{};
	for (i = 0; i < streamAddress_.size(); ++i) {
		m_pictures[i].ShowWindow(SW_SHOW);
		auto camView = std::make_shared<CamView2>();
		CRect rect = GetPictureRect(i, col);
		camView->Create(rect, this);
		camView->Init(i, streamAddress_[i], &m_pictures[i]);
		camViewVec_.push_back(camView);
	}

	EmptyCam(i, col);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

CRect COpenCVMultiCamDlg::GetPictureRect(const int i, const int col) const 
{
	if (col == 1) {
		return CRect(0, 0, 1366, 768);
	}
	else if (col == 2) {
		return GetPictureRect2x2(i);
	}
	return CRect();
}

CRect COpenCVMultiCamDlg::GetPictureRect2x2(const int i) const
{
	int width{ 864 };
	int height{ 486 };
	CRect rect{};
	if (i == 0) {
		rect = CRect(0, 0, width, height);
	}
	else if (i == 1) {
		rect = CRect(width, 0, width * 2, height);
	}
	else if (i == 2) {
		rect = CRect(0, height, width, height * 2);
	}
	else {
		rect = CRect(width, height, width * 2, height * 2);
	}

	return rect;
}

void COpenCVMultiCamDlg::EmptyCam(const int pos, const int col)
{
	m_pictures[pos].ShowWindow(SW_SHOW);

	int i = pos;
	int count = col * col;
	for (; i < count; ++i) {
		auto rect = GetPictureRect(i, col);
		m_pictures[i].SetWindowPos(0, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW | SWP_NOZORDER);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void COpenCVMultiCamDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR COpenCVMultiCamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

