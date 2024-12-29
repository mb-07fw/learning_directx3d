#pragma once

#include <chrono>

namespace CTM // (stands for custom)
{
	enum class CTMTimerStatusType
	{
		IDLE, RUNNING, STOPPED
	};

	class CTMTimer
	{
		using HighResClock = std::chrono::high_resolution_clock;
	public:
		CTMTimer() = default;
		void StartTimer() noexcept;
		void EndTimer() noexcept;
		[[nodiscard]] double ElapsedSeconds() const noexcept;
		[[nodiscard]] double ElapsedMillis() const noexcept;
		[[nodiscard]] CTMTimerStatusType Status() const noexcept;
		[[nodiscard]] double StartTimeMillis() const noexcept;
		[[nodiscard]] double EndTimeMillis() const noexcept;
	private:
		template <typename TimeUnit>
		static double TimePointToDouble(const HighResClock::time_point& tp) noexcept
		{
			auto duration = tp.time_since_epoch();  // Duration since epoch
			return std::chrono::duration<double, TimeUnit>(duration).count();  // Convert to seconds as a double
		}
	private:
		CTMTimerStatusType m_Status = CTMTimerStatusType::IDLE;
		HighResClock::time_point m_StartTime;
		HighResClock::time_point m_EndTime;
	};
}