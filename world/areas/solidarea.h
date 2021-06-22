#pragma once

#include "../gridsystem.h"

struct SolidArea : public LevelArea<SolidArea> {
	SolidArea() = default;
	SolidArea(float x, float y, float w, float h)
		: LevelArea(x, y, w, h)
	{}
};
