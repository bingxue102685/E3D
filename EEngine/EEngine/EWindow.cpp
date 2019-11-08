#include "EWindow.h"
#include "EGraphics.h"
#include "ECanvas.h"
#include "EInputListener.h"
#include <ctime>
#include <windowsx.h>

namespace E3D 
{
	EGameWindow *EGameWindow::GWindow;

	EGameWindow::EGameWindow(const EString &windowTitle, HINSTANCE hInstance)
	{
		mWindowTitle = windowTitle;
		mHInstance = hInstance;
		mCanvasListener = NULL;
		mQuite = false;

		if (!(registerWindow() && createWindow()))
		{
			throw "Init Window Failed!";
		}
	}

	EGameWindow::~EGameWindow()
	{
		DestroyWindow(mHwnd);
		for (InputIter iter = mInputListeners.begin(); iter != mInputListeners.end(); iter++)
		{
			SafeDelete(*iter);
		}
		SafeDelete(mCanvasListener);
	}

	void EGameWindow::showWindow(EBool show)
	{
		::ShowWindow(mHwnd, show ? SW_SHOW : SW_HIDE);
		::UpdateWindow(mHwnd);
	}
	void EGameWindow::updateWindow()
	{
		::InvalidateRect(mHwnd, NULL, FALSE);
		::UpdateWindow(mHwnd);
	}

	void EGameWindow::setCanvasListener(ECanvas *canvasListener)
	{
		mCanvasListener = canvasListener;
	}

	void EGameWindow::onUpdate()
	{

	}

	void EGameWindow::onPaint(HDC hdc)
	{
		if (mCanvasListener)
		{
			EGraphics::clearBuffer(EColor(0, 0, 0));
			mCanvasListener->update();
			mCanvasListener->onPaint();
			EGraphics::fillBuffer(hdc);
		}
	}

	void EGameWindow::addInputListener(EInputListener *listener)
	{
		mInputListeners.push_back(listener);
	}

	void EGameWindow::removeInputLIstener(EInputListener *listener)
	{
		for (InputIter iter = mInputListeners.begin(); iter != mInputListeners.end();)
		{
			if (*iter == listener)
			{
				mInputListeners.erase(iter++);
			}
			else
			{
				++iter;
			}
		}
	}

	void EGameWindow::onKeyDown(EInt msg)
	{
		for (InputIter iter = mInputListeners.begin(); iter != mInputListeners.end(); iter++)
		{
			(*iter)->keyPress(msg);
		}
	}
	void EGameWindow::onKeyRelease(EInt msg)
	{
		for (InputIter iter = mInputListeners.begin(); iter != mInputListeners.end(); iter++)
		{
			(*iter)->keyRelease(msg);
		}
	}
	void EGameWindow::onMousePress(bool rightPress)
	{
		if (rightPress)
		{
			::MessageBox(0, "Window R onMousePress", 0, 0);
		}
		else
		{
			::MessageBox(0, "Window L onMousePress", 0, 0);
		}
	}
	void EGameWindow::onMouseMove(EInt x, EInt y)
	{
		for (InputIter iter = mInputListeners.begin(); iter != mInputListeners.end(); iter++)
		{
			(*iter)->mouseMove(x, y);
		}
	}
	void EGameWindow::onMouseWheel(EInt delta)
	{
		for (InputIter iter = mInputListeners.begin(); iter != mInputListeners.end(); iter++)
		{
			(*iter)->mouseWheel(delta);
		}
	}

	void EGameWindow::quiteApplication()
	{
		mQuite = true;
	}

	LRESULT EGameWindow::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (!EGameWindow::GWindow)
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		switch (msg)
		{
		case WM_MOVING:
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EGameWindow::GWindow->onPaint(hdc);
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_TIMER:
		{
			EGameWindow::GWindow->updateWindow();
			break;
		}
		case WM_KEYDOWN:
		{
			EGameWindow::GWindow->onKeyDown(wParam);
			break;
		}
		case WM_KEYUP:
		{
			EGameWindow::GWindow->onKeyRelease(wParam);
			break;
		}

		case WM_MOUSEWHEEL:
		{
			EShort delta = GET_WHEEL_DELTA_WPARAM(wParam);
			EGameWindow::GWindow->onMouseWheel(delta * 0.5f / PI);
			break;
		}
		case WM_MOUSEMOVE:
		{
			EInt x = GET_X_LPARAM(lParam);
			EInt y = GET_Y_LPARAM(lParam);
			EGameWindow::GWindow->onMouseMove(x, y);
			break;
		}
		case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	bool EGameWindow::registerWindow()
	{
		WNDCLASS wind;
		{
			wind.style = CS_HREDRAW | CS_VREDRAW;
			wind.lpfnWndProc = (WNDPROC)EGameWindow::WinProc;
			wind.cbClsExtra = 0;
			wind.cbWndExtra = 0;
			wind.hInstance = mHInstance;
			wind.hIcon = LoadIcon(0, IDI_APPLICATION);
			wind.hCursor = LoadCursor(0, IDC_ARROW);
			wind.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wind.lpszMenuName = 0;
			wind.lpszClassName = "E3D";
		}

		if (!RegisterClass(&wind))
		{
			MessageBox(0, "Register Class Failed", 0, 0);
			return false;
		}
		return true;
	}

	bool EGameWindow::createWindow()
	{
		mHwnd = ::CreateWindow("E3D", mWindowTitle.c_str(),
			//窗口不可改变大小
			WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX) & (~WS_THICKFRAME),
			CW_USEDEFAULT, CW_USEDEFAULT,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			0, 0, mHInstance, 0);

		if (!mHwnd)
		{
			::MessageBox(0, "Create Window Failed", 0, 0);
			return false;
		}

		::SetTimer(mHwnd, WM_TIMER, 35, NULL);

		//时间种子
		srand(time(NULL));

		return true;
	}

	void EGameWindow::startLoop()
	{
		MSG msg;
		while (!mQuite)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				//更新逻辑
				onUpdate();
			}
			else
			{
				Sleep(10);
			}
		}
	}
}