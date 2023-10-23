#ifndef SCANNER_H
#define SCANNER_H

#include <opencv2/opencv.hpp>

#include "Config.h"
#include "scan_event.h"
#include <Windows.h>

class Scanner;

typedef void(__stdcall* ScreenScanEventHandler)(Scanner*, ScanEvent*);

class Scanner
{
public:
	Scanner(utrigger_config::Config* config)
		:m_scan_active(false), m_enable_raising_events(true), m_capture_wnd(nullptr)
	{
		if (config == nullptr)
			throw std::invalid_argument("config object can not be nullptr");

		m_listeners = new std::vector<ScreenScanEventHandler>();
		m_config = config;
	}

	~Scanner()
	{
		m_listeners->clear();
		delete m_listeners;
	}

	void AddEventListener(ScreenScanEventHandler) noexcept;
	void RemoveEventListener(ScreenScanEventHandler) noexcept;
	void ToggleScan();
	void StartCapturingDesktopWindow(bool show_wnd = false, std::string window_name = "utrigger_cap_preview");
	void SetTarget(HWND);

private:
	cv::Mat Hwnd2Mat(HWND target);
	void RaiseEvent(cv::Mat, int);


	bool m_scan_active;
	bool m_enable_raising_events;
	std::vector<ScreenScanEventHandler>* m_listeners;
	utrigger_config::Config* m_config;

	cv::Mat m_prev_frame;
	cv::Mat m_curr_frame;
	HWND m_capture_wnd;
};


#endif 