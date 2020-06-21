#ifndef MOVEGENERATOR_H_INCLUDED
#define MOVEGENERATOR_H_INCLUDED
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "King.h"
#include "MoveMaker.h"

class MoveGenerator {
public:

	const bool hasPossibleMoves(BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorPiece, Position& position) {
		bitBoard.attacks = getValidMoves(bitBoard, namePiece, colorPiece, position);
		return bitBoard.attacks != 0;
	}

	const bool hasPossibleMoves(BitBoard bitBoard, const uint8_t& color) {
		generateAttacks(bitBoard, color, true);
		return bitBoard.attacks != 0;
	}

	//gera um bitmap das casas alcançadas pela color
	const void generateAttacks(BitBoard& bitBoard, const uint8_t& color, const bool& validMoves) const {
		uint64_t attacks = 0;
		Position pos;
		for (uint8_t namePiece = 0; namePiece < bitBoard.bitMapsLength; namePiece++) {
			for (uint64_t pieceBitmap = bitBoard.bitMaps[namePiece][color]; pieceBitmap != 0; pieceBitmap = bitBoardOperations::remainder(pieceBitmap)) {
				pos.setSquare(bitBoardOperations::getSquareFromBitmap(pieceBitmap));
				if (validMoves)
					attacks = bitBoardOperations::getUnion(attacks, getValidMoves(bitBoard, namePiece, color, pos));
				else
					attacks = bitBoardOperations::getUnion(attacks, getMoves(bitBoard, namePiece, color, pos));
			}
		}
		bitBoard.attacks = attacks;
	}

	//gera um bitmap das casas alcançadas pela peça sem colocar o rei da mesma cor em xeque
	const void generateValidMoves(BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorPiece, const Position& position) const {
		bitBoard.attacks = getValidMoves(bitBoard, namePiece, colorPiece, position);
	}

private:
	//retorna um bitmap das casas alcançadas pela peça sem colocar o rei da mesma cor em xeque
	const uint64_t getValidMoves(BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorPiece, const Position& position) const {
		uint64_t attacks = 0;
		attacks = getMoves(bitBoard, namePiece, colorPiece, position);
		tryToMakeMove(bitBoard, namePiece, colorPiece, position, attacks);
		tryToMakeCastle(bitBoard, namePiece, colorPiece, attacks);
		return attacks;
	}

	//retorna um bitmap das casas alcançadas pela pela peça
	const uint64_t getMoves(const BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorPiece, const Position& position) const {
		uint64_t attacks = 0;
		switch (namePiece) {
			case Piece::PAWN:
				attacks = pawn::getMoves(bitBoard, colorPiece, position);
				break;
			case Piece::KNIGHT:
				attacks = knight::getMoves(bitBoard, position);
				break;
			case Piece::BISHOP:
				attacks = bishop::getMoves(bitBoard, position);
				break;
			case Piece::ROOK:
				attacks = rook::getMoves(bitBoard, position);
				break;
			case Piece::QUEEN:
				attacks = bishop::getMoves(bitBoard, position) | rook::getMoves(bitBoard, position);
				break;
			case Piece::KING:
				attacks = king::getMoves(bitBoard, position);
				break;
		}
		return bitBoardOperations::unsetIntersections(attacks, bitBoard.allPiecesColor(colorPiece));
	}

	//tenta mover a peça sem colocar o rei em xeque
	void tryToMakeMove(const BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorPiece, const Position& origin, uint64_t& attacks) const {
		Position destiny;

		for (uint64_t destinations = attacks; destinations != 0; destinations = bitBoardOperations::remainder(destinations)) {
			destiny.setSquare(bitBoardOperations::getSquareFromBitmap(destinations));

			BitBoard clone(bitBoard);
			MoveMaker moveMaker(origin, destiny);
			moveMaker.makeMove(clone);

			generateAttacks(clone, otherColor(colorPiece), false);
			if (bitBoardOperations::isKingCheck(clone, colorPiece))
				attacks = bitBoardOperations::unsetIntersections(attacks, bitBoardOperations::getBitmapFromSquare(destiny.getSquare()));
		}
	}

	//tenta fazer o movimento roque
	void tryToMakeCastle(BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorPiece, uint64_t& attacks) const {
		if (namePiece == Piece::KING) {
			generateAttacks(bitBoard, otherColor(colorPiece), false);
			if (canMakeSmallCastle(bitBoard, colorPiece))
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.bitMaps[namePiece][colorPiece] << 2);
			if (canMakeBigCastle(bitBoard, colorPiece))
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.bitMaps[namePiece][colorPiece] >> 2);
			bitBoard.attacks = 0;
		}
	}

	//este metodo verifica se o rei pode fazer o castelo
	const bool canMakeSmallCastle(const BitBoard& bitBoard, const uint8_t& color) const {
		if (!king::wasKingMoved(bitBoard, color))
			if (!rook::wasSmallRookMoved(bitBoard, color))
				if (king::isPathSmallRookClear(bitBoard, color))
					return !bitBoardOperations::isKingCheck(bitBoard, color);
		return false;
	}

	//este metodo verifica se o rei pode fazer o castelo
	const bool canMakeBigCastle(const BitBoard& bitBoard, const uint8_t& color) const {
		if (!king::wasKingMoved(bitBoard, color))
			if (!rook::wasBigRookMoved(bitBoard, color))
				if (king::isPathBigRookClear(bitBoard, color))
					return !bitBoardOperations::isKingCheck(bitBoard, color);
		return false;
	}

	Piece::COLOR otherColor(const uint8_t& color) const {
		if (color == Piece::BLACK)
			return Piece::WHITE;
		return Piece::BLACK;
	}
};
#endif // MOVEGENERATOR_H_INCLUDED