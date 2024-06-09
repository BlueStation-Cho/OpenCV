
// OpenCVMultiCamDlg.h: 헤더 파일
//

#pragma once
#include "CamView2.h"
#include <vector>
#include <memory>
#include <array>

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

// COpenCVMultiCamDlg 대화 상자
class COpenCVMultiCamDlg : public CDialogEx
{
// 생성입니다.
public:
	COpenCVMultiCamDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCVMULTICAM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CRect GetPictureRect(const int i, const int col) const;
	CRect GetPictureRect2x2(const int i) const;
	void EmptyCam(const int pos, const int col);

	std::array<CStatic, 12> m_pictures;
	std::vector<cv::String> streamAddress_;
	std::vector<std::shared_ptr<CamView2>> camViewVec_;
};
