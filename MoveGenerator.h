#pragma once
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "King.h"
#include "MoveMaker.h"

class MoveGenerator {
public:

	const bool hasPossibleMoves(BitBoard& bitBoard, const uint8_t namePiece, const uint8_t colorPiece, const uint8_t& square) {
		bitBoard.attacks = getValidMoves(bitBoard, namePiece, colorPiece, square);
		return bitBoard.attacks != 0;
	}

	const bool hasPossibleMoves(BitBoard bitBoard, const uint8_t& color) {
		generateAttacks(bitBoard, color, true);
		return bitBoard.attacks != 0;
	}

	//gera um bitmap das casas alcançadas pela color
	const void generateAttacks(BitBoard& bitBoard, const uint8_t& color, const bool& validMoves) const {
		uint64_t attacks = 0;
		uint8_t square;
		for (uint8_t namePiece = 0; namePiece < 6; namePiece++) {
			for (uint64_t pieceBitmap = bitBoard.bitMaps[namePiece][color]; pieceBitmap != 0; pieceBitmap = bitBoardOperations::remainder(pieceBitmap)) {
				square = bitBoardOperations::getSquareFromBitmap(pieceBitmap);
				if (validMoves)
					attacks = bitBoardOperations::getUnion(attacks, getValidMoves(bitBoard, namePiece, color, square));
				else
					attacks = bitBoardOperations::getUnion(attacks, getMoves(bitBoard, namePiece, color, square));
			}
		}
		bitBoard.attacks = attacks;
	}

	//gera um bitmap das casas alcançadas pela peça sem colocar o rei da mesma cor em xeque
	const void generateValidMoves(BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorPiece, const uint8_t& square) const {
		bitBoard.attacks = getValidMoves(bitBoard, namePiece, colorPiece, square);
	}

private:
	//retorna um bitmap das casas alcançadas pela peça sem colocar o rei da mesma cor em xeque
	const uint64_t getValidMoves(BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorPiece, const uint8_t& square) const {
		uint64_t attacks = 0;
		attacks = getMoves(bitBoard, namePiece, colorPiece, square);
		tryToMakeMove(bitBoard, namePiece, colorPiece, square, attacks);
		tryToMakeCastle(bitBoard, namePiece, colorPiece, attacks);
		return attacks;
	}

	//retorna um bitmap das casas alcançadas pela pela peça
	const uint64_t getMoves(const BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorPiece, const uint8_t& square) const {
		uint64_t attacks = 0;
		switch (namePiece) {
			case Piece::Name::PAWN:
				attacks = pawn::getMoves(bitBoard, colorPiece, square);
				break;
			case Piece::Name::KNIGHT:
				attacks = knight::getMoves(bitBoard, square);
				break;
			case Piece::Name::BISHOP:
				attacks = bishop::getMoves(bitBoard, square);
				break;
			case Piece::Name::ROOK:
				attacks = rook::getMoves(bitBoard, square);
				break;
			case Piece::Name::QUEEN:
				attacks = bishop::getMoves(bitBoard, square) | rook::getMoves(bitBoard, square);
				break;
			case Piece::Name::KING:
				attacks = king::getMoves(bitBoard, square);
		}
		return bitBoardOperations::unsetIntersections(attacks, bitBoardOperations::allPiecesColor(bitBoard, (Piece::Color)colorPiece));
	}

	//tenta mover a peça sem colocar o rei em xeque
	void tryToMakeMove(const BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorPiece, const uint8_t& origin, uint64_t& attacks) const {
		uint8_t destiny;

		for (uint64_t destinations = attacks; destinations != 0; destinations = bitBoardOperations::remainder(destinations)) {
			destiny = bitBoardOperations::getSquareFromBitmap(destinations);

			BitBoard clone(bitBoard);
			MoveMaker moveMaker(origin, destiny);
			moveMaker.makeMove(clone);

			generateAttacks(clone, otherColor(colorPiece), false);
			if (bitBoardOperations::isKingCheck(clone, (Piece::Color)colorPiece))
				attacks = bitBoardOperations::unsetIntersections(attacks, bitBoardOperations::getBitmapFromSquare(destiny));
		}
	}

	//tenta fazer o movimento roque
	void tryToMakeCastle(BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorPiece, uint64_t& attacks) const {
		if (namePiece == (uint8_t)Piece::Name::KING) {
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
					return !bitBoardOperations::isKingCheck(bitBoard, (Piece::Color)color);
		return false;
	}

	//este metodo verifica se o rei pode fazer o castelo
	const bool canMakeBigCastle(const BitBoard& bitBoard, const uint8_t& color) const {
		if (!king::wasKingMoved(bitBoard, color))
			if (!rook::wasBigRookMoved(bitBoard, color))
				if (king::isPathBigRookClear(bitBoard, color))
					return !bitBoardOperations::isKingCheck(bitBoard, (Piece::Color)color);
		return false;
	}

	uint8_t otherColor(const uint8_t& color) const {
		if (color == 1)
			return 0;
		return 1;
	}
};