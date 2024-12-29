#include <pch.hpp>
#include <Keyboard.hpp>

namespace CTM // (stands for custom)
{
	CTMKeyEvent::CTMKeyEvent(CTMKeyEventType type) noexcept
		: m_Type(type), m_Code(0u)
	{

	}
}