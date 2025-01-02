#pragma once

namespace CTM // (Stands for custom)
{
	enum class CTMMouseEventType
	{
		INVALID = -1,
		LPress,
		LRelease,
		RPress,
		RRelease,
		WheelUp,
		WheelDown,
		Move
	};

	class CTMMouse
	{
	public:
		CTMMouse() = default;
		CTMMouse(const CTMMouse&) = delete;
		CTMMouse(CTMMouse&&) = delete;
		CTMMouse& operator=(const CTMMouse&) = delete;
		CTMMouse& operator=(CTMMouse&&) = delete;
	public:
		void SetCurrentX(int x) noexcept;
		void SetCurrentY(int y) noexcept;
		void SetCurrentPoint(int x, int y) noexcept;
		[[nodiscard]] int GetCurrentX() const noexcept;
		[[nodiscard]] int GetCurrentY() const noexcept;
		[[nodiscard]] std::string GetPosAsStr() const noexcept;
	private:
		int m_CurrentX;
		int m_CurrentY;
	};
}