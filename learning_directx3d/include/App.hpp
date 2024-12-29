#pragma once

#include <CustomWindow.hpp>
#include <CustomException.hpp>
#include <CustomTimer.hpp>

namespace CTM // (stands for custom)
{
	class CTMApp
	{
	public:
		CTMApp();
		CTMApp(const CTMApp&) = delete;
		CTMApp(CTMApp&&) = delete;
		CTMApp& operator=(const CTMApp&) = delete;
		CTMApp& operator=(CTMApp&&) = delete;
	public:
		int Start();
		void Tick();
	private:
		CTMWindow m_Window;
		CTMTimer m_Timer;
	};
}