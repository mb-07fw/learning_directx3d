#pragma once

#pragma region CTMException Makers
// Makes a basic CTMException.
// (Requires Window.h, which is included in pch.hpp)
#define MAKE_CTM_EX															CTM::CTMException(__LINE__, __FILE__)

// Makes a CTMInfoException from the provided vector of info messages.
// Expects vecInfo to be of type const std::vector<std::string>&.
// (Requires Window.h, which is included in pch.hpp)
#define MAKE_CTM_INFO_EX(vecInfo)											CTM::CTMInfoException(__LINE__, __FILE__, vecInfo)

// Makes a CTMWindowException from the last queued error.
// (Requires Window.h, which is included in pch.hpp)
#define MAKE_CTM_WINDOW_EX_LAST												CTM::CTMWindowException(__LINE__, __FILE__, GetLastError())

// Makes a CTMDirectXException from the provided result handle and vector of info messages.
// Expects hResult to be a variable of type HRESULT.
// Expects vecInfo to be of type const std::vector<std::string>&.
// (Requires Window.h, which is included in pch.hpp)
#define MAKE_CTM_DIRECTX_EX(hResult, vecInfo)								CTMDirectXException(__LINE__, __FILE__, hResult, vecInfo)

// Makes a CTMDirectXException if the function call returns an HRESULT that indicates failure via the FAILED macro. 
// Expects hResult to be a variable of type HRESULT.
// (Requires Window.h, which is included in pch.hpp)
#define MAKE_CTM_DEVICE_REMOVED_EX(hResultReason)							CTMDeviceRemovedException(__LINE__, __FILE__, hResultReason)
#pragma endregion

#pragma region CTMException Throwers
// Makes and throws a basic CTMException.
// (Requires Window.h, which is included in pch.hpp)
#define THROW_CTM_EX														throw MAKE_CTM_EX

// Makes and throws a CTMInfoException from the provided vector of info messages.
// Expects vecInfo to be of type const std::vector<std::string>&.
// (Requires Window.h, which is included in pch.hpp)
#define THROW_CTM_INFO_EX(vecInfo)											throw MAKE_CTM_INFO_EX(vecInfo)

// Makes and throws a CTMWindowException from the provided result handle.
// Expects hResult to be a variable of type HRESULT.
// (Requires Window.h, which is included in pch.hpp)
#define THROW_CTM_WINDOW_EX(hResult)										throw MAKE_CTM_WINDOW_EX(hResult)

// Makes and throws a CTMWindowException from the last queued error.
// (Requires Window.h, which is included in pch.hpp)
#define THROW_CTM_WINDOW_EX_LAST											throw MAKE_CTM_WINDOW_EX_LAST
#pragma endregion

#pragma region CTMException Conditionals
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

// Makes and throws a CTMDirectXException if the function call returns an HRESULT that indicates failure via the FAILED macro. 
// Expects hResult to be a variable of type HRESULT.
// Expects func to be a function that returns an HRESULT.
// (Requires Window.h, which is included in pch.hpp)
#define GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(hResult, func)				if (FAILED(hResult = func)) \
																				throw MAKE_CTM_DIRECTX_EX(hResult, m_InfoManager.CollectAndGetMessages())

// (Used only in CTMGraphics, as it requires access to it's members)
// Makes and throws a CTMDeviceRemovedException if the HRESULT == DXGI_ERROR_DEVICE_REMOVED.
// Expects hResult to be a variable of type HRESULT.
// (Requires Window.h, which is included in pch.hpp)
#define GFX_IF_DEVICE_REMOVED_THROW_CTM_DEVICE_REMOVED_EX(hResult)			if (hResult == DXGI_ERROR_DEVICE_REMOVED) \
																				throw MAKE_CTM_DEVICE_REMOVED_EX(mP_Device->GetDeviceRemovedReason())

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
#pragma endregion