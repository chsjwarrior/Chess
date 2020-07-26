#pragma once
#include "GameBehavior.h"
#include "Position.h"
#include "BitBoard.h"
#include "BitBoardOperations.h"
#include "MoveGenerator.h"

class Board : public GameBehavior {
private:
	const olc::vu2d CELL_SIZE = {55, 55};
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

	Position pos;

	//0======================================================================0
	inline const bool isMouseInsideBoard(const olc::vu2d& point) const {
		return point.x >= BORDER_SIZE && point.x < BOARD_SIZE + BORDER_SIZE &&
			point.y >= BORDER_SIZE && point.y < BOARD_SIZE + BORDER_SIZE;
	}

	inline const uint8_t pointToPosition(const uint32_t& value) const {
		return (value - BORDER_SIZE) / CELL_SIZE.x;
	}

	inline const olc::vu2d positionToPoint(const Position& pos) const {
		return {positionToPoint(pos.getFile()), positionToPoint(pos.getRank())};
	}

	inline const uint32_t positionToPoint(const uint8_t& value) const {
		return value * CELL_SIZE.x + BORDER_SIZE;
	}

	inline const bool isColorTime(const Piece::COLOR& pieceColor) const {
		return (whiteTime && pieceColor == Piece::COLOR::WHITE) || (!whiteTime && pieceColor == Piece::COLOR::BLACK);
	}
	//0======================================================================0
public:
	Board() : GameBehavior(),
		boardLayer(0),
		whiteTime(false),
		checkMate(true),
		pieces(nullptr),
		selected(std::nullopt),
		lastMove(std::nullopt),
		movePiece(std::nullopt) {}
	~Board() {
		boardLayer = 0;
		whiteTime = false;
		checkMate = whiteTime;
		if (pieces != nullptr)
			delete pieces;
		pieces = nullptr;
		selected.reset();
		lastMove.reset();
		movePiece.reset();
		board.clear();
	}

	void startNewGame(Piece::COLOR playerColor) {
		bitBoardOperations::setInitialPosition(bitBoard);

		board.clear();
		for (uint8_t i = 0; i < 64; i++) {
			auto piece = bitBoardOperations::getPieceFromSquare(bitBoard, i);
			if (piece.has_value())
				board.emplace(i, piece.value());
		}

		whiteTime = true;
		checkMate = false;

		selected = std::nullopt;
		lastMove = std::nullopt;
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
			point.x = positionToPoint(selected->getFile());
			point.y = positionToPoint(selected->getRank());
			olc.FillRect(point, CELL_SIZE, olc::YELLOW);
		}

		if (lastMove.has_value()) {
			olc.DrawRect(lastMove->first, CELL_SIZE, olc::YELLOW);
			olc.DrawRect(lastMove->second, CELL_SIZE, olc::YELLOW);
		}

		//Position pos;
		for (auto it = board.cbegin(); it != board.cend(); ++it) {
			pos.setFile(it->first.getFile());
			pos.setRank(it->first.getRank());
			pos.invert();
			point.x = positionToPoint(pos.getFile());
			point.y = positionToPoint(pos.getRank());
			olc.DrawPartialDecal(point, CELL_SIZE, pieces, {float(((uint8_t) it->second.name * CELL_SIZE.x) + ((uint8_t) it->second.color * CELL_SIZE.y * 6)) ,0}, CELL_SIZE);
		}

		for (uint64_t attacks = bitBoard.attacks; attacks != 0; attacks = bitBoardOperations::remainder(attacks)) {
			pos.setSquare(bitBoardOperations::getSquareFromBitmap(attacks));
			pos.invert();
			point.x = positionToPoint(pos.getFile());
			point.y = positionToPoint(pos.getRank());
			olc.FillCircle(point.x + 26, point.y + 26, CELL_SIZE.x / 5, olc::YELLOW);
		}

		if (movePiece.has_value()) {
			olc.DrawPartialDecal(movePiece->first, CELL_SIZE, pieces, {float(((uint8_t) movePiece->second.name * CELL_SIZE.x) + ((uint8_t) movePiece->second.color * CELL_SIZE.y * 6)), 0}, CELL_SIZE);

			float deltaX = float(lastMove->second.x) - movePiece->first.x;
			float deltaY = float(lastMove->second.y) - movePiece->first.y;
			float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);

			movePiece->first.x += deltaX / distance;
			movePiece->first.y += deltaY / distance;

			if (distance < 5) {
				pos.setFile(pointToPosition(lastMove->second.x));
				pos.setRank(pointToPosition(lastMove->second.y));
				pos.invert();

				if (board.count(pos) > 0)
					board.erase(pos);
				board.emplace(pos, movePiece->second);

				movePiece.reset();

				std::cout << bitBoard;
			}
		} else {
			point.x = olc.GetMouseX();
			point.y = olc.GetMouseY();
			if (isMouseInsideBoard(point)) {
				pos.setFile(pointToPosition(point.x));
				pos.setRank(pointToPosition(point.y));
				point.x = positionToPoint(pos.getFile());
				point.y = positionToPoint(pos.getRank());
				olc.DrawRect(point, CELL_SIZE, olc::YELLOW);

				if (olc.GetMouse(0).bPressed) {
					pos.invert();
					if (!selected.has_value()) {
						auto it = board.find(pos);
						if (it != board.cend()) {
							MoveGenerator moveGenerator;
							if (moveGenerator.hasPossibleMoves(bitBoard, (uint8_t) it->second.name, (uint8_t) it->second.color, pos)) {
								selected.emplace(pos);
								selected->invert();
							}
						}
					} else if (bitBoardOperations::isSquareAttacked(bitBoard, pos.getSquare())) {
						pos.invert();
						lastMove.reset();
						lastMove = std::pair<olc::vu2d, olc::vu2d>(positionToPoint(*selected), positionToPoint(pos));

						selected->invert();
						auto it = board.find(*selected);
						movePiece.emplace(lastMove->first, it->second);
						board.erase(it);

						pos.invert();
						MoveMaker moveMaker(*selected, pos);
						moveMaker.makeMove(bitBoard);

						selected.reset();
						bitBoard.attacks = 0;
					}
				} else if (olc.GetMouse(1).bPressed) {
					selected.reset();
					bitBoard.attacks = 0;
				}
			}
		}
		return true;
	}
};