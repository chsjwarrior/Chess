#pragma once
#include "GameBehavior.h"
#include "BitBoardOperations.h"
#include "MoveGenerator.h"

class BoardGUI : public GameBehavior {
private:
	const olc::vu2d CELL_SIZE = {55, 55};
	const uint32_t BORDER_SIZE = 25;
	const uint32_t BOARD_SIZE = 8 * 55;

	int boardLayer;

	olc::Decal* pieces;

	struct SPiece {
		Piece piece;
		const PieceType type() const { return bitBoardOperations::getPieceTypeOfPiece(piece); }
		const Color color() const { return bitBoardOperations::getColorOfPiece(piece); }
	} piece;

	struct Position {
		Square square;
		const File getFile() const { return bitBoardOperations::getFileOfSquare(square); }
		const Rank getRank() const { return bitBoardOperations::getRankOfSquare(square); }
	} position;

	BitBoard bitBoard;

	Square selected;

	std::pair<olc::vu2d, olc::vu2d> lastMove;

	std::pair<olc::vf2d, Piece> movePiece;

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

	inline const bool isColorTime(const uChar& pieceColor) const {
		return (bitBoard.whiteTime && pieceColor == WHITE) || (!bitBoard.whiteTime && pieceColor == BLACK);
	}
	//0======================================================================0
public:
	BoardGUI() : GameBehavior(), boardLayer(NULL), pieces(nullptr), selected(NONE_SQUARE) {
		piece.piece = NONE_PIECE;
		position.square = NONE_SQUARE;
	}
	~BoardGUI() {
		if (pieces != nullptr)
			delete pieces;
		pieces = nullptr;
	}

	void startNewGame(uChar playerColor) {
		bitBoardOperations::setInitialPosition(bitBoard);

		selected = NONE_SQUARE;

		lastMove.first = olc::vu2d();
		lastMove.second = olc::vu2d();
		movePiece.first = olc::vu2d();
		movePiece.second = NONE_PIECE;
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
		if (selected != NONE_SQUARE) {
			position.square = selected;
			point.x = positionToPoint(position.getFile());
			point.y = positionToPoint(position.getRank());
			olc.FillRect(point, CELL_SIZE, olc::YELLOW);
		}

		if (lastMove.first.x != lastMove.second.x && lastMove.first.y != lastMove.second.y) {
			olc.DrawRect(lastMove.first, CELL_SIZE, olc::YELLOW);
			olc.DrawRect(lastMove.second, CELL_SIZE, olc::YELLOW);
		}

		for (uChar namePiece = 0; namePiece < 6; namePiece++)
			for (uChar color = 0; color < 2; color++)
				for (Bitmap bitmap = bitBoard.bitMaps[namePiece][color]; bitmap != 0; bitmap = bitBoardOperations::remainder(bitmap)) {
					position.square = bitBoardOperations::getSquareOfBitmap(bitmap);
					position.square = ~position.square;
					point.x = positionToPoint(position.getFile());
					point.y = positionToPoint(position.getRank());
					piece.piece = bitBoardOperations::makePiece(static_cast<PieceType>(namePiece), static_cast<Color>(color));
					olc::vu2d aux((piece.type() * CELL_SIZE.x) + (piece.color() * CELL_SIZE.y * 6), 0);
					olc.DrawPartialDecal(point, CELL_SIZE, pieces, aux, CELL_SIZE);
				}

		for (Bitmap attacks = bitBoard.attacks; attacks != 0; attacks = bitBoardOperations::remainder(attacks)) {
			position.square = bitBoardOperations::getSquareOfBitmap(attacks);
			position.square = ~position.square;
			point.x = positionToPoint(position.getFile());
			point.y = positionToPoint(position.getRank());
			olc.FillCircle(point.x + 26, point.y + 26, CELL_SIZE.x / 5, olc::YELLOW);
		}

		if (movePiece.second != NONE_PIECE) {
			piece.piece = movePiece.second;
			point.x = (piece.type() * CELL_SIZE.x) + (piece.color() * CELL_SIZE.y * 6);
			point.y = 0;
			olc.DrawPartialDecal(movePiece.first, CELL_SIZE, pieces, point, CELL_SIZE);

			float deltaX = float(lastMove.second.x) - movePiece.first.x;
			float deltaY = float(lastMove.second.y) - movePiece.first.y;
			float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);

			movePiece.first.x += deltaX / distance;
			movePiece.first.y += deltaY / distance;

			if (distance < 5) {
				movePiece.second = NONE_PIECE;

				bitBoardOperations::printBitmap("BitBoard:", bitBoardOperations::allPieces(bitBoard));
				bitBoardOperations::printBitmap("Flags:", bitBoard.flags);
				bitBoardOperations::printBitmap("Attacks:", bitBoard.attacks);
			}
		}
		else {
			point.x = olc.GetMouseX();
			point.y = olc.GetMouseY();
			if (isMouseInsideBoard(point)) {
				point.x = pointToPosition(point.x);
				point.y = pointToPosition(point.y);
				position.square = bitBoardOperations::getSquareOfFileRank(static_cast<File>(point.x), static_cast<Rank>(point.y));
				point.x = positionToPoint(point.x);
				point.y = positionToPoint(point.y);
				olc.DrawRect(point, CELL_SIZE, olc::YELLOW);

				if (olc.GetMouse(0).bPressed) {
					if (selected == NONE_SQUARE) {
						piece.piece = bitBoardOperations::getPieceFromSquare(bitBoard, ~position.square);
						if (piece.piece != NONE_PIECE) {
							MoveGenerator moveGenerator;
							if (moveGenerator.hasPossibleMoves(bitBoard, piece.type(), piece.color(), ~position.square))
								selected = position.square;
						}
					}
					else if (bitBoardOperations::isSquareAttacked(bitBoard, ~position.square)) {
						lastMove.first.x = positionToPoint(bitBoardOperations::getFileOfSquare(selected));
						lastMove.first.y = positionToPoint(bitBoardOperations::getRankOfSquare(selected));
						//position.square = ~position.square;
						lastMove.second.x = positionToPoint(position.getFile());
						lastMove.second.y = positionToPoint(position.getRank());

						movePiece.first = lastMove.first;
						movePiece.second = bitBoardOperations::getPieceFromSquare(bitBoard, ~selected);

						MoveMaker moveMaker(~selected, ~position.square);
						moveMaker.makeMove(bitBoard);

						selected = NONE_SQUARE;
						bitBoard.attacks = 0;
					}
				}
				else if (olc.GetMouse(1).bPressed) {
					selected = NONE_SQUARE;
					bitBoard.attacks = 0;
				}
			}
		}
		return true;
	}
};