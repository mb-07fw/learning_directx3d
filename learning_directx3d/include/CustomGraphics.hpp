#pragma once

#include <pch.hpp>
#include <CustomDXGIInfoManager.hpp>
#include <CustomException.hpp>

namespace CTM // (stands for custom)
{
	class CTMGraphics
	{
	public:
		CTMGraphics(HWND hWnd);
		~CTMGraphics() = default;
		CTMGraphics(const CTMGraphics&) = delete;
		CTMGraphics& operator=(const CTMGraphics&) = delete;
		CTMGraphics& operator=(CTMGraphics&&) = delete;
	public:
		void EndFrame();
		void ClearBuffer(float red, float green, float blue) noexcept;
		inline void DrawTestTriangle()
		{
			namespace WRL = Microsoft::WRL;

			 // 1. Create the buffers.
			 // 2. Bind the buffers.
			 // 3. Create the shaders.
			 // 4. Bind the shaders.
			 // 5. Draw.

			struct Vertex
			{
				float x, y;
			};

			const Vertex vertices[] = {
				{  0.0f,  0.5f },
				{  0.5f, -0.5f },
				{ -0.5f, -0.5f }
			};

			const UINT stride = sizeof(Vertex);
			const UINT offset = 0;

			// Initialize buffer obj and it's description.
			WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
			D3D11_BUFFER_DESC bufDesc = {};
			bufDesc.ByteWidth = sizeof(vertices);
			bufDesc.Usage = D3D11_USAGE_DEFAULT;
			bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufDesc.CPUAccessFlags = 0;
			bufDesc.MiscFlags = 0;
			bufDesc.StructureByteStride = stride;

			// Initialize the buffer's sub data.
			D3D11_SUBRESOURCE_DATA subData = {};
			subData.pSysMem = vertices;

			// Create the buffer.
			HRESULT hResult;
			GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
				hResult,
				mP_Device->CreateBuffer(&bufDesc, &subData, &pVertexBuffer)
			);

			mP_Context->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

			GFX_IF_FUNC_CAUSES_ERROR_THROW_CTM_INFO_EX(
				mP_Context->Draw(3, 0)
			);
		}
	private:
		static constexpr unsigned char SM_SYNC_INTERVAL = 1u;
	private:
		CTMDXGIInfoManager m_InfoManager;
		Microsoft::WRL::ComPtr<ID3D11Device> mP_Device = nullptr;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mP_Swap = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mP_Context;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mP_Target = nullptr;
	};
}