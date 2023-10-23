#include "scanner.h"
#include <algorithm>

void Scanner::AddEventListener(ScreenScanEventHandler handler_ptr) noexcept
{
	if (handler_ptr)
		m_listeners->push_back(handler_ptr);
}

void Scanner::RemoveEventListener(ScreenScanEventHandler existing_handler) noexcept
{
	auto pos = std::find(m_listeners->begin(), m_listeners->end(), existing_handler);
	if (pos != m_listeners->end())
	{
		m_listeners->erase(pos);
	}
}

void Scanner::ToggleScan()
{
	m_scan_active = !m_scan_active;
}


void Scanner::StartCapturingDesktopWindow(bool show_wnd, std::string window_name)
{
	auto desktop = GetDesktopWindow();
	if (show_wnd)
		cv::namedWindow(window_name);
	m_capture_wnd = desktop;
	int key = 0;

	while (true)
	{
		auto frame = Hwnd2Mat(desktop);

		if (frame.empty())
			continue;

		if (m_prev_frame.empty())
		{
			m_prev_frame = frame;
			continue;
		}

		m_curr_frame = frame;

		cv::Mat diff;
		cv::absdiff(m_prev_frame, m_curr_frame, diff);
		cv::Mat grayDiff;
		cv::cvtColor(diff, grayDiff, cv::COLOR_BGR2GRAY);

		auto diffCount = cv::countNonZero(grayDiff);

		cv::imshow(window_name, frame);

		if (diffCount > m_config->GetThreshold())
		{
			RaiseEvent(frame, diffCount);
		}

		if (GetAsyncKeyState(VK_ESCAPE) & 1)
			std::exit(0);
	}
}

void Scanner::RaiseEvent(cv::Mat frame, int diff)
{
	for (const auto func_ptr : *m_listeners)
	{
		if (func_ptr == nullptr)
			continue;
		func_ptr(this, new ScanEvent(frame, diff, SCANEVENT_DETECT));
	}
}

cv::Mat Scanner::Hwnd2Mat(HWND target)
{
	RECT wnd_rect;
	if (!GetWindowRect(target, &wnd_rect))
		throw std::invalid_argument("Given target window is not a window");

	BITMAPINFOHEADER  bi;

	const auto target_width = wnd_rect.right - wnd_rect.left;
	const auto target_height = wnd_rect.bottom - wnd_rect.top;

	auto target_center_x = target_width / 2;
	auto target_center_y = target_height / 2;

	const auto size = m_config->GetScanWindowSize();
	cv::Mat result(size, size, CV_8UC4);

	HDC dc = GetDC(target);
	HDC mem_dc = CreateCompatibleDC(dc);
	SetStretchBltMode(mem_dc, COLORONCOLOR);

	HBITMAP hBitmap = CreateCompatibleBitmap(dc, size, size);
	bi.biSize = sizeof(BITMAPINFOHEADER);    
	bi.biWidth = target_width;
	bi.biHeight = target_height;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	SelectObject(mem_dc, hBitmap);
	BitBlt(mem_dc, 0, 0, size, size, dc, target_center_x - size / 2, target_center_y - size / 2, SRCCOPY);
	GetDIBits(mem_dc, hBitmap, 0, size, result.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	DeleteObject(hBitmap);
	DeleteDC(mem_dc);
	ReleaseDC(target, dc);

	return result;
}