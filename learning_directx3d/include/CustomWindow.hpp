#pragma once

#include <pch.hpp>
#include <CustomGraphics.hpp>
#include <CustomKeyboard.hpp>
#include <CustomMouse.hpp>

namespace CTM // (stands for custom)
{
	struct WindowSpace
	{
	public:
		WindowSpace(int w, int h, float ar)
			: width(w), height(h), aspectRatio(ar) {}
	public:
		int width, height;
		float aspectRatio;
	};

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
		[[nodiscard]] bool IsWindowInitialized() const noexcept;
		[[nodiscard]] const WindowSpace& GetWindowSpace() const noexcept;
		[[nodiscard]] const char* GetWndClass() noexcept;
		[[nodiscard]] const char* GetWndTitle() noexcept;
		[[nodiscard]] HINSTANCE GetWndInstance() noexcept;
		[[nodiscard]] const std::unique_ptr<CTMGraphics>& GetGraphics() const noexcept;
		[[nodiscard]] HWND GetWndHandle() noexcept;
		[[nodiscard]] const CTMKeyboard& GetKeyboard() const noexcept;
		[[nodiscard]] const CTMMouse& GetMouse() const noexcept;
	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lPARAM) noexcept;
		[[nodiscard]] LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	private:
		bool m_IsWindowInitialized;
		WindowSpace m_WindowSpace;
		const char* m_WndClass;
		const char* m_WndTitle;
		HINSTANCE m_WndInstance;
		HWND m_WndHandle;
		std::unique_ptr<CTMGraphics> m_Graphics; // NOTE : This is a smart pointer to defer creation of this until after the window is created. Fix this bad design later.
		CTMKeyboard m_Keyboard;
		CTMMouse m_Mouse;
	};
}