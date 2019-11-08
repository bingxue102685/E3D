#pragma once

#include "ECommon.h"
#include <vector>
#include <Windows.h>

namespace E3D
{
	class EInputListener;
	class ECanvas;

	//��Ϸ���ڹ���
	class EGameWindow
	{
	public:
		static EGameWindow *GWindow;

		//�¼�����
		void startLoop();

		EGameWindow(const EString &windowTitle, HINSTANCE hInstance);
		~EGameWindow();

		//��ʾ����
		void showWindow(EBool show);
		void updateWindow();

		void quiteApplication();
		HWND getHWnd() const { return mHwnd; }
		HINSTANCE getHInstance() const { return mHInstance; }

		void addInputListener(EInputListener *listener);
		void removeInputLIstener(EInputListener *listener);

		void setCanvasListener(ECanvas *canvasListener);

	protected:
		//ȫ��win�¼��ص�����
		static LRESULT WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		//���ڻ���
		virtual void onPaint(HDC hdc);

		virtual void onUpdate();
		
		virtual void onKeyDown(EInt msg);
		virtual void onKeyRelease(EInt msg);
		virtual void onMousePress(bool rightPress);
		virtual void onMouseMove(EInt x, EInt y);
		virtual void onMouseWheel(EInt delta);
	protected:
		//ע�ᴰ��
		bool registerWindow();
		//��������
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
