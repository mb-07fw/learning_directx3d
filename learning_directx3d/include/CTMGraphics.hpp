#pragma once

#include <pch.hpp>
#include <CTMWindowAttributes.hpp>
#include <CTMException.hpp>
#include <CTMExceptionMacros.hpp>
#include <CTMDXGIInfoManager.hpp>

namespace CTM // (stands for custom)
{
	class CTMGraphicsInterface
	{

	};

	class CTMGraphics
	{
	public:
		CTMGraphics(HWND hWnd, const WindowSpace& windowSpaceRef);
		~CTMGraphics() = default;
		CTMGraphics(const CTMGraphics&) = delete;
		CTMGraphics& operator=(const CTMGraphics&) = delete;
		CTMGraphics& operator=(CTMGraphics&&) = delete;
	public:
		void EndFrame();
		void ClearBuffer(float red, float green, float blue) noexcept;
		inline void DrawTestTriangle(float angle, float x, float z)
		{
			// TODO : Move all non-drawing stuff outside of this function, as they all get allocated every single tick.
			// 1. Create the buffers.
			// 2. Bind the buffers.
			// 3. Create the shaders.
			// 4. Bind the shaders.
			// 5. Draw.

			HRESULT hResult;

			struct Vertex
			{
				struct
				{
					float x, y, z;
				} pos;
			};

			//// Rectangle vertices.
			//const Vertex vertices[] = {
			//	{ -0.5f,  0.5f, 1, 0, 0, 1 },
			//	{  0.5f, -0.5f, 0, 1, 0, 1 },
			//	{ -0.5f, -0.5f, 0, 0, 1, 1 },

			//	{ -0.5f,  0.5f, 1, 0, 0, 1 },
			//	{  0.5f,  0.5f, 0, 0, 1, 1 },
			//	{  0.5f, -0.5f, 0, 1, 0, 1 },
			//};

			//// Rectangle indices.
			//const short indices[] = {
			//	0, 1, 2,
			//	3, 4, 5
			//};

			const unsigned short indices[] = {
				0, 2, 1,	2, 3, 1,
				1, 3, 5,	3, 7, 5,
				2, 6, 3,	3, 6, 7,
				4, 5, 7,	7, 6, 4,
				0, 4, 2,	2, 4, 6,
				0, 1, 4,	1, 5, 4
			};

			const Vertex vertices[] = {
				{ -1.0f, -1.0f, -1.0f },
				{  1.0f, -1.0f, -1.0f },
				{ -1.0f,  1.0f, -1.0f },
				{  1.0f,  1.0f, -1.0f },
				{ -1.0f, -1.0f,  1.0f },
				{  1.0f, -1.0f,  1.0f },
				{ -1.0f,  1.0f,  1.0f },
				{  1.0f,  1.0f,  1.0f }
			};

			const UINT vertexStride = sizeof(Vertex);
			const UINT vertexOffset = 0;

			// Initialize buffer obj and it's description.
			Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
			D3D11_BUFFER_DESC vertexBufferDesc = {};
			vertexBufferDesc.ByteWidth = sizeof(vertices);
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = vertexStride;

			// Initialize the buffer's sub data.
			D3D11_SUBRESOURCE_DATA vertexSubData = {};
			vertexSubData.pSysMem = vertices;

			// Create the vertex buffer.
			GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
				hResult,
				mP_Device->CreateBuffer(&vertexBufferDesc, &vertexSubData, &pVertexBuffer)
			);

			// Bind the vertex buffer.
			mP_Context->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &vertexStride, &vertexOffset);

			// Create the index buffer desc.
			Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
			D3D11_BUFFER_DESC indexBufferDesc = {};
			indexBufferDesc.ByteWidth = sizeof(indices);
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = sizeof(short);

			// Create the index buffer's sub data.
			D3D11_SUBRESOURCE_DATA indexBufferSubData;
			indexBufferSubData.pSysMem = indices;

			// Create the index buffer.
			GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
				hResult,
				mP_Device->CreateBuffer(&indexBufferDesc, &indexBufferSubData, pIndexBuffer.GetAddressOf())
			);

			// Bind the index buffer.
			mP_Context->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

			struct ConstantBuffer1
			{
				DirectX::XMMATRIX transform;
			};

			struct ConstantBuffer2
			{
				struct
				{
					float r, g, b, a;
				} faceColors[6];
			};

			const ConstantBuffer1 cb1 = {
				{
					DirectX::XMMatrixTranspose(
						DirectX::XMMatrixRotationZ(angle) *
						DirectX::XMMatrixRotationX(angle) *
						DirectX::XMMatrixTranslation(x, 0.0f, z + 4.0f) *
						DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
					)
				}
			};

			Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer1;
			D3D11_BUFFER_DESC constBufferDesc1;
			constBufferDesc1.ByteWidth = sizeof(cb1);
			constBufferDesc1.Usage = D3D11_USAGE_DYNAMIC;
			constBufferDesc1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constBufferDesc1.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constBufferDesc1.MiscFlags = 0;
			constBufferDesc1.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA constBufferSubData1 = {};
			constBufferSubData1.pSysMem = &cb1;

			GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
				hResult,
				mP_Device->CreateBuffer(&constBufferDesc1, &constBufferSubData1, &pConstantBuffer1)
			);

			mP_Context->VSSetConstantBuffers(0, 1, pConstantBuffer1.GetAddressOf());

			const ConstantBuffer2 cb2 = {
				{
					{ 1.0f, 0.0f, 1.0f },
					{ 1.0f, 0.0f, 0.0f },
					{ 0.0f, 1.0f, 0.0f },
					{ 0.0f, 0.0f, 1.0f },
					{ 1.0f, 1.0f, 0.0f },
					{ 0.0f, 1.0f, 1.0f }
				}
			};

			Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer2;
			D3D11_BUFFER_DESC constBufferDesc2;
			constBufferDesc2.ByteWidth = sizeof(cb2);
			constBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
			constBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constBufferDesc2.MiscFlags = 0;
			constBufferDesc2.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA constBufferSubData2 = {};
			constBufferSubData2.pSysMem = &cb2;

			GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
				hResult,
				mP_Device->CreateBuffer(&constBufferDesc2, &constBufferSubData2, &pConstantBuffer2)
			);

			mP_Context->PSSetConstantBuffers(0, 1, pConstantBuffer2.GetAddressOf());

			// Create the pixel shader.
			Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
			Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;

			// Read the bytecode of the compiled shader into a blob.
			GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
				hResult,
				D3DReadFileToBlob(L"PixelShader.cso", &pBlob)
			);

			// Create the pixel shader from the blob.
			GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
				hResult,
				mP_Device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader)
			);

			// Bind the pixel shader.
			mP_Context->PSSetShader(pPixelShader.Get(), nullptr, 0);

			// Create the vertex shader
			Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;

			// Read the bytecode of the compiled shader into a blob.
			GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
				hResult,
				D3DReadFileToBlob(L"VertexShader.cso", &pBlob)
			);

			// Create the vertex shader from the blob.
			GFX_IF_FUNC_FAILED_THROW_CTM_DIRECTX_EX(
				hResult,
				mP_Device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader)
			);

			// Bind the vertex shader.
			mP_Context->VSSetShader(pVertexShader.Get(), nullptr, 0);

			// Declare the input layout and its describtion.
			Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
			const D3D11_INPUT_ELEMENT_DESC inElementDesc[] = {
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};

			mP_Device->CreateInputLayout(inElementDesc, (UINT)std::size(inElementDesc), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);

			mP_Context->IASetInputLayout(pInputLayout.Get());

			// Bind the render target.
			mP_Context->OMSetRenderTargets(1, mP_Target.GetAddressOf(), mP_DepthStencilView.Get());

			// Set primitive topology to triangle list (group of 3 vertices)
			mP_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// Configure the viewport.
			D3D11_VIEWPORT viewport = {};
			viewport.Width = (FLOAT)m_WindowSpaceRef.width;
			viewport.Height = (FLOAT)m_WindowSpaceRef.height;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;

			// Bind the viewport.
			mP_Context->RSSetViewports(1, &viewport);

			// Draw the vertices.
			GFX_IF_FUNC_CAUSES_ERROR_THROW_CTM_INFO_EX(
				mP_Context->DrawIndexed((UINT)std::size(indices), 0, 0)
			);
		}
	private:
		static constexpr unsigned char SM_SYNC_INTERVAL = 1u;
	private:
		const WindowSpace& m_WindowSpaceRef;
		CTMDXGIInfoManager m_InfoManager;
		Microsoft::WRL::ComPtr<ID3D11Device> mP_Device;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mP_Swap;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mP_Context;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mP_Target;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mP_DepthStencilView;
	};
}