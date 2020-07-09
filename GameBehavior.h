#pragma once
#include"olcPixelGameEngine.h"

class GameBehavior {
public:
	GameBehavior() {}
	virtual ~GameBehavior() {}

	virtual bool OnUserCreate(olc::PixelGameEngine& olc) = 0;

	virtual bool OnUserUpdate(olc::PixelGameEngine& olc, float elapsedTime) = 0;
};