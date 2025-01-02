#include <pch.hpp>
#include <CTMMouse.hpp>

namespace CTM // (stands for custom)
{
	void CTMMouse::SetCurrentX(int x) noexcept
	{
		m_CurrentX = x;
	}

	void CTMMouse::SetCurrentY(int y) noexcept
	{
		m_CurrentY = y;
	}

	void CTMMouse::SetCurrentPoint(int x, int y) noexcept
	{
		m_CurrentX = x;
		m_CurrentY = y;
	}

	int CTMMouse::GetCurrentX() const noexcept
	{
		return m_CurrentX;
	}
	
	int CTMMouse::GetCurrentY() const noexcept
	{
		return m_CurrentY;
	}

	std::string CTMMouse::GetPosAsStr() const noexcept
	{
		return '(' + std::to_string(m_CurrentX) + ", " + std::to_string(m_CurrentY) + ')';
	}
}