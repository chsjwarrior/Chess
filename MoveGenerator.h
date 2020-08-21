#pragma once
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "King.h"
#include "MoveMaker.h"

class MoveGenerator {
public:

	const bool hasPossibleMoves(BitBoard& bitBoard, const uChar namePiece, const uChar colorPiece, const uChar& square) {
		bitBoard.attacks = getValidMoves(bitBoard, namePiece, colorPiece, square);
		return bitBoard.attacks != 0;
	}

	const bool hasPossibleMoves(BitBoard bitBoard, const uChar& color) {
		generateAttacks(bitBoard, color, true);
		return bitBoard.attacks != 0;
	}

	//gera um bitmap das casas alcançadas pela color
	const void generateAttacks(BitBoard& bitBoard, const uChar& color, const bool& validMoves) const {
		Tbitmap attacks = 0;
		uChar square;
		for (uint8_t namePiece = 0; namePiece < 6; namePiece++) {
			for (Tbitmap pieceBitmap = bitBoard.bitMaps[namePiece][color]; pieceBitmap != 0; pieceBitmap = bitBoardOperations::remainder(pieceBitmap)) {
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
	const void generateValidMoves(BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorPiece, const uChar& square) const {
		bitBoard.attacks = getValidMoves(bitBoard, namePiece, colorPiece, square);
	}

private:
	//retorna um bitmap das casas alcançadas pela peça sem colocar o rei da mesma cor em xeque
	const Tbitmap getValidMoves(BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorPiece, const uChar& square) const {
		Tbitmap attacks = 0;
		attacks = getMoves(bitBoard, namePiece, colorPiece, square);
		tryToMakeMove(bitBoard, namePiece, colorPiece, square, attacks);
		tryToMakeCastle(bitBoard, namePiece, colorPiece, attacks);
		return attacks;
	}

	//retorna um bitmap das casas alcançadas pela pela peça
	const Tbitmap getMoves(const BitBoard& bitBoard, const uChar& namePiece, const uChar& colorPiece, const uChar& square) const {
		uint64_t attacks = 0;
		switch (namePiece) {
			case PAWN:
				attacks = pawn::getMoves(bitBoard, colorPiece, square);
				break;
			case KNIGHT:
				attacks = knight::getMoves(bitBoard, square);
				break;
			case BISHOP:
				attacks = bishop::getMoves(bitBoard, square);
				break;
			case ROOK:
				attacks = rook::getMoves(bitBoard, square);
				break;
			case QUEEN:
				attacks = bishop::getMoves(bitBoard, square) | rook::getMoves(bitBoard, square);
				break;
			case KING:
				attacks = king::getMoves(bitBoard, square);
		}
		return bitBoardOperations::unsetIntersections(attacks, bitBoardOperations::allPiecesColor(bitBoard, colorPiece));
	}

	//tenta mover a peça sem colocar o rei em xeque
	void tryToMakeMove(const BitBoard& bitBoard, const uChar& namePiece, const uChar& colorPiece, const uChar& origin, Tbitmap& attacks) const {
		uChar destiny;

		for (Tbitmap destinations = attacks; destinations != 0; destinations = bitBoardOperations::remainder(destinations)) {
			destiny = bitBoardOperations::getSquareFromBitmap(destinations);

			BitBoard clone(bitBoard);
			MoveMaker moveMaker(origin, destiny);
			moveMaker.makeMove(clone);

			generateAttacks(clone, bitBoardOperations::getOtherColor(colorPiece), false);
			if (bitBoardOperations::isKingCheck(clone, colorPiece))
				attacks = bitBoardOperations::unsetIntersections(attacks, bitBoardOperations::getBitmapFromSquare(destiny));
		}
	}

	//tenta fazer o movimento roque
	void tryToMakeCastle(BitBoard& bitBoard, const uChar& namePiece, const uChar& colorPiece, Tbitmap& attacks) const {
		if (namePiece == KING) {
			generateAttacks(bitBoard, bitBoardOperations::getOtherColor(colorPiece), false);
			if (canMakeSmallCastle(bitBoard, colorPiece))
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.bitMaps[namePiece][colorPiece] << 2);
			if (canMakeBigCastle(bitBoard, colorPiece))
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.bitMaps[namePiece][colorPiece] >> 2);
			bitBoard.attacks = 0;
		}
	}

	//este metodo verifica se o rei pode fazer o castelo
	const bool canMakeSmallCastle(const BitBoard& bitBoard, const uChar& color) const {
		if (!king::wasKingMoved(bitBoard, color))
			if (!rook::wasSmallRookMoved(bitBoard, color))
				if (king::isPathSmallRookClear(bitBoard, color))
					return !bitBoardOperations::isKingCheck(bitBoard, color);
		return false;
	}

	//este metodo verifica se o rei pode fazer o castelo
	const bool canMakeBigCastle(const BitBoard& bitBoard, const uChar& color) const {
		if (!king::wasKingMoved(bitBoard, color))
			if (!rook::wasBigRookMoved(bitBoard, color))
				if (king::isPathBigRookClear(bitBoard, color))
					return !bitBoardOperations::isKingCheck(bitBoard, color);
		return false;
	}	
};