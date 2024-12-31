#pragma once

#include <pch.hpp>
#include <CustomGraphics.hpp>
#include <CustomKeyboard.hpp>
#include <CustomMouse.hpp>

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
		[[nodiscard]] bool IsWindowInitialized() const noexcept;
		[[nodiscard]] const char* GetWndClass() noexcept;
		[[nodiscard]] const char* GetWndTitle() noexcept;
		[[nodiscard]] HINSTANCE GetWndInstance() noexcept;
		[[nodiscard]] const std::unique_ptr<CTMGraphics>& GetGraphics() const noexcept;
		[[nodiscard]] HWND GetWndHandle() noexcept;
		[[nodiscard]] const CTMKeyboard& GetKeyboard() const noexcept;
	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lPARAM) noexcept;
		[[nodiscard]] LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	private:
		bool m_IsWindowInitialized = false;
		const char* m_WndClass = nullptr;
		const char* m_WndTitle = nullptr;
		HINSTANCE m_WndInstance = nullptr;
		HWND m_WndHandle = nullptr;
		std::unique_ptr<CTMGraphics> m_Graphics = nullptr;
		CTMKeyboard m_Keyboard;
		CTMMouse m_Mouse;
	};
}