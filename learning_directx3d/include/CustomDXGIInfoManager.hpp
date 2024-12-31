#pragma once

#include <pch.hpp>

namespace CTM // (stands for custom)
{
	class CTMDXGIInfoManager
	{
	public:
		CTMDXGIInfoManager();
		~CTMDXGIInfoManager() = default;
		CTMDXGIInfoManager(const CTMDXGIInfoManager&) = delete;
		CTMDXGIInfoManager& operator=(const CTMDXGIInfoManager&) = delete;
		CTMDXGIInfoManager& operator=(CTMDXGIInfoManager&&) = delete;
	public:
		// Sets the current read index to the current amount of messages within the queue. (The end of the queue)
		void SetReadIndexToCurrent() noexcept;

		// Adds all messages in the info queue from the current read index to the vector of collected messages.
		// Throws if an error occurs.
		void CollectMessages();

		// Clears the vector of collected messages.
		void ClearMessages() noexcept;

		// Collects all messages in the info queue, and then returns a const reference to all collected messages.
		// Throws if an error occurs during collection.
		[[nodiscard]] const std::vector<std::string>& CollectAndGetMessages();

		// Returns a const reference to the vector of collected messages.
		[[nodiscard]] const std::vector<std::string>& GetMessages() const noexcept;
	private:
		unsigned long long m_CurrentReadIndex = 0;
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> mP_InfoQueue = nullptr;
		std::vector<std::string> m_Messages;
	};
}