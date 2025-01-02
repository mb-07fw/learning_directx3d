#include <pch.hpp>
#include <CTMGraphics.hpp>
#include <CTMExceptionMacros.hpp>

namespace CTM // (stands for custom)
{
	CTMGraphics::CTMGraphics(HWND hWnd, const WindowSpace& windowSpaceRef)
		: m_WindowSpaceRef(windowSpaceRef), m_InfoManager(), mP_Device(nullptr),
		  mP_Swap(nullptr), mP_Context(nullptr), mP_Target(nullptr), mP_DepthStencilView(nullptr)
	{
		DXGI_SWAP_CHAIN_DESC swapDesc = {};
		swapDesc.BufferDesc.Width = 0;
		swapDesc.BufferDesc.Height = 0;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapDesc.BufferDesc.RefreshRate.Denominator = 0;
		swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount = 2;
		swapDesc.OutputWindow = hWnd;
		swapDesc.Windowed = TRUE;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapDesc.Flags = 0;

		HRESULT hResult;

		GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
			hResult,
			D3D11CreateDeviceAndSwapChain(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				D3D11_CREATE_DEVICE_DEBUG,
				nullptr,
				0,
				D3D11_SDK_VERSION,
				&swapDesc,
				&mP_Swap,
				&mP_Device,
				nullptr,
				&mP_Context
			)
		);

		// Declare pointer to back buffer.
		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;

		// Set pBackBuffer to the texture subresource in swap chain. (represents the back buffer)
		GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
			hResult,
			mP_Swap->GetBuffer(
				0, __uuidof(ID3D11Resource), &pBackBuffer
			)
		);

		// Set mP_Target to the created RenderTargetView.
		GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
			hResult,
			mP_Device->CreateRenderTargetView(
				pBackBuffer.Get(), nullptr, mP_Target.GetAddressOf()
			)
		);

		// Create depth stencil description.
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// Create depth stencil state.
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
		GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
			hResult,
			mP_Device->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState)
		);

		// Bind the stencil state.
		mP_Context->OMSetDepthStencilState(pDepthStencilState.Get(), 1u);

		// Create depth stencil texture.
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = m_WindowSpaceRef.width;
		textureDesc.Height = m_WindowSpaceRef.height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
			hResult,
			mP_Device->CreateTexture2D(&textureDesc, nullptr, &pDepthStencil)
		);

		// Create view of depth stencil texture.
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
			hResult,
			mP_Device->CreateDepthStencilView(
				pDepthStencil.Get(), &dsvDesc, &mP_DepthStencilView
			)
		);

		// Bind depth stencil view.
		mP_Context->OMSetRenderTargets(1, mP_Target.GetAddressOf(), mP_DepthStencilView.Get());
	}

	void CTMGraphics::EndFrame()
	{
		GFX_IF_DEVICE_REMOVED_THROW_CTM_DEVICE_REMOVED_EX(
			mP_Swap->Present(SM_SYNC_INTERVAL, 0u)
		);
	}

	void CTMGraphics::ClearBuffer(float red, float green, float blue) noexcept
	{
		const float colors[] = { red, green, blue, 1.0f };
		mP_Context->ClearRenderTargetView(mP_Target.Get(), colors);
		mP_Context->ClearDepthStencilView(mP_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
}