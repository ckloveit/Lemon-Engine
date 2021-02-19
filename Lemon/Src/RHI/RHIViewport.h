#pragma once
#include "Core/Core.h"

namespace Lemon
{
	struct LEMON_API RHIViewport
	{
	public:
		RHIViewport(const float x = 0.0f, const float y = 0.0f, const float width = 0.0f, const float height = 0.0f, const float depthMin = 0.0f, const float depthMax = 1.0f)
		{
			this->X = x;
			this->Y = y;
			this->Width = width;
			this->Height = height;
			this->DepthMin = depthMin;
			this->DepthMax = depthMax;
		}

		RHIViewport(const RHIViewport& viewport)
		{
			X = viewport.X;
			Y = viewport.Y;
			Width = viewport.Width;
			Height = viewport.Height;
			DepthMin = viewport.DepthMin;
			DepthMax = viewport.DepthMax;
		}

		~RHIViewport() = default;

		bool operator==(const RHIViewport& rhs) const
		{
			return
				X == rhs.X && Y == rhs.Y &&
				Width == rhs.Width && Height == rhs.Height &&
				DepthMin == rhs.DepthMin && DepthMax == rhs.DepthMax;
		}

		bool operator!=(const RHIViewport& rhs) const
		{
			return !(*this == rhs);
		}

		bool IsDefined() const
		{
			return
				X != 0.0f ||
				Y != 0.0f ||
				Width != 0.0f ||
				Height != 0.0f ||
				DepthMin != 0.0f ||
				DepthMax != 0.0f;
		}

		float AspectRatio()    const { return Width / Height; }

		float X = 0.0f;
		float Y = 0.0f;
		float Width = 0.0f;
		float Height = 0.0f;
		float DepthMin = 0.0f;
		float DepthMax = 0.0f;

		static const RHIViewport Undefined;
	};
}
