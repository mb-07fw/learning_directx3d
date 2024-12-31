#include <pch.hpp>
#include <CustomException.hpp>

namespace CTM // (stands for custom)
{
	#pragma region CTMException
	CTMException::CTMException(unsigned int line, const char* file) noexcept
		: m_Line(line), m_File(file) 
	{
	}

	const char* CTMException::what() const noexcept
	{
		if (!m_DefinedWhat)
		{
			ClearStringBuffer();

			DefineWhatMessage();

			m_DefinedWhat = true;
		}

		return m_WhatBuffer.c_str();
	}

	const char* CTMException::GetFile() const noexcept
	{
		return m_File;
	}

	unsigned int CTMException::GetLine() const noexcept
	{
		return m_Line;
	}

	void CTMException::AppendOriginString() const noexcept
	{
		m_StringBuffer << "[File] " << m_File << '\n' << "[Line] " << m_Line;
	}

	void CTMException::ClearStringBuffer() const noexcept
	{
		m_StringBuffer.str("");
	}
	#pragma endregion

	#pragma region CTMInfoException
	CTMInfoException::CTMInfoException(unsigned int line, const char* file, const std::vector<std::string>& infoMessages) noexcept
		: CTMException(line, file)
	{
		std::ostringstream oss;
		for (const std::string& message : infoMessages)
			oss << message << '\n';
		
		m_ErrorInfo = oss.str();
	}

	const char* CTMInfoException::GetType() const noexcept
	{
		return "CTMInfoException";
	}

	void CTMInfoException::DefineWhatMessage() const noexcept
	{
		m_StringBuffer << GetType() << '\n';

		AppendErrorInfo();
		AppendOriginString();

		m_WhatBuffer = m_StringBuffer.str();
	}

	void CTMInfoException::AppendErrorInfo() const noexcept
	{
		m_StringBuffer << "[Debug Info] " << m_ErrorInfo;
	}
	#pragma endregion

	#pragma region HResultException
	CTMHResultException::CTMHResultException(unsigned int line, const char* file, HRESULT hResult) noexcept
		: CTMException(line, file), m_HResult(hResult)
	{
	}

	void CTMHResultException::DefineWhatMessage() const noexcept
	{
		m_StringBuffer << GetType() << '\n';

		AppendErrorString();
		AppendOriginString();

		m_WhatBuffer = m_StringBuffer.str();
	}

	void CTMHResultException::AppendErrorString() const noexcept
	{
		m_StringBuffer << "[Error Code] " << m_HResult << '\n' << "[Error Message] " << TranslateHResult(m_HResult);
	}

	HRESULT CTMHResultException::GetHResult() const noexcept
	{
		return m_HResult;
	}

	std::string CTMHResultException::TranslateHResult(HRESULT hResult) noexcept
	{
		char* pMsgBuf = nullptr;
		DWORD msgLength = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			hResult,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&pMsgBuf),
			0,
			nullptr
		);

		if (msgLength == 0 || pMsgBuf == nullptr)
			return "Unidentified error code.";

		std::string errorString = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorString;
	}
	#pragma endregion

	#pragma region CTMWindowException
	CTMWindowException::CTMWindowException(unsigned int line, const char* file, HRESULT hResult) noexcept
		: CTMHResultException(line, file, hResult)
	{
	}

	const char* CTMWindowException::GetType() const noexcept
	{
		return "CTMWindowException";
	}
	#pragma endregion

	#pragma region CTMDirectXException
	CTMDirectXException::CTMDirectXException(unsigned int line, const char* file, HRESULT hResult, const std::vector<std::string>& infoMessages) noexcept
		: CTMHResultException(line, file, hResult)
	{
		std::ostringstream oss;
		for (const std::string& message : infoMessages)
			oss << message << '\n';

		m_ErrorInfo = oss.str();
	}

	void CTMDirectXException::DefineWhatMessage() const noexcept
	{
		m_StringBuffer << GetType() << '\n';

		AppendErrorString();
		AppendErrorInfo();
		AppendOriginString();

		m_WhatBuffer = m_StringBuffer.str();
	}

	void CTMDirectXException::AppendErrorInfo() const noexcept
	{
		m_StringBuffer << "[Debug Info] " << m_ErrorInfo;
	}

	const char* CTMDirectXException::GetType() const noexcept
	{
		return "CTMDirectXException";
	}
	#pragma endregion

	#pragma region CTMDeviceRemovedException
	CTMDeviceRemovedException::CTMDeviceRemovedException(unsigned int line, const char* file, HRESULT hResult) noexcept
		: CTMHResultException(line, file, hResult)
	{
	}

	const char* CTMDeviceRemovedException::GetType() const noexcept
	{
		return "CTMDeviceRemovedException";
	}
	#pragma endregion
}