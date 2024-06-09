// CamView2.cpp: 구현 파일
//

#include "pch.h"
#include "OpenCVMultiCam.h"
#include "CamView2.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <iostream>
#include <chrono>

// CamView2

IMPLEMENT_DYNAMIC(CamView2, CWnd)

CamView2::CamView2()
{

}

CamView2::~CamView2()
{
	thread_->Destroy();
}


BEGIN_MESSAGE_MAP(CamView2, CWnd)
END_MESSAGE_MAP()



// CamView2 메시지 처리기
BOOL CamView2::Create(const int x, const int y, const int width, const int height, CWnd* parentWnd)
{
	viewRect_ = CRect(x, y, x + width, y + height);
	return CWnd::Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE, viewRect_, parentWnd, 0);
}

BOOL CamView2::Create(const CRect& rect, CWnd* parentWnd)
{
	viewRect_ = rect;
	return CWnd::Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE, viewRect_, parentWnd, 0);
}

void CamView2::Init(const int i, const cv::String& address, CStatic* picture)
{
	capture = new cv::VideoCapture(address);
	if (!capture->isOpened()) {
		MessageBox(_T("웹캠을 열수 없습니다."));
	}

	m_picture = picture;
	m_picture->SetWindowPos(0, viewRect_.left, viewRect_.top, viewRect_.Width(), viewRect_.Height(), SWP_SHOWWINDOW | SWP_NOZORDER);

	CreateThread();
}

void CamView2::CreateThread()
{
	const double FPS = 24.0;

	thread_ = std::make_unique<MyThread>(_T("DeleteLog"));
	if (thread_->Thread == nullptr) {
		thread_->Run = true;
		thread_->FutureObject = thread_->ExitSignal.get_future();

		thread_->Thread = std::make_unique<std::thread>(std::thread([this, FPS]() {
			std::chrono::system_clock::time_point prevTime = std::chrono::system_clock::now();
			while (thread_->Run) {
				//if (thread_->FutureObject.wait_for(std::chrono::duration<double>(0.03)) == std::future_status::timeout) {
					auto ret = capture->read(mat_frame);
					auto currentTime = std::chrono::system_clock::now();
					std::chrono::duration<double> sec = currentTime - prevTime;
					if ((ret == true) && (sec.count() > (1.0 / FPS))) {
						prevTime = currentTime;
						DrawMat();
					}
				//}
			}
		}));
	}
}


void CamView2::DrawMat()
{
	//std::scoped_lock<std::mutex> addLock(mutex_);


	/*if (mat_frame.empty()) {
		return;
	}*/

	CDC* pDC;
	CRect rect;

	pDC = m_picture->GetDC();  // picture control 변수
	m_picture->GetClientRect(&rect);

	int bpp = 8 * mat_frame.elemSize();

	int w = mat_frame.cols;
	int h = mat_frame.rows;

	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biYPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biClrImportant = 0;
	bitmapInfo.bmiHeader.biSizeImage = 0;
	bitmapInfo.bmiHeader.biWidth = w;
	bitmapInfo.bmiHeader.biHeight = -h;		// bitmap이 거꾸로 출력될 경우 biHeight에 -값을 넣으면 된다.
	bitmapInfo.bmiHeader.biBitCount = bpp;

	double ResizeRate = 0.0f;

	//가로를 남긴다. 이미지의 사이즈 비율이 가로가 더 크기 때문에
	double resHgt;
	resHgt = double(rect.Height() * w) / (double)h;
	ResizeRate = (double)rect.Height() / (double)h;
	CRect rc = rect;
	rc.left = (LONG)(rect.Width() / 2 - resHgt / 2);
	rc.right = (LONG)(rc.left + resHgt);


	if (rc.right > rect.right)
	{
		//세로를 남긴다. 이미지의 사이즈 비율이 세로가 더 크기 때문에
		resHgt = double(rect.Width() * (double)h) / (double)w;
		ResizeRate = (double)rect.Width() / (double)h;
		rc = rect;
		rc.top = (LONG)(rect.Height() / 2 - resHgt / 2);
		rc.bottom = (LONG)(rc.top + resHgt);
	}

	pDC->SetStretchBltMode(COLORONCOLOR);
	::StretchDIBits(pDC->GetSafeHdc(), rc.left, rc.top, rc.Width(), rc.Height(),
		0, 0, w, h, mat_frame.data, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	ReleaseDC(pDC);
}
