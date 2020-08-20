#pragma once
#include "GameBehavior.h"
#include "BitBoard.h"
#include "BitBoardOperations.h"
#include "MoveGenerator.h"

class BoardGUI : public GameBehavior {
private:
	const olc::vu2d CELL_SIZE = {55, 55};
	const uint32_t BORDER_SIZE = 25;
	const uint32_t BOARD_SIZE = 8 * 55;

	struct Position {
		uint8_t square;
		uint8_t getFile() { return bitBoardOperations::getFileFromSquare(square); }
		uint8_t getRank() { return bitBoardOperations::getRankFromSquare(square); }
		void invert() { square = 8 * (7 - getRank()) + getFile(); }
	} position;

	int boardLayer;

	bool whiteTime, checkMate;

	olc::Decal* pieces;

	std::pair<olc::vu2d, olc::vu2d> lastMove;

	std::optional<uint8_t> selected;
	std::optional<std::pair<olc::vf2d, Piece>> movePiece;

	BitBoard bitBoard;
	std::map<uint8_t, Piece> board;

	//0======================================================================0
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

	inline const bool isColorTime(const Piece::Color& pieceColor) const {
		return (whiteTime && pieceColor == Piece::Color::WHITE) || (!whiteTime && pieceColor == Piece::Color::BLACK);
	}
	//0======================================================================0
public:
	BoardGUI() : GameBehavior(),
		boardLayer(0),
		whiteTime(false),
		checkMate(true),
		pieces(nullptr),
		selected(std::nullopt),
		movePiece(std::nullopt) {}
	~BoardGUI() {
		boardLayer = 0;
		whiteTime = false;
		checkMate = whiteTime;
		if (pieces != nullptr)
			delete pieces;
		pieces = nullptr;
		selected.reset();
		movePiece.reset();
		board.clear();
	}

	void startNewGame(Piece::Color playerColor) {
		bitBoardOperations::setInitialPosition(bitBoard);

		board.clear();
		for (uint8_t i = 0; i < 64; i++) {
			std::optional<Piece> piece = bitBoardOperations::getPieceFromSquare(bitBoard, i);
			if (piece.has_value())
				board.emplace(i, piece.value());
		}

		whiteTime = true;
		checkMate = false;

		lastMove = std::make_pair(olc::vu2d(), olc::vu2d());

		selected = std::nullopt;
		movePiece = std::nullopt;
	}

	bool OnUserCreate(olc::PixelGameEngine& olc) override {
		pieces = new olc::Decal(new olc::Sprite("Images/PiecesSprite.png"));

		boardLayer = olc.CreateLayer();
		olc.SetDrawTarget(boardLayer);
		olc.Clear(olc::DARK_GREY);

		olc.FillRect(0, 0, BOARD_SIZE + BORDER_SIZE, BORDER_SIZE, olc::BLACK);
		olc.FillRect(BOARD_SIZE + BORDER_SIZE, 0, BORDER_SIZE, BOARD_SIZE + BORDER_SIZE, olc::BLACK);
		olc.FillRect(BORDER_SIZE, BOARD_SIZE + BORDER_SIZE, BOARD_SIZE + BORDER_SIZE, BORDER_SIZE, olc::BLACK);
		olc.FillRect(0, BORDER_SIZE, BORDER_SIZE, BOARD_SIZE + BORDER_SIZE, olc::BLACK);
		olc.DrawRect(BORDER_SIZE - 1, BORDER_SIZE - 1, BOARD_SIZE + 1, BOARD_SIZE + 1, olc::YELLOW);

		for (uint16_t i = 0, x = 49; i < 8; i++, x += 55) {
			std::string value(1, 56 - i); //56 == char '8'
			olc.DrawString(BORDER_SIZE / 3, x, value, olc::YELLOW);
			olc.DrawString(BORDER_SIZE / 3 + (BOARD_SIZE + BORDER_SIZE), x, value, olc::YELLOW);
			value = 65 + i;//65 == char 'A'
			olc.DrawString(x, BORDER_SIZE / 3, value, olc::YELLOW);
			olc.DrawString(x, BOARD_SIZE + BORDER_SIZE + 9, value, olc::YELLOW);
		}

		std::unique_ptr<olc::Sprite> whiteSquare(new olc::Sprite("Images/WhiteSquare.png"));
		std::unique_ptr<olc::Sprite> blackSquare(new olc::Sprite("Images/BlackSquare.png"));
		for (uint8_t x = 0; x < 8; x++)
			for (uint8_t y = 0; y < 8; y++)
				if ((x % 2 == 0) == (y % 2 == 0))
					olc.DrawSprite(positionToPoint(x), positionToPoint(y), whiteSquare.get());
				else
					olc.DrawSprite(positionToPoint(x), positionToPoint(y), blackSquare.get());

		olc.EnableLayer(boardLayer, true);
		olc.SetDrawTarget(nullptr);

		return true;
	}

	bool OnUserUpdate(olc::PixelGameEngine& olc, float elapsedTime) override {
		olc::vu2d point;
		if (selected.has_value()) {
			position.square = *selected;
			point.x = positionToPoint(position.getFile());
			point.y = positionToPoint(position.getRank());
			olc.FillRect(point, CELL_SIZE, olc::YELLOW);
		}

		if (lastMove.first.x != lastMove.second.x && lastMove.first.y != lastMove.second.y) {
			olc.DrawRect(lastMove.first, CELL_SIZE, olc::YELLOW);
			olc.DrawRect(lastMove.second, CELL_SIZE, olc::YELLOW);
		}

		for (auto it = board.cbegin(); it != board.cend(); ++it) {
			position.square = it->first;
			position.invert();
			point.x = positionToPoint(position.getFile());
			point.y = positionToPoint(position.getRank());
			olc::vu2d aux(((uint8_t)it->second.NAME * CELL_SIZE.x) + ((uint8_t)it->second.COLOR * CELL_SIZE.y * 6), 0);
			olc.DrawPartialDecal(point, CELL_SIZE, pieces, aux, CELL_SIZE);
		}

		for (uint64_t attacks = bitBoard.attacks; attacks != 0; attacks = bitBoardOperations::remainder(attacks)) {
			position.square = bitBoardOperations::getSquareFromBitmap(attacks);
			position.invert();
			point.x = positionToPoint(position.getFile());
			point.y = positionToPoint(position.getRank());
			olc.FillCircle(point.x + 26, point.y + 26, CELL_SIZE.x / 5, olc::YELLOW);
		}

		if (movePiece.has_value()) {
			point.x = ((uint8_t)movePiece->second.NAME * CELL_SIZE.x) + ((uint8_t)movePiece->second.COLOR * CELL_SIZE.y * 6);
			point.y = 0;
			olc.DrawPartialDecal(movePiece->first, CELL_SIZE, pieces, point, CELL_SIZE);

			float deltaX = float(lastMove.second.x) - movePiece->first.x;
			float deltaY = float(lastMove.second.y) - movePiece->first.y;
			float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);

			movePiece->first.x += deltaX / distance;
			movePiece->first.y += deltaY / distance;

			if (distance < 5) {
				point.x = pointToPosition(lastMove.second.x);
				point.y = pointToPosition(lastMove.second.y);
				position.square = bitBoardOperations::getSquareFromFileRank(point.x, point.y);
				position.invert();

				if (board.count(position.square) > 0)
					board.erase(position.square);
				board.emplace(position.square, movePiece->second);

				movePiece.reset();

				std::cout << bitBoard;
			}
		}
		else {
			point.x = olc.GetMouseX();
			point.y = olc.GetMouseY();
			if (isMouseInsideBoard(point)) {
				point.x = pointToPosition(point.x);
				point.y = pointToPosition(point.y);
				position.square = bitBoardOperations::getSquareFromFileRank(point.x, point.y);
				point.x = positionToPoint(point.x);
				point.y = positionToPoint(point.y);
				olc.DrawRect(point, CELL_SIZE, olc::YELLOW);

				if (olc.GetMouse(0).bPressed) {
					position.invert();
					if (!selected.has_value()) {
						auto it = board.find(position.square);
						if (it != board.cend()) {
							MoveGenerator moveGenerator;
							if (moveGenerator.hasPossibleMoves(bitBoard, (uint8_t)it->second.NAME, (uint8_t)it->second.COLOR, position.square)) {
								position.invert();
								selected.emplace(position.square);
							}
						}
					}
					else if (bitBoardOperations::isSquareAttacked(bitBoard, position.square)) {
						position.invert();
						uint8_t square = position.square;
						position.square = *selected;
						lastMove.first.x = positionToPoint(position.getFile());
						lastMove.first.y = positionToPoint(position.getRank());
						position.square = square;
						lastMove.second.x = positionToPoint(position.getFile());
						lastMove.second.y = positionToPoint(position.getRank());

						position.square = *selected;
						position.invert();
						auto it = board.find(position.square);
						movePiece.emplace(lastMove.first, it->second);
						board.erase(it);

						position.square = square;
						position.invert();
						MoveMaker moveMaker(*selected, position.square);
						moveMaker.makeMove(bitBoard);

						selected.reset();
						bitBoard.attacks = 0;
					}
				}
				else if (olc.GetMouse(1).bPressed) {
					selected.reset();
					bitBoard.attacks = 0;
				}
			}
		}
		return true;
	}
};