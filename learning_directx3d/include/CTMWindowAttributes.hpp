#pragma once

namespace CTM // (stands for custom)
{
	struct WindowSpace
	{
	public:
		WindowSpace(int w, int h, float ar)
			: width(w), height(h), aspectRatio(ar) {
		}
	public:
		int width, height;
		float aspectRatio;
	};

}