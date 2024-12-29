#pragma once

#include <pch.hpp>

namespace CTM // (stands for custom)
{
	class CTMWindow
	{
	public:
		CTMWindow(unsigned int width, unsigned int height, const char* className, const char* titleName);
		~CTMWindow() noexcept;
		CTMWindow(const CTMWindow&) = delete;
		CTMWindow(CTMWindow&&) = delete;
		CTMWindow& operator=(const CTMWindow&) = delete;
		CTMWindow& operator=(CTMWindow&&) = delete;
	public:
		// Loops through message queue until a closure is triggered. Returns value of closure.
		[[nodiscard]] int MessageLoop() noexcept;
		[[nodiscard]] const char* GetWndClass() noexcept;
		[[nodiscard]] const char* GetWndTitle() noexcept;
		[[nodiscard]] HINSTANCE GetWndInstance() noexcept;
		[[nodiscard]] HWND GetWndHandle() noexcept;
	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lPARAM) noexcept;
		[[nodiscard]] LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	private:
		const char* m_WndClass;
		const char* m_WndTitle;
		HINSTANCE m_WndInstance;
		HWND m_WndHandle;
	};
}