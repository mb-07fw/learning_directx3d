#pragma once

#include <CTMWindow.hpp>
#include <CTMException.hpp>
#include <CTMTimer.hpp>

namespace CTM // (stands for custom)
{
	class CTMApp
	{
		using MillisDuration = std::chrono::duration<double, std::milli>;
	public:
		static constexpr unsigned char SM_TARGET_FRAME_RATE = 60;
	public:
		CTMApp();
		CTMApp(const CTMApp&) = delete;
		CTMApp(CTMApp&&) = delete;
		CTMApp& operator=(const CTMApp&) = delete;
		CTMApp& operator=(CTMApp&&) = delete;
	public:
		[[nodiscard]] WPARAM Start();
		void Tick();
	private:
		static constexpr MillisDuration SM_TARGET_FRAME_DURATION = MillisDuration(1000.0 / SM_TARGET_FRAME_RATE);
	private:
		MillisDuration m_DeltaTime;
		CTMWindow m_Window;
		CTMTimer m_Timer;
	};
}