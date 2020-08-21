#pragma once
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "King.h"
#include "MoveMaker.h"

class MoveGenerator {
public:
	const bool hasPossibleMoves(BitBoard& bitBoard, const uChar namePiece, const uChar colorPiece, const uChar& square) const {
		generateValidMoves(bitBoard, namePiece, colorPiece, square);
		return bitBoard.attacks != 0;
	}

	const bool hasPossibleMoves(BitBoard bitBoard, const uChar& color) const {
		generateAttacks(bitBoard, color, true);
		return bitBoard.attacks != 0;
	}

	//gera um bitmap das casas alcançadas pela color
	const void generateAttacks(BitBoard& bitBoard, const uChar& color, const bool& validMoves) const {
		Tbitmap attacks = 0;
		uChar square;
		for (uChar namePiece = 0; namePiece < 6; namePiece++) {
			for (Tbitmap pieceBitmap = bitBoard.bitMaps[namePiece][color]; pieceBitmap != 0; pieceBitmap = bitBoardOperations::remainder(pieceBitmap)) {
				square = bitBoardOperations::getSquareFromBitmap(pieceBitmap);
				if (validMoves)
					generateValidMoves(bitBoard, namePiece, color, square);
				else
					generateMoves(bitBoard, namePiece, color, square);
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.attacks);
			}
		}
		bitBoard.attacks = attacks;
	}

	//gera um bitmap das casas alcançadas pela peça sem colocar o rei da mesma cor em xeque
	const void generateValidMoves(BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorPiece, const uChar& square) const {
		generateMoves(bitBoard, namePiece, colorPiece, square);
		tryToMakeMove(bitBoard, namePiece, colorPiece, square);
		tryToMakeCastle(bitBoard, namePiece, colorPiece);
	}

private:
	//retorna um bitmap das casas alcançadas pela pela peça
	void generateMoves(BitBoard& bitBoard, const uChar& namePiece, const uChar& colorPiece, const uChar& square) const {
		switch (namePiece) {
			case PAWN:
				bitBoard.attacks = pawn::getMoves(bitBoard, colorPiece, square);
				break;
			case KNIGHT:
				bitBoard.attacks = knight::getMoves(square);
				break;
			case BISHOP:
				bitBoard.attacks = bishop::getMoves(bitBoardOperations::allPieces(bitBoard), square);
				break;
			case ROOK:
				bitBoard.attacks = rook::getMoves(bitBoardOperations::allPieces(bitBoard), square);
				break;
			case QUEEN:
				bitBoard.attacks = bishop::getMoves(bitBoardOperations::allPieces(bitBoard), square) | rook::getMoves(bitBoardOperations::allPieces(bitBoard), square);
				break;
			case KING:
				bitBoard.attacks = king::getMoves(square);
		}
		bitBoard.attacks = bitBoardOperations::unsetIntersections(bitBoard.attacks, bitBoardOperations::allPiecesColor(bitBoard, colorPiece));
	}

	//tenta mover a peça sem colocar o rei em xeque
	void tryToMakeMove(BitBoard& bitBoard, const uChar& namePiece, const uChar& colorPiece, const uChar& origin) const {
		uChar destiny;

		for (Tbitmap destinations = bitBoard.attacks; destinations != 0; destinations = bitBoardOperations::remainder(destinations)) {
			destiny = bitBoardOperations::getSquareFromBitmap(destinations);

			BitBoard clone(bitBoard);
			MoveMaker moveMaker(origin, destiny);
			moveMaker.makeMove(clone);

			generateAttacks(clone, bitBoardOperations::getOtherColor(colorPiece), false);
			if (bitBoardOperations::isKingCheck(clone, colorPiece))
				bitBoard.attacks = bitBoardOperations::unsetIntersections(bitBoard.attacks, bitBoardOperations::getBitmapFromSquare(destiny));
		}
	}

	//tenta fazer o movimento roque
	void tryToMakeCastle(BitBoard& bitBoard, const uChar& namePiece, const uChar& colorPiece) const {
		if (namePiece == KING) {
			Tbitmap attacks = bitBoard.attacks;
			generateAttacks(bitBoard, bitBoardOperations::getOtherColor(colorPiece), false);
			if (canMakeSmallCastle(bitBoard, colorPiece))
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.bitMaps[namePiece][colorPiece] << 2);
			if (canMakeBigCastle(bitBoard, colorPiece))
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.bitMaps[namePiece][colorPiece] >> 2);
			bitBoard.attacks = attacks;
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