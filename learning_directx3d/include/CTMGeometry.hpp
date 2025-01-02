#pragma once

namespace CTM::Geometry // (stands for custom)
{
	struct Pos2
	{
		float x, y;
	};

	struct Pos3
	{
		float x, y, z;
	};

	struct Vertex2
	{
		Pos2 pos;
	};

	struct Vertex3
	{
		Pos3 pos;
	};
}