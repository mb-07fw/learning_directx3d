#include <pch.hpp>
#include <CTMBindable.hpp>

namespace CTM::Graphics // (stands for custom)
{
	CTMBindable::CTMBindable(const CTMGraphicsInterface& graphicsInterface) noexcept
		: m_GraphicsInterface(graphicsInterface)
	{
	}
}