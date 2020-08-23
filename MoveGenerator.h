#pragma once
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "King.h"
#include "MoveMaker.h"

class MoveGenerator {
public:
	const bool hasPossibleMoves(BitBoard& bitBoard, const PieceType pieceType, const Color color, const Square& square) const {
		generateValidMoves(bitBoard, pieceType, color, square);
		return bitBoard.attacks != 0;
	}

	const bool hasPossibleMoves(BitBoard bitBoard, const Color& color) const {
		generateAttacks(bitBoard, color, true);
		return bitBoard.attacks != 0;
	}

	//gera um bitmap das casas alcançadas pela color
	const void generateAttacks(BitBoard& bitBoard, const Color& color, const bool& validMoves) const {
		Bitmap attacks = 0;
		Square square;
		for (uChar namePiece = 0; namePiece < 6; namePiece++) {
			for (Bitmap pieceBitmap = bitBoard.bitMaps[namePiece][color]; pieceBitmap != 0; pieceBitmap = bitBoardOperations::remainder(pieceBitmap)) {
				square = bitBoardOperations::getSquareOfBitmap(pieceBitmap);
				if (validMoves)
					generateValidMoves(bitBoard, static_cast<PieceType>(namePiece), color, square);
				else
					generateMoves(bitBoard, static_cast<PieceType>(namePiece), color, square);
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.attacks);
			}
		}
		bitBoard.attacks = attacks;
	}

	//gera um bitmap das casas alcançadas pela peça sem colocar o rei da mesma cor em xeque
	const void generateValidMoves(BitBoard& bitBoard, const PieceType& pieceType, const Color& color, const Square& square) const {
		generateMoves(bitBoard, pieceType, color, square);
		tryToMakeMove(bitBoard, pieceType, color, square);
		tryToMakeCastle(bitBoard, pieceType, color);
	}

private:
	//retorna um bitmap das casas alcançadas pela pela peça
	void generateMoves(BitBoard& bitBoard, const PieceType& pieceType, const Color& color, const Square& square) const {
		switch (pieceType) {
			case PAWN:
				bitBoard.attacks = pawn::getMoves(bitBoard, color, square);
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
		bitBoard.attacks = bitBoardOperations::unsetIntersections(bitBoard.attacks, bitBoardOperations::allPiecesColor(bitBoard, color));
	}

	//tenta mover a peça sem colocar o rei em xeque
	void tryToMakeMove(BitBoard& bitBoard, const PieceType& pieceType, const Color& color, const Square& origin) const {
		Square destiny;

		for (Bitmap destinations = bitBoard.attacks; destinations != 0; destinations = bitBoardOperations::remainder(destinations)) {
			destiny = bitBoardOperations::getSquareOfBitmap(destinations);

			BitBoard clone(bitBoard);
			MoveMaker moveMaker(origin, destiny);
			moveMaker.makeMove(clone);

			generateAttacks(clone, ~color, false);
			if (bitBoardOperations::isKingCheck(clone, color))
				bitBoard.attacks = bitBoardOperations::unsetIntersections(bitBoard.attacks, bitBoardOperations::getBitmapOfSquare(destiny));
		}
	}

	//tenta fazer o movimento roque
	void tryToMakeCastle(BitBoard& bitBoard, const uChar& namePiece, const Color& color) const {
		if (namePiece == KING) {
			Bitmap attacks = bitBoard.attacks;
			generateAttacks(bitBoard, ~color, false);
			if (canMakeSmallCastle(bitBoard, color))
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.bitMaps[namePiece][color] << 2);
			if (canMakeBigCastle(bitBoard, color))
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.bitMaps[namePiece][color] >> 2);
			bitBoard.attacks = attacks;
		}
	}

	//este metodo verifica se o rei pode fazer o castelo
	const bool canMakeSmallCastle(const BitBoard& bitBoard, const Color& color) const {
		if (!king::wasKingMoved(bitBoard, color))
			if (!rook::wasSmallRookMoved(bitBoard, color))
				if (king::isPathSmallRookClear(bitBoard, color))
					return !bitBoardOperations::isKingCheck(bitBoard, color);
		return false;
	}

	//este metodo verifica se o rei pode fazer o castelo
	const bool canMakeBigCastle(const BitBoard& bitBoard, const Color& color) const {
		if (!king::wasKingMoved(bitBoard, color))
			if (!rook::wasBigRookMoved(bitBoard, color))
				if (king::isPathBigRookClear(bitBoard, color))
					return !bitBoardOperations::isKingCheck(bitBoard, color);
		return false;
	}
};