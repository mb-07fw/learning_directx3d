#pragma once

namespace CTM // (stands for custom)
{
	/*enum class CTMKeyEventType
	{
		INVALID = -1, PRESS, RELEASE
	};

	class CTMKeyEvent
	{
	public:
		CTMKeyEvent(CTMKeyEventType type = CTMKeyEventType::INVALID, unsigned char keyCode = 0u) noexcept;
	private:
		CTMKeyEventType m_Type;
		unsigned char m_KeyCode;
	};*/

	class CTMKeyboard
	{
	public:
		CTMKeyboard() = default;
		CTMKeyboard(const CTMKeyboard&) = delete;
		CTMKeyboard(CTMKeyboard&&) = delete;
		CTMKeyboard& operator=(const CTMKeyboard&) = delete;
		CTMKeyboard& operator=(CTMKeyboard&&) = delete;
	public:
		// Sets the state of the key code (m_KeyStates[keyCode]) to true.
		void OnKeyPressed(unsigned char keyCode) noexcept;

		// Sets the state of the key code (m_KeyStates[keyCode]) to false.
		void OnKeyReleased(unsigned char keyCode) noexcept;

		// Appends the character to the current string buffer.
		void OnChar(char character) noexcept;

		// Resets the state of each bit in the bitset m_KeyStates to false.
		void ResetKeyStates() noexcept;

		// Clears the current string buffer.
		void ClearCharBuffer() noexcept;

		// Returns true if the key is current being pressed, or false if not.
		[[nodiscard]] bool KeyIsPressed(unsigned char keycode) const noexcept;

		// Returns true if auto-repeat is enabled, or false if not.
		[[nodiscard]] bool IsAutoRepeatEnabled() const noexcept;

		// Returns a reference to the current buffer.
		[[nodiscard]] const std::string& GetCharBuffer() noexcept;
	private:
		// A static constant representing the number of keyCodes that are tracked in the bitset m_KeyStates.
		static constexpr unsigned int s_MaxKeys = 256u;
		
		// A basic boolean flag representing the auto-repeating of text.
		bool m_AutoRepeatEnabled = true;

		// An std::bitset that represents each key code as true for pressed, or false if released or not pressed.
		// Each key is indexed into by it's key code.
		std::bitset<s_MaxKeys> m_KeyStates;
		
		// A standard buffer for collected keys. Appended to in OnChar.
		std::string m_CharBuffer;
	};
}