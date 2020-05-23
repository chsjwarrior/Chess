#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Position.h"
#include "BitBoard.h"
#include "BitBoardOperations.h"
#include "MoveGenerator.h"

class Chess : public olc::PixelGameEngine {
private:
	const olc::vu2d CELL_SIZE = { 55, 55 };
	const uint32_t BORDER_SIZE = 25;
	const uint32_t BOARD_SIZE = 8 * 55;

	int boardLayer;

	bool whiteTime, checkMate;

	olc::Decal* pieces;

	std::optional<Position> selected;
	std::optional<std::pair<olc::vu2d, olc::vu2d>> lastMove;
	std::optional<std::pair<olc::vf2d, Piece>> movePiece;

	BitBoard bitBoard;

	std::map<Position, Piece, std::less<Position>> board;

	void Choice_Color() {
		FillRect(30, 100, BOARD_SIZE - 5, 100, olc::DARK_BLUE);
		DrawRect(30, 100, BOARD_SIZE - 5, 100, olc::YELLOW);
		DrawString(40, 110, "Please, choice your color to play");
		DrawPartialDecal({ 50,130 }, pieces, { float(pieces::KING * CELL_SIZE.x + (pieces::WHITE * CELL_SIZE.y * 6)), 0 }, CELL_SIZE);
		DrawPartialDecal({ 160,130 }, pieces, { float(pieces::KING * CELL_SIZE.x + (pieces::BLACK * CELL_SIZE.y * 6)), 0 }, CELL_SIZE);

		olc::vu2d mouse;
		mouse.x = GetMouseX();
		mouse.y = GetMouseY();
		if (mouse.x > 50 && mouse.y > 130 && mouse.x < 50 + CELL_SIZE.x && mouse.y < CELL_SIZE.y + 130) {
			DrawRect(50, 130, CELL_SIZE.x, CELL_SIZE.y);
			if (GetMouse(0).bPressed) {
				//player = pieces::WHITE;
				//state = STATE::PLAY;
			}
		} else if (mouse.x > 160 && mouse.y > 130 && mouse.x < 160 + CELL_SIZE.x && mouse.y < CELL_SIZE.y + 130) {
			DrawRect(160, 130, CELL_SIZE.x, CELL_SIZE.y);
			if (GetMouse(0).bPressed) {
				//player = pieces::WHITE;
				//state = STATE::PLAY;
			}
		}
	}

public:
	Chess() {
		sAppName = "Chess";
		boardLayer = 0;
		whiteTime = false;
		checkMate = true;
		pieces = nullptr;
		selected = std::nullopt;
		lastMove = std::nullopt;
		movePiece = std::nullopt;
	}
	~Chess() {
		if (pieces != nullptr)
			delete pieces;
		selected.reset();
		lastMove.reset();
		movePiece.reset();
		board.clear();
	}

	inline const bool isMouseInsideBoard(const olc::vu2d& point) const {
		return point.x >= BORDER_SIZE && point.x < BOARD_SIZE + BORDER_SIZE &&
			point.y >= BORDER_SIZE && point.y < BOARD_SIZE + BORDER_SIZE;
	}

	inline const uint8_t pointToPosition(const uint32_t& value) const {
		return (value - BORDER_SIZE) / CELL_SIZE.x;
	}

	inline const uint32_t positionToPoint(const uint8_t& value) const {
		return value * CELL_SIZE.x + BORDER_SIZE;
	}

	bool OnUserCreate() override {
		pieces = new olc::Decal(new olc::Sprite("PiecesSprite.png"));

		bitBoardOperations::setInitialPosition(bitBoard);

		for (uint8_t i = 0; i < 64; i++) {
			Position pos(i);
			auto piece = bitBoardOperations::getPieceFromSquare(bitBoard, pos.getSquare());
			if (piece.has_value())
				board.emplace(pos, *piece);
		}

		boardLayer = CreateLayer();
		SetDrawTarget(boardLayer);
		Clear(olc::DARK_GREY);

		FillRect(0, 0, BOARD_SIZE + BORDER_SIZE, BORDER_SIZE, olc::BLACK);
		FillRect(BOARD_SIZE + BORDER_SIZE, 0, BORDER_SIZE, BOARD_SIZE + BORDER_SIZE, olc::BLACK);
		FillRect(BORDER_SIZE, BOARD_SIZE + BORDER_SIZE, BOARD_SIZE + BORDER_SIZE, BORDER_SIZE, olc::BLACK);
		FillRect(0, BORDER_SIZE, BORDER_SIZE, BOARD_SIZE + BORDER_SIZE, olc::BLACK);
		DrawRect(BORDER_SIZE - 1, BORDER_SIZE - 1, BOARD_SIZE + 1, BOARD_SIZE + 1, olc::YELLOW);

		for (uint16_t i = 0, x = 49; i < 8; i++, x += 55) {
			std::string value(1, 56 - i); //56 == char '8'
			DrawString(BORDER_SIZE / 3, x, value, olc::YELLOW);
			DrawString(BORDER_SIZE / 3 + (BOARD_SIZE + BORDER_SIZE), x, value, olc::YELLOW);
			value = 65 + i;//65 == char 'A'
			DrawString(x, BORDER_SIZE / 3, value, olc::YELLOW);
			DrawString(x, BOARD_SIZE + BORDER_SIZE + 9, value, olc::YELLOW);
		}

		std::unique_ptr<olc::Sprite> whiteSquare = std::make_unique<olc::Sprite>("WhiteSquare.png");
		std::unique_ptr<olc::Sprite> blackSquare = std::make_unique<olc::Sprite>("BlackSquare.png");
		for (uint8_t x = 0; x < 8; x++)
			for (uint8_t y = 0; y < 8; y++)
				if ((x % 2 == 0) == (y % 2 == 0))
					DrawSprite(positionToPoint(x), positionToPoint(y), whiteSquare.get());
				else
					DrawSprite(positionToPoint(x), positionToPoint(y), blackSquare.get());

		EnableLayer(boardLayer, true);
		SetDrawTarget(nullptr);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		Clear(olc::BLANK);

		olc::vu2d point;
		if (selected.has_value()) {
			point.x = positionToPoint(selected->getFile());
			point.y = positionToPoint(selected->getRank());
			FillRect(point, CELL_SIZE, olc::YELLOW);
		}

		if (lastMove.has_value()) {
			DrawRect(lastMove->first, CELL_SIZE, olc::YELLOW);
			DrawRect(lastMove->second, CELL_SIZE, olc::YELLOW);
		}

		Position pos;
		for (auto it = board.cbegin(); it != board.cend(); it++) {
			pos.setFile(it->first.getFile());
			pos.setRank(it->first.getRank());
			pos.invert();
			point.x = positionToPoint(pos.getFile());
			point.y = positionToPoint(pos.getRank());
			DrawPartialDecal(point, pieces, { float(it->second.NAME * CELL_SIZE.x + (it->second.COLOR * CELL_SIZE.y * 6)),0 }, CELL_SIZE);
		}

		if (bitBoard.attacks != 0)
			for (uint8_t i = 0; i < 64; i++) {
				if (bitBoardOperations::isSquareAttacked(bitBoard, i)) {
					pos.setSquare(i);
					pos.invert();
					point.x = positionToPoint(pos.getFile());
					point.y = positionToPoint(pos.getRank());
					FillCircle(point.x + 26, point.y + 26, CELL_SIZE.x * 0.2f, olc::YELLOW);
				}
			}

		if (movePiece.has_value()) {
			DrawPartialDecal(movePiece->first, pieces, { float(movePiece->second.NAME * CELL_SIZE.x + (movePiece->second.COLOR * CELL_SIZE.y * 6)),0 }, CELL_SIZE);

			float deltaX = float(lastMove->second.x) - movePiece->first.x;
			float deltaY = float(lastMove->second.y) - movePiece->first.y;
			float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);

			movePiece->first.x += deltaX / distance;
			movePiece->first.y += deltaY / distance;

			if (distance < 5) {
				movePiece.reset();

				pos.setFile(pointToPosition(lastMove->second.x));
				pos.setRank(pointToPosition(lastMove->second.y));
				pos.invert();
				board.erase(board.find(pos));

			}
		} else {
			point.x = GetMouseX();
			point.y = GetMouseY();
			if (isMouseInsideBoard(point)) {
				pos.setFile(pointToPosition(point.x));
				pos.setRank(pointToPosition(point.y));
				point.x = positionToPoint(pos.getFile());
				point.y = positionToPoint(pos.getRank());
				DrawRect(point, CELL_SIZE, olc::YELLOW);

				if (GetMouse(0).bPressed) {
					pos.invert();

					if (!selected.has_value()) {
						auto it = board.find(pos);
						if (it != board.cend()) {
							MoveGenerator moveGenerator;
							moveGenerator.generateValidMoves(bitBoard, it->second.NAME, it->second.COLOR, pos);
							if (bitBoard.attacks != 0) {
								selected.emplace(pos);
								selected->invert();
							}
						}
					} else if (bitBoardOperations::isSquareAttacked(bitBoard, pos.getSquare())) {
						pos.invert();
						lastMove.reset();
						lastMove = std::make_pair<olc::vu2d, olc::vu2d>({ positionToPoint(selected->getFile()), positionToPoint(selected->getRank()) }, { positionToPoint(pos.getFile()), positionToPoint(pos.getRank()) });

						selected->invert();
						auto it = board.find(*selected);
						movePiece.emplace(lastMove->first, it->second);
						board.erase(it);

						selected.reset();
						bitBoard.attacks = 0;
						std::cout << bitBoard;
					}
				} else if (GetMouse(1).bPressed) {
					selected.reset();
					bitBoard.attacks = 0;
				}
			}
		}
		return true;
	}
};

int main() {
	Chess chess;
	if (chess.Construct(580, 500, 1, 1))
		chess.Start();
	return 0;
}