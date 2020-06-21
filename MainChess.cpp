#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "MenuManager.h" 
#include "Board.h"

/*
A chess game implemented using bitboard techniques I already implemented something like this in java and now I want to implement it in C++ which is lighter and faster than java.

The graphical interface of the project is made using the OLC: PixelGameEngine: https://github.com/OneLoneCoder/olcPixelGameEngine
*/

class Chess : public olc::PixelGameEngine {
private:
	MenuManager menu;
	Board board;

public:
	Chess() {
		sAppName = "Chess";
	}
	~Chess() {}

	bool OnUserCreate() override {		
		return menu.OnUserCreate(*this) && board.OnUserCreate(*this);
	}

	bool OnUserUpdate(float fElapsedTime) override {
		Clear(olc::BLANK);

		menu.OnUserUpdate(*this, fElapsedTime);

		return true;
	}
};

int main() {
	Chess chess;
	if (chess.Construct(580, 500, 1, 1))
		chess.Start();
	return 0;
}