#pragma once

#include <queue>
#include <bitset>

#include <CustomWindow.hpp>

namespace CTM // (stands for custom)
{
	enum class CTMKeyEventType
	{
		INVALID, PRESS, RELEASE
	};

	class CTMKeyEvent
	{
	public:
		CTMKeyEvent(CTMKeyEventType type = CTMKeyEventType::INVALID) noexcept;
	private:
		CTMKeyEventType m_Type;
		unsigned char m_Code;
	};

	class CTNKeyboard
	{
		friend class CTMWindow;
	public:
	};
}