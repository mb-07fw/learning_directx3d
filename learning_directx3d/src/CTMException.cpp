#include <pch.hpp>
#include <CTMException.hpp>

namespace CTM // (stands for custom)
{
	#pragma region CTMException
	CTMException::CTMException(unsigned int line, const char* file)
		: m_Line(line), m_File(file) 
	{
	}

	const char* CTMException::what() const noexcept
	{
		if (!m_DefinedWhat)
		{
			std::ostringstream oss;
			DefineWhatMessage(oss);

			m_WhatBuffer = oss.str();
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

	void CTMException::AppendTypeString(std::ostringstream& oss) const
	{
		oss << GetType() << '\n';
	}

	void CTMException::AppendOriginString(std::ostringstream& oss) const
	{
		oss << "[File] " << m_File << '\n' << "[Line] " << m_Line;
	}
	#pragma endregion

	#pragma region CTMInfoException
	CTMInfoException::CTMInfoException(unsigned int line, const char* file, const std::vector<std::string>& infoMessages)
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

	void CTMInfoException::DefineWhatMessage(std::ostringstream& oss) const
	{
		AppendTypeString(oss);
		AppendErrorInfo(oss);
		AppendOriginString(oss);
	}

	void CTMInfoException::AppendErrorInfo(std::ostringstream& oss) const
	{
		oss << "[Debug Info] " << m_ErrorInfo;
	}
	#pragma endregion

	#pragma region HResultException
	CTMHResultException::CTMHResultException(unsigned int line, const char* file, HRESULT hResult)
		: CTMException(line, file), m_HResult(hResult)
	{
	}

	void CTMHResultException::AppendErrorString(std::ostringstream& oss) const
	{
		oss << "[Error Code] " << m_HResult << '\n' << "[Error Message] " << TranslateHResult(m_HResult);
	}

	HRESULT CTMHResultException::GetHResult() const noexcept
	{
		return m_HResult;
	}

	void CTMHResultException::DefineWhatMessage(std::ostringstream& oss) const
	{
		AppendTypeString(oss);
		AppendErrorString(oss);
		AppendOriginString(oss);
	}

	std::string CTMHResultException::TranslateHResult(HRESULT hResult) const
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
	CTMWindowException::CTMWindowException(unsigned int line, const char* file, HRESULT hResult)
		: CTMHResultException(line, file, hResult)
	{
	}

	const char* CTMWindowException::GetType() const noexcept
	{
		return "CTMWindowException";
	}
	#pragma endregion

	#pragma region CTMDirectXException
	CTMDirectXException::CTMDirectXException(unsigned int line, const char* file, HRESULT hResult, const std::vector<std::string>& infoMessages)
		: CTMHResultException(line, file, hResult)
	{
		std::ostringstream oss;
		for (const std::string& message : infoMessages)
			oss << message << '\n';

		m_ErrorInfo = oss.str();
	}

	const char* CTMDirectXException::GetType() const noexcept
	{
		return "CTMDirectXException";
	}

	void CTMDirectXException::DefineWhatMessage(std::ostringstream& oss) const
	{
		AppendTypeString(oss);
		AppendErrorString(oss);
		AppendErrorInfo(oss);
		AppendOriginString(oss);
	}

	void CTMDirectXException::AppendErrorInfo(std::ostringstream& oss) const
	{
		oss << "[Debug Info] " << m_ErrorInfo;
	}
	#pragma endregion

	#pragma region CTMDeviceRemovedException
	CTMDeviceRemovedException::CTMDeviceRemovedException(unsigned int line, const char* file, HRESULT hResult)
		: CTMHResultException(line, file, hResult)
	{
	}

	const char* CTMDeviceRemovedException::GetType() const noexcept
	{
		return "CTMDeviceRemovedException";
	}
	#pragma endregion
}