#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "GUIComponent.h" 
#include "Board.h"

/*
A chess game implemented using bitboard techniques I already implemented something like this in java and now I want to implement it in C++ which is lighter and faster than java.
The graphical interface of the project is made using the OLC: PixelGameEngine: https://github.com/OneLoneCoder/olcPixelGameEngine
*/

class GameManager : public olc::PixelGameEngine {
private:
	const uint8_t MENU_SCALE = 2;

	enum class GameState {
		MAIN_MENU,
		PAUSED_MENU,
		COLOR_CHOICE,
		PLAYING,
		EXITING
	};
	GameState currentState, lastState;

	gui::Button btn1, btn2, btn3;
	gui::Table table;

	Board board;

	void buildTable() {
		switch (currentState) {
			case GameState::MAIN_MENU:
				table.getTitle().setText("Chess");
				table.setTable(2, 1);
				btn2.setText("novo jogo");
				btn3.setText("sair");
				table.items.push_back(&btn2);
				table.items.push_back(&btn3);
				break;
			case GameState::PAUSED_MENU:
				table.getTitle().setText("Paused");
				table.setTable(3, 1);
				btn1.setText("continue");
				btn2.setText("novo jogo");
				btn3.setText("exit");
				table.items.push_back(&btn1);
				table.items.push_back(&btn2);
				table.items.push_back(&btn3);
				break;
			case GameState::COLOR_CHOICE:
				table.getTitle().setText("Escolha a sua cor:");
				table.setTable(2, 2);
				btn1.setText("Brancas");
				btn2.setText("Negras");
				btn3.setText("return");
				table.items.push_back(&btn1);
				table.items.push_back(&btn2);
				table.items.push_back(&btn3);
				break;
			case GameState::EXITING:
				table.getTitle().setText("Deseja realmente sair?");
				table.setTable(1, 2);
				btn1.setText("sim");
				btn2.setText("nao");
				table.items.push_back(&btn1);
				table.items.push_back(&btn2);
		}
	}

public:
	GameManager() { sAppName = "Chess"; }
	~GameManager() {}

	bool OnUserCreate() override {
		btn1.setScale(MENU_SCALE);
		btn2.setScale(MENU_SCALE);
		btn3.setScale(MENU_SCALE);
		table.getTitle().setScale(MENU_SCALE);

		currentState = GameState::MAIN_MENU;
		lastState = currentState;

		buildTable();

		return board.OnUserCreate(*this);
	}

	bool OnUserUpdate(float fElapsedTime) override {
		Clear(olc::BLANK);

		if (GetKey(olc::ESCAPE).bPressed)
			if (currentState != lastState) {
				currentState = lastState;
				table.items.clear();
			}

		if (currentState == GameState::PLAYING)
			return board.OnUserUpdate(*this, fElapsedTime);

		if (table.items.empty())
			buildTable();

		table.onUserUpdate(*this, fElapsedTime, {50,200});

		if (currentState == GameState::MAIN_MENU || currentState == GameState::PAUSED_MENU) {
			if (btn1.isMousePressed()) {
				currentState = GameState::PLAYING;
			} else if (btn2.isMousePressed()) {
				currentState = GameState::COLOR_CHOICE;
				table.items.clear();
			} else if (btn3.isMousePressed()) {
				currentState = GameState::EXITING;
				table.items.clear();
			}
		} else if (currentState == GameState::COLOR_CHOICE) {
			if (btn1.isMousePressed()) {
				board.startNewGame(Piece::COLOR::WHITE);
				currentState = GameState::PLAYING;
				lastState = GameState::PAUSED_MENU;
				table.items.clear();
			} else if (btn2.isMousePressed()) {
				board.startNewGame(Piece::COLOR::BLACK);
				currentState = GameState::PLAYING;
				lastState = GameState::PAUSED_MENU;
				table.items.clear();
			} else if (btn3.isMousePressed()) {
				currentState = lastState;
				table.items.clear();
			}
		} else if (currentState == GameState::EXITING) {
			if (btn2.isMousePressed()) {
				currentState = lastState;
				table.items.clear();
			} else if (btn1.isMousePressed()) {
				table.items.clear();
				return false;
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