#include <pch.hpp>
#include <CustomKeyboard.hpp>

namespace CTM // (stands for custom)
{
	/*CTMKeyEvent::CTMKeyEvent(CTMKeyEventType type, unsigned char keyCode) noexcept
		: m_Type(type), m_KeyCode(keyCode)
	{

	}*/

	#pragma region CTMKeyboard
	void CTMKeyboard::OnKeyPressed(unsigned char keyCode) noexcept
	{
		m_KeyStates[keyCode] = true;
	}

	void CTMKeyboard::OnKeyReleased(unsigned char keyCode) noexcept
	{
		m_KeyStates[keyCode] = false;
	}

	void CTMKeyboard::OnChar(char character) noexcept
	{
		m_CharBuffer.push_back(character);
	}

	void CTMKeyboard::ResetKeyStates() noexcept
	{
		m_KeyStates.reset();
	}

	void CTMKeyboard::ClearCharBuffer() noexcept
	{
		m_CharBuffer.clear();
	}

	bool CTMKeyboard::KeyIsPressed(unsigned char keyCode) const noexcept
	{
		return m_KeyStates[keyCode];
	}

	bool CTMKeyboard::IsAutoRepeatEnabled() const noexcept
	{
		return m_AutoRepeatEnabled;
	}

	const std::string& CTMKeyboard::GetCharBuffer() noexcept
	{
		return m_CharBuffer;
	}
	#pragma endregion
}