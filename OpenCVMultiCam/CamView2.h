#pragma once
#include <opencv2/opencv.hpp>
#include "MyThread.h"
#include <mutex>

// CamView2


class CamView2 : public CWnd
{
	DECLARE_DYNAMIC(CamView2)

public:
	CamView2();
	virtual ~CamView2();

	BOOL Create(const int x, const int y, const int width, const int height, CWnd* parentWnd);
	BOOL Create(const CRect& rect, CWnd* parentWnd);

	void Init(const int i, const cv::String& address, CStatic* picture);


protected:
	DECLARE_MESSAGE_MAP()



private:
	void CreateThread();
	void DrawMat();

	std::unique_ptr<MyThread> thread_{ nullptr };
	CRect viewRect_{};
	CStatic* m_picture;
	cv::VideoCapture* capture;
	cv::Mat mat_frame;
	std::mutex mutex_;
};


