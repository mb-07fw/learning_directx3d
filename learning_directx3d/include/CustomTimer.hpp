#pragma once

namespace CTM // (stands for custom)
{
	enum class CTMTimerStatusType
	{
		IDLE, RUNNING, STOPPED
	};

	// TODO : Refactor timer to return primitive doubles, or a reference to a cached m_ElapsedDuration variable.
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
	public:
		// Expects an std::chrono::duration specialization such as std::chrono::milliseconds, and returns the elapsed time_point of that duration.
		// Uses std::chrono::duration<double, std::milli> as template duration by default.
		template <typename Duration = std::chrono::duration<double, std::milli>>
		[[nodiscard]] Duration ElapsedDuration() const noexcept
		{
			return Duration(HighResClock::now() - m_StartTime);
		}
	private:
		template <typename TimeUnit>
		[[nodiscard]] static double TimePointToDouble(const HighResClock::time_point& tp) noexcept
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