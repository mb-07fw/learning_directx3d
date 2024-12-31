#pragma once

#include <pch.hpp>

namespace CTM // (stands for custom)
{
	class CTMException : public std::exception
	{
	public:
		CTMException(unsigned int line, const char* file) noexcept;
		CTMException(const CTMException&) noexcept = default;
		virtual ~CTMException() noexcept = default;
		CTMException& operator=(const CTMException&) noexcept = default;
		CTMException& operator=(CTMException&&) noexcept = default;
	public:
		// Returns the type of the exception in string format.
		virtual const char* GetType() const noexcept = 0;
		
		// Returns the full message of the exception.
		const char* what() const noexcept override;

		// Returns the file name where the exception occured.
		const char* GetFile() const noexcept;

		// Returns the line where the exception occured.
		unsigned int GetLine() const noexcept;
	protected:
		// Sets all exception info (origin, etc) to the what message.
		virtual void DefineWhatMessage() const noexcept = 0;

		// Appends the origin string to the string buffer.
		void AppendOriginString() const noexcept;

		// Clears the string buffer.
		void ClearStringBuffer() const noexcept;
	private:
		unsigned int m_Line;
		const char* m_File;
	protected:
		mutable bool m_DefinedWhat = false;
		mutable std::string m_WhatBuffer;
		mutable std::ostringstream m_StringBuffer;
	};

	class CTMHResultException : public CTMException
	{
	public:
		CTMHResultException(unsigned int line, const char* file, HRESULT hResult) noexcept;
		CTMHResultException(const CTMHResultException&) noexcept = default;
		virtual ~CTMHResultException() noexcept = default;
		CTMHResultException& operator=(const CTMHResultException&) noexcept = default;
		CTMHResultException& operator=(CTMHResultException&&) noexcept = default;
	public:
		// Returns the type of the exception in string format.
		virtual const char* GetType() const noexcept override = 0;

		// Appends the description of the HRESULT error to the string buffer.
		void AppendErrorString() const noexcept;

		// Returns the underlying HRESULT.
		HRESULT GetHResult() const noexcept;
	public:
		static std::string TranslateHResult(HRESULT result) noexcept;
	protected:
		// Sets all exception info (origin, etc) to the what buffer.
		void DefineWhatMessage() const noexcept override;
	private:
		HRESULT m_HResult;
	};

	class CTMInfoException : public CTMException
	{
	public:
		CTMInfoException(unsigned int line, const char* file, const std::vector<std::string>& infoMessages) noexcept;
		CTMInfoException(const CTMInfoException&) noexcept = default;
		~CTMInfoException() = default;
		CTMInfoException& operator=(const CTMInfoException&) noexcept = default;
		CTMInfoException& operator=(CTMInfoException&&) noexcept = default;
	public:
		// Returns the type of the exception in string format.
		virtual const char* GetType() const noexcept override;
	protected:
		// Sets all exception info (origin, etc) to the what message.
		void DefineWhatMessage() const noexcept override;

		// Appends the error info to the string buffer.
		void AppendErrorInfo() const noexcept;
	private:
		std::string m_ErrorInfo;
	};

	class CTMWindowException : public CTMHResultException
	{
	public:
		CTMWindowException(unsigned int line, const char* file, HRESULT hResult) noexcept;
		CTMWindowException(const CTMWindowException&) noexcept = default;
		~CTMWindowException() = default;
		CTMWindowException& operator=(const CTMWindowException&) noexcept = default;
		CTMWindowException& operator=(CTMWindowException&&) noexcept = default;
	public:
		// Returns the type of the exception in string format.
		const char* GetType() const noexcept override;
	};

	class CTMDirectXException : public CTMHResultException
	{
	public:
		CTMDirectXException(unsigned int line, const char* file, HRESULT hResult, const std::vector<std::string>& infoMessages) noexcept;
		CTMDirectXException(const CTMDirectXException&) noexcept = default;
		~CTMDirectXException() noexcept = default;
		CTMDirectXException& operator=(const CTMDirectXException&) noexcept = default;
		CTMDirectXException& operator=(CTMDirectXException&&) noexcept = default;
	public:
		// Returns the type of the exception in string format.
		const char* GetType() const noexcept override;
	protected:
		// Sets all exception info (origin, etc) to the what message.
		void DefineWhatMessage() const noexcept override;

		// Appends the error info to the string buffer.
		void AppendErrorInfo() const noexcept;
	private:
		// A string of all error messages.
		std::string m_ErrorInfo;
	};

	class CTMDeviceRemovedException : public CTMHResultException
	{
	public:
		CTMDeviceRemovedException(unsigned int line, const char* file, HRESULT hResult) noexcept;
		CTMDeviceRemovedException(const CTMDeviceRemovedException&) noexcept = default;
		~CTMDeviceRemovedException() noexcept = default;
		CTMDeviceRemovedException& operator=(const CTMDeviceRemovedException&) noexcept = default;
		CTMDeviceRemovedException& operator=(CTMDeviceRemovedException&&) noexcept = default;
	public:
		// Returns the type of the exception in string format.
		const char* GetType() const noexcept override;
	};
}

// Makes a basic CTMException.
// (Requires Window.h, which is included in pch.hpp)
#define MAKE_CTM_EX															CTM::CTMException(__LINE__, __FILE__)

// Makes and throws a basic CTMException.
// (Requires Window.h, which is included in pch.hpp)
#define THROW_CTM_EX														throw CTM::CTMException(__LINE__, __FILE__)

// Makes a CTMInfoException from the provided vector of info messages.
// Expects vecInfo to be of type const std::vector<std::string>&.
// (Requires Window.h, which is included in pch.hpp)
#define MAKE_CTM_INFO_EX(vecInfo)											CTM::CTMInfoException(__LINE__, __FILE__, vecInfo)

// Makes and throws a CTMInfoException from the provided vector of info messages.
// Expects vecInfo to be of type const std::vector<std::string>&.
// (Requires Window.h, which is included in pch.hpp)
#define THROW_CTM_INFO_EX(vecInfo)											throw MAKE_CTM_INFO_EX(vecInfo)

// Makes a CTMWindowException from the last queued error.
// (Requires Window.h, which is included in pch.hpp)
#define MAKE_CTM_WINDOW_EX_LAST												CTM::CTMWindowException(__LINE__, __FILE__, GetLastError())

// Makes and throws a CTMWindowException from the provided result handle.
// Expects hResult to be a variable of type HRESULT.
// (Requires Window.h, which is included in pch.hpp)
#define THROW_CTM_WINDOW_EX(hResult)										throw MAKE_CTM_WINDOW_EX(hResult)

// Makes and throws a CTMWindowException from the last queued error.
// (Requires Window.h, which is included in pch.hpp)
#define THROW_CTM_WINDOW_EX_LAST											throw MAKE_CTM_WINDOW_EX_LAST

// Makes a CTMDirectXException from the provided result handle and vector of info messages.
// Expects hResult to be a variable of type HRESULT.
// Expects vecInfo to be of type const std::vector<std::string>&.
// (Requires Window.h, which is included in pch.hpp)
#define MAKE_CTM_DIRECTX_EX(hResult, vecInfo)								CTMDirectXException(__LINE__, __FILE__, hResult, vecInfo)

// Makes a CTMDirectXException if the function call returns an HRESULT that indicates failure via the FAILED macro. 
// Expects hResult to be a variable of type HRESULT.
// (Requires Window.h, which is included in pch.hpp)
#define MAKE_CTM_DEVICE_REMOVED_EX(hResultReason)							CTMDeviceRemovedException(__LINE__, __FILE__, hResultReason)

// Makes and throws a CTMDirectXException if the function call returns an HRESULT that indicates failure via the FAILED macro. 
// Expects hResult to be a variable of type HRESULT.
// Expects func to be a function that returns an HRESULT.
// (Requires Window.h, which is included in pch.hpp)
#define GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(hResult, func)				if (FAILED(hResult = func)) \
																			{ \
																				throw MAKE_CTM_DIRECTX_EX(hResult, m_InfoManager.GetMessages()); \
																			}

// (Used only in CTMGraphics, as it requires access to it's members)
// Makes and throws a CTMDeviceRemovedException if the HRESULT == DXGI_ERROR_DEVICE_REMOVED.
// Expects hResult to be a variable of type HRESULT.
// (Requires Window.h, which is included in pch.hpp)
#define GFX_IF_DEVICE_REMOVED_THROW_CTM_DEVICE_REMOVED_EX(hResult)			if (hResult == DXGI_ERROR_DEVICE_REMOVED) \
																				throw MAKE_CTM_DEVICE_REMOVED_EX(mP_Device->GetDeviceRemovedReason())

// Makes and throws a CTMDirectXException if the function call returns an HRESULT that indicates failure via the FAILED macro. 
// Expects hResult to be a variable of type HRESULT.
// Expects func to be a function that returns an HRESULT.
// Expects vecInfo to be of type const std::vector<std::string>&.
// (Requires Window.h, which is included in pch.hpp)
#define INFO_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(hResult, func, vecInfo)	if (FAILED(hResult = func)) \
																				throw MAKE_CTM_DIRECTX_EX(hResult, vecInfo)

// Makes and throws a CTMDirectXException if the function call returns an HRESULT that indicates failure via the FAILED macro. 
// Expects hResult to be a variable of type HRESULT.
// Expects func to be a function that returns an HRESULT.
// (Requires Window.h, which is included in pch.hpp)
#define IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(hResult, func)					if (FAILED(hResult = func)) \
																				throw MAKE_CTM_DIRECTX_EX(hResult, {"No info provided."})

// (Used only in CTMGraphics, as it requires access to it's members)
// Makes and throws a CTMInfoException if there are error messages in the info queue.
// Expects func to be a function that is capable of adding error messages to the IDXGIInfoQueue.
// (Requires Window.h, which is included in pch.hpp)
#define GFX_IF_FUNC_CAUSES_ERROR_THROW_CTM_INFO_EX(func)					func; \
																			const std::vector<std::string>& messages = m_InfoManager.CollectAndGetMessages(); \
																			if (messages.size() != 0) \
																				throw MAKE_CTM_INFO_EX(messages)