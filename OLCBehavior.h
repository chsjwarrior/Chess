#pragma once
#include"olcPixelGameEngine.h"

class OLCBehavior {
public:
	OLCBehavior() {}
	virtual ~OLCBehavior() {}

	virtual bool OnUserCreate(olc::PixelGameEngine& olc) = 0;

	virtual bool OnUserUpdate(olc::PixelGameEngine& olc, float elapsedTime) = 0;
};