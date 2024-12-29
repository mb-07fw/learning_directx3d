#include <pch.hpp>
#include <CustomException.hpp>

namespace CTM // (stands for custom)
{
	#pragma region Exception
	CTMException::CTMException(unsigned int line, const char* file)	noexcept
		: m_Line(line), m_File(file)
	{

	}

	const char* CTMException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << '\n' << GetOriginString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	unsigned int CTMException::GetLine() const noexcept
	{
		return m_Line;
	}

	const std::string& CTMException::GetFile() const noexcept
	{
		return m_File;
	}

	std::string CTMException::GetOriginString() const noexcept
	{
		std::ostringstream oss;
		oss << "[File] " << m_File << '\n' << "[Line] " << m_Line;

		return oss.str();
	}

	const char* CTMException::GetType() const noexcept
	{
		return "CTMException";
	}
	#pragma endregion

	#pragma region WindowException
	CTMWindowException::CTMWindowException(HRESULT result, unsigned int line, const char* file) noexcept
		: m_HResult(result), CTMException(line, file)
	{

	}

	const char* CTMWindowException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() <<  '\n' << GetErrorString() << '\n' << GetOriginString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	const char* CTMWindowException::GetType() const noexcept
	{
		return "CTMWindowException";
	}

	std::string CTMWindowException::GetErrorString() const noexcept
	{
		std::ostringstream oss;
		oss << "[Error : " << m_HResult << "] " << TranslateErrorCode(m_HResult);

		return oss.str();
	}

	HRESULT CTMWindowException::GetErrorCode() const noexcept
	{
		return m_HResult;
	}

	std::string CTMWindowException::TranslateErrorCode(HRESULT result)
	{
		char* pMsgBuf = nullptr;
		DWORD nMsgLength = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			result,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&pMsgBuf),
			0,
			nullptr
		);

		if (nMsgLength == 0)
			return "Unidentified error code.";

		std::string errorString = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorString;
	}
	#pragma endregion
}