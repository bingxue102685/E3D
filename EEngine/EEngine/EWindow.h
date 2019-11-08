#pragma once

#include "ECommon.h"
#include <vector>
#include <Windows.h>

namespace E3D
{
	class EInputListener;
	class ECanvas;

	//游戏窗口管理
	class EGameWindow
	{
	public:
		static EGameWindow *GWindow;

		//事件处理
		void startLoop();

		EGameWindow(const EString &windowTitle, HINSTANCE hInstance);
		~EGameWindow();

		//显示窗口
		void showWindow(EBool show);
		void updateWindow();

		void quiteApplication();
		HWND getHWnd() const { return mHwnd; }
		HINSTANCE getHInstance() const { return mHInstance; }

		void addInputListener(EInputListener *listener);
		void removeInputLIstener(EInputListener *listener);

		void setCanvasListener(ECanvas *canvasListener);

	protected:
		//全局win事件回调函数
		static LRESULT WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		//窗口绘制
		virtual void onPaint(HDC hdc);

		virtual void onUpdate();
		
		virtual void onKeyDown(EInt msg);
		virtual void onKeyRelease(EInt msg);
		virtual void onMousePress(bool rightPress);
		virtual void onMouseMove(EInt x, EInt y);
		virtual void onMouseWheel(EInt delta);
	protected:
		//注册窗口
		bool registerWindow();
		//创建窗口
		bool createWindow();
	protected:
		HWND	mHwnd;
		EString mWindowTitle;
		HINSTANCE mHInstance;
		std::vector<EInputListener*> mInputListeners;
		ECanvas *mCanvasListener;
		EBool mQuite;
		typedef std::vector<EInputListener*>::iterator InputIter;
	};
}
