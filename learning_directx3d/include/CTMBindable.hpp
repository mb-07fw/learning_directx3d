#pragma once

#include <CTMGraphics.hpp>

namespace CTM::Graphics // (stands for custom)
{
	class CTMBindable
	{
	public:
		CTMBindable(const CTMGraphicsInterface& graphicsInterface) noexcept;
		virtual ~CTMBindable() = default;
	public:
		virtual void Bind() const = 0;
	private:
		const CTMGraphicsInterface& m_GraphicsInterface;
	};

	template <typename T>
	class CTMBuffer
	{
	public:
		CTMBuffer() = default;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer() = default;
	};
}