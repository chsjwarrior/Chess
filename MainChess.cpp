#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "GUIComponent.h"
#include "BoardGUI.h"
#include <stack>

/*
A chess game implemented using bitboard techniques I already implemented something like this in java and now I want to implement it in C++ which is lighter and faster than java.
The graphical interface of the project is made using the OLC: PixelGameEngine: https://github.com/OneLoneCoder/olcPixelGameEngine
*/

class GameManager : public olc::PixelGameEngine {
private:
	const uint8_t MENU_SCALE = 2;
	enum class GAME_STATE {
		MAIN_MENU,
		PAUSED_MENU,
		COLOR_CHOICE,
		PAWN_PROMOTION,
		PLAYING,
		EXITING
	};

	gui::MessageBox box;

	BoardGUI board;

	std::stack<GAME_STATE> stack;

	void buildBoxMenu() {
		switch (stack.top()) {
			case GAME_STATE::MAIN_MENU:
				box.getTitle().setText("Chess");
				box.setButtonsGrid(2, 1);
				break;
			case GAME_STATE::PAUSED_MENU:
				box.getTitle().setText("Paused");
				box.setButtonsGrid(3, 1);
				break;
			case GAME_STATE::COLOR_CHOICE:
				box.getTitle().setText("Escolha a sua cor:");
				box.setButtonsGrid(2, 2);
				break;
			case GAME_STATE::PAWN_PROMOTION:
				box.getTitle().setText("Promoção do peao:");
				box.setButtonsGrid(1, 4);
				break;
			case GAME_STATE::EXITING:
				box.getTitle().setText("Deseja realmente sair?");
				box.setButtonsGrid(1, 2);
		}
	}

public:
	GameManager() { sAppName = "Chess"; }
	~GameManager() {}

	bool OnUserCreate() override {
		box.getTitle().setScale(MENU_SCALE);

		stack.push(GAME_STATE::MAIN_MENU);

		buildBoxMenu();

		return board.OnUserCreate(*this);
	}

	bool OnUserUpdate(float fElapsedTime) override {
		Clear(olc::BLANK);

		if (GetKey(olc::ESCAPE).bPressed) {
			if (stack.size() > 1)
				stack.pop();
			else if (stack.top() == GAME_STATE::PLAYING)
				stack.push(GAME_STATE::PAUSED_MENU);
			box.clear();
		}

		if (stack.top() == GAME_STATE::PLAYING)
			return board.OnUserUpdate(*this, fElapsedTime);

		if (box.empty())
			buildBoxMenu();

		box.onUserUpdate(*this, fElapsedTime, {50,200});

		if (stack.top() == GAME_STATE::COLOR_CHOICE) {
			if (box["Brancas"].isMousePressed()) {
				while (!stack.empty()) stack.pop();
				stack.push(GAME_STATE::PLAYING);
				board.startNewGame(Piece::Color::WHITE);
				box.clear();
			}
			else if (box["Negras"].isMousePressed()) {
				while (!stack.empty()) stack.pop();
				stack.push(GAME_STATE::PLAYING);
				board.startNewGame(Piece::Color::BLACK);
				box.clear();
			}
			else if (box["cancelar"].isMousePressed()) {
				stack.pop();
				box.clear();
			}
		}
		else if (stack.top() == GAME_STATE::EXITING) {
			if (box["sim"].isMousePressed()) {
				box.clear();
				return false;
			}
			else if (box["nao"].isMousePressed()) {
				stack.pop();
				box.clear();
			}
		}
		else {
			if (stack.top() == GAME_STATE::PAUSED_MENU)
				if (box["continue"].isMousePressed()) {
					stack.pop();
					box.clear();
				}
			if (box["novo jogo"].isMousePressed()) {
				stack.push(GAME_STATE::COLOR_CHOICE);
				box.clear();
			}
			else if (box["sair"].isMousePressed()) {
				stack.push(GAME_STATE::EXITING);
				box.clear();
			}
		}
		return true;
	}
};

int main() {
	GameManager game;
	if (game.Construct(580, 500, 1, 1))
		game.Start();
	return 0;
}