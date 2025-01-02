#include <pch.hpp>
#include <CTMException.hpp>
#include <CTMExceptionMacros.hpp>
#include <CTMDXGIInfoManager.hpp>

namespace CTM // (stands for custom)
{
	CTMDXGIInfoManager::CTMDXGIInfoManager()
	{
		// Define the function signature of DXGIGetDebugInterface.
		typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

		// Load the .dll that contains the function DXGIGetDebugInterface.
		HMODULE hModuleDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
		DXGIGetDebugInterface DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
			reinterpret_cast<void*>(GetProcAddress(hModuleDxgiDebug, "DXGIGetDebugInterface"))
		);

		// Throw if getting the interface failed.
		if (DxgiGetDebugInterface == nullptr)
			THROW_CTM_WINDOW_EX_LAST;

		HRESULT hResult;

		INFO_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
			hResult,
			DxgiGetDebugInterface(
				__uuidof(IDXGIInfoQueue), &mP_InfoQueue
			),
			{"(CTM) Failure occured trying to get IDXGIInfoQueue."}
		);
	}

	void CTMDXGIInfoManager::SetReadIndexToCurrent() noexcept
	{
		// Set the current read index to the number of messages in the info queue, skipping all previous messages.
		m_CurrentReadIndex = mP_InfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	}

	void CTMDXGIInfoManager::CollectMessages()
	{
		UINT64 end = mP_InfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
		HRESULT hResult;

		SIZE_T messageLength = 0;
		constexpr SIZE_T PREDICTED_MESSAGE_LENGTH = 512;
		SIZE_T currentBufferSize = PREDICTED_MESSAGE_LENGTH;

		std::unique_ptr<byte[]> buffer = std::make_unique<byte[]>(PREDICTED_MESSAGE_LENGTH);
		DXGI_INFO_QUEUE_MESSAGE* pMessageBuffer = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(buffer.get());

		for (SIZE_T i = m_CurrentReadIndex; i < end; i++)
		{
			// Store the size of the message into messageLength.
			IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
				hResult,
				mP_InfoQueue->GetMessage(
					DXGI_DEBUG_ALL, i, nullptr, &messageLength
				)
			);

			// Resize the buffer if needed.
			if (messageLength > currentBufferSize)
			{
				currentBufferSize = messageLength;
				buffer = std::make_unique<byte[]>(currentBufferSize);
				pMessageBuffer = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(buffer.get());
			}

			// Store the i-th message into pMessageBuffer.
			IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
				hResult,
				mP_InfoQueue->GetMessageA(
					DXGI_DEBUG_ALL, i, pMessageBuffer, &messageLength
				)
			);

			// Emplace the message in the vector.
			m_Messages.emplace_back(pMessageBuffer->pDescription);
		}
		
		// Clear messages after they've been collected.
		mP_InfoQueue->ClearStoredMessages(DXGI_DEBUG_ALL);
	}

	void CTMDXGIInfoManager::ClearMessages() noexcept
	{
		m_Messages.clear();
	}

	const std::vector<std::string>& CTMDXGIInfoManager::CollectAndGetMessages()
	{
		CollectMessages();

		return m_Messages;
	}

	const std::vector<std::string>& CTMDXGIInfoManager::GetMessages() const noexcept
	{
		return m_Messages;
	}
}