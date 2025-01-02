#pragma once

#include <pch.hpp>

namespace CTM // (stands for custom)
{
	// An abstract base representation of an exception.
	class CTMException : public std::exception
	{
	public:
		// NOTE : this isn't marked noexcept as it performs string operations in DefineWhatMessage, which can throw exceptions.
		CTMException(unsigned int line, const char* file);

		CTMException(const CTMException&) noexcept = default;
		virtual ~CTMException() noexcept = default;
		CTMException& operator=(const CTMException&) noexcept = default;
		CTMException& operator=(CTMException&&) noexcept = default;
	public:		
		// Returns the full message of the exception.
		[[nodiscard]] const char* what() const noexcept override;

		// Returns the file name where the exception occured.
		[[nodiscard]] const char* GetFile() const noexcept;

		// Returns the line where the exception occured.
		[[nodiscard]] unsigned int GetLine() const noexcept;

		// Returns the type of the exception in string format.
		[[nodiscard]] virtual const char* GetType() const noexcept = 0;
	protected:
		// Appends the type string to the provided ostringstream.
		void AppendTypeString(std::ostringstream& oss) const;

		// Appends the origin string to the provided ostringstream.
		void AppendOriginString(std::ostringstream& oss) const;

		// Appends all exception info (origin, etc) to the provided ostringstream.
		virtual void DefineWhatMessage(std::ostringstream& oss) const = 0;
	private:
		// NOTE : Used to defer calling the pure virtual function DefineWhatMessage 
		// until after the derived object is constructed, (in what()) as calling a pure virtual function in the constructor
		// of the object is extremely problematic, and will invoke a-many linker errors.
		mutable bool m_DefinedWhat = false;
		mutable std::string m_WhatBuffer;

		unsigned int m_Line;
		const char* m_File;
	};

	// A concrete representation of errors related to debug info provided
	// through WinAPI or DirectX debug utilities that aren't tied directly to an HRESULT.
	class CTMInfoException : public CTMException
	{
	public:
		// NOTE : This isn't marked noexcept as it's base constructor of CTMException 
		// performs string operations in DefineWhatMessage, which can throw exceptions.
		CTMInfoException(unsigned int line, const char* file, const std::vector<std::string>& infoMessages);

		CTMInfoException(const CTMInfoException&) noexcept = default;
		~CTMInfoException() = default;
		CTMInfoException& operator=(const CTMInfoException&) noexcept = default;
		CTMInfoException& operator=(CTMInfoException&&) noexcept = default;
	public:
		// Returns the type of the exception in string format.
		[[nodiscard]] virtual const char* GetType() const noexcept override;
	protected:
		// Appends all exception info (origin, etc) to the provided ostringstream.
		void DefineWhatMessage(std::ostringstream& oss) const override;
	private:
		// Appends the error info to the string buffer.
		void AppendErrorInfo(std::ostringstream& oss) const;
	private:
		std::string m_ErrorInfo;
	};

	// An abstract representation of errors described by an HRESULT.
	class CTMHResultException : public CTMException
	{
	public:
		// NOTE : This isn't marked noexcept as it's base constructor of CTMException 
		// performs string operations in DefineWhatMessage, which can throw exceptions.
		CTMHResultException(unsigned int line, const char* file, HRESULT hResult);
		CTMHResultException(const CTMHResultException&) = default;
		virtual ~CTMHResultException() = default;
		CTMHResultException& operator=(const CTMHResultException&) = default;
		CTMHResultException& operator=(CTMHResultException&&) = default;
	public:
		// Returns the type of the exception in string format.
		[[nodiscard]] virtual const char* GetType() const noexcept override = 0;

		// Returns the underlying HRESULT.
		[[nodiscard]] HRESULT GetHResult() const noexcept;
	protected:
		// Appends all exception info (origin, etc) to the provided ostringstream.
		void DefineWhatMessage(std::ostringstream& oss) const override;

		// Appends the description of the HRESULT error to the string buffer.
		void AppendErrorString(std::ostringstream& oss) const;
	private:
		// Translates the HRESULT into a string message using FormatMessage from the WinAPI.
		[[nodiscard]] std::string TranslateHResult(HRESULT result) const;
	private:
		HRESULT m_HResult;
	};

	// A concrete representation of errors described by an HRESULT, and are also tied directly to a window throught the WinAPI.
	class CTMWindowException : public CTMHResultException
	{
	public:
		// NOTE : This isn't marked noexcept as it's base constructor of CTMException 
		// performs string operations in DefineWhatMessage, which can throw exceptions.
		CTMWindowException(unsigned int line, const char* file, HRESULT hResult);
		CTMWindowException(const CTMWindowException&) = default;
		~CTMWindowException() = default;
		CTMWindowException& operator=(const CTMWindowException&) = default;
		CTMWindowException& operator=(CTMWindowException&&) = default;
	public:
		// Returns the type of the exception in string format.
		[[nodiscard]] const char* GetType() const noexcept override;
	};

	// A concrete representation of errors described by an HRESULT, and are also described by further
	// debug info provided through Direct3D's debug utilities. (e.g., IDXGIInfoQueue)
	class CTMDirectXException : public CTMHResultException
	{
	public:
		// NOTE : This isn't marked noexcept as both this, and it's base constructor of CTMException 
		// performs string operations, which can throw exceptions.
		CTMDirectXException(unsigned int line, const char* file, HRESULT hResult, const std::vector<std::string>& infoMessages);
		CTMDirectXException(const CTMDirectXException&) = default;
		~CTMDirectXException() = default;
		CTMDirectXException& operator=(const CTMDirectXException&) = default;
		CTMDirectXException& operator=(CTMDirectXException&&) = default;
	public:
		// Returns the type of the exception in string format.
		[[nodiscard]] const char* GetType() const noexcept override;
	protected:
		// Appends all exception info (origin, etc) to the provided ostringstream.
		void DefineWhatMessage(std::ostringstream& oss) const override;
	private:
		// Appends the error info to the provided ostringstream.
		void AppendErrorInfo(std::ostringstream& oss) const;
	private:
		// A string of all error messages.
		std::string m_ErrorInfo;
	};

	class CTMDeviceRemovedException : public CTMHResultException
	{
	public:
		// NOTE : This isn't marked noexcept as it's base constructor of CTMException 
		// performs string operations in DefineWhatMessage, which can throw exceptions.
		CTMDeviceRemovedException(unsigned int line, const char* file, HRESULT hResult);
		CTMDeviceRemovedException(const CTMDeviceRemovedException&) = default;
		~CTMDeviceRemovedException() = default;
		CTMDeviceRemovedException& operator=(const CTMDeviceRemovedException&) = default;
		CTMDeviceRemovedException& operator=(CTMDeviceRemovedException&&) = default;
	public:
		// Returns the type of the exception in string format.
		[[nodiscard]] const char* GetType() const noexcept override;
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
																				throw MAKE_CTM_DIRECTX_EX(hResult, m_InfoManager.CollectAndGetMessages()); \
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
																			{ \
																				const std::vector<std::string>& messages = m_InfoManager.CollectAndGetMessages(); \
																				if (messages.size() != 0) \
																					throw MAKE_CTM_INFO_EX(messages); \
																			}