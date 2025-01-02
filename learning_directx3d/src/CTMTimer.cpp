#include <pch.hpp>
#include <CTMTimer.hpp>

namespace CTM // (stands for custom)
{
	void CTMTimer::StartTimer() noexcept
	{
		m_Status = CTMTimerStatusType::RUNNING;
		m_StartTime = HighResClock::now();
	}

	void CTMTimer::EndTimer() noexcept
	{
		m_Status = CTMTimerStatusType::STOPPED;
		m_EndTime = HighResClock::now();
	}

	double CTMTimer::ElapsedSeconds() const noexcept
	{
		if (m_Status != CTMTimerStatusType::RUNNING)
			return 0.0;

		return std::chrono::duration<double>(HighResClock::now() - m_StartTime).count();
	}

	double CTMTimer::ElapsedMillis() const noexcept
	{
		if (m_Status != CTMTimerStatusType::RUNNING)
			return 0.0;

		return std::chrono::duration<double, std::milli>(HighResClock::now() - m_StartTime).count();
	}

	CTMTimerStatusType CTMTimer::Status() const noexcept
	{
		return m_Status;
	}

	double CTMTimer::StartTimeMillis() const noexcept
	{
		return TimePointToDouble<std::milli>(m_StartTime);
	}

	double CTMTimer::EndTimeMillis() const noexcept
	{
		return TimePointToDouble<std::milli>(m_EndTime);
	}
}