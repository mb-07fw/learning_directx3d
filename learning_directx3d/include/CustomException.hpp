#pragma once

#include <pch.hpp>

namespace CTM // (stands for custom)
{
	class CTMException : public std::exception
	{
	public:
		CTMException(unsigned int line, const char* file) noexcept;
		const char* what() const noexcept;
		unsigned int GetLine() const noexcept;
		const std::string& GetFile() const noexcept;
		std::string GetOriginString() const noexcept;
		virtual const char* GetType() const noexcept;
	private:
		unsigned int m_Line;
		std::string m_File;
	protected:
		mutable std::string m_WhatBuffer;
	};

	class CTMWindowException : public CTMException
	{
	public:
		CTMWindowException(HRESULT result, unsigned int line, const char* file) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorString() const noexcept;
		HRESULT GetErrorCode() const noexcept;
	public:
		static std::string TranslateErrorCode(HRESULT result);
	private:
		HRESULT m_HResult;
	};
}

// Makes a basic CTMException.
#define MAKE_CTM_EX						CTM::CTMException(__LINE__, __FILE__)

// Makes a CTMWindowException from the provided result handle. (Requires Window.h, which is included in pch.hpp)
#define MAKE_CTM_WINDOW_EX(hResult)		CTM::CTMWindowException(hResult, __LINE__, __FILE__)

// Makes and throws a CTMWindowException from the provided result handle. (Requires Window.h, which is included in pch.hpp)
#define THROW_CTM_WINDOW_EX(hResult)	throw MAKE_CTM_WINDOW_EX(hResult)

// Makes a CTMWindowException from the last queued error. (Requires Window.h, which is included in pch.hpp)
#define MAKE_CTM_WINDOW_EX_LAST			CTM::CTMWindowException(GetLastError(), __LINE__, __FILE__)

// Makes and throws a CTMWindowException from the last queued error. (Requires Window.h, which is included in pch.hpp)
#define THROW_CTM_WINDOW_EX_LAST		throw MAKE_CTM_WINDOW_EX_LAST