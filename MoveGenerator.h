#pragma once
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "King.h"
#include "MoveMaker.h"

class MoveGenerator {
public:
	const bool hasPossibleMoves(BitBoard& bitBoard, PieceType pieceType, Color color, Square square) const {
		generateValidMoves(bitBoard, pieceType, color, square);
		return bitBoard.attacks != 0;
	}

	const bool hasPossibleMoves(BitBoard& bitBoard, Color color) const {
		generateMoves(bitBoard, color, true);
		return bitBoard.attacks != 0;
	}

	//gera um bitmap das casas alcançadas pela color
	void generateMoves(BitBoard& bitBoard, Color color, bool validMoves) const {
		Bitmap attacks = 0;
		Square square;
		for (uChar pieceType = 0; pieceType < 6; pieceType++) {
			for (Bitmap pieceBitmap = bitBoard.bitMaps[pieceType][color]; pieceBitmap != 0; pieceBitmap = bitBoardOperations::remainder(pieceBitmap)) {
				square = bitBoardOperations::getSquareOfBitmap(pieceBitmap);
				if (validMoves)
					generateValidMoves(bitBoard, static_cast<PieceType>(pieceType), color, square);
				else
					generateMoves(bitBoard, static_cast<PieceType>(pieceType), color, square);
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.attacks);
			}
		}
		bitBoard.attacks = attacks;
	}

	//gera um bitmap das casas alcançadas pela peça sem colocar o rei da mesma cor em xeque
	const void generateValidMoves(BitBoard& bitBoard, const PieceType pieceType, const Color color, const Square square) const {
		generateMoves(bitBoard, pieceType, color, square);
		tryToMakeMove(bitBoard, pieceType, color, square);
		tryToMakeCastle(bitBoard, pieceType, color);
	}

private:
	//retorna um bitmap das casas em ataque
	void generateMoves(BitBoard& bitBoard, PieceType pieceType, Color color, Square square) const {
		switch (pieceType) {
			case PAWN:
				bitBoard.attacks = pawn::getMoves(bitBoardOperations::getBitmapAllPieces(bitBoard), color, bitBoard.flags & (RANKS[RANK_4] | RANKS[RANK_5]), square);
				break;
			case KNIGHT:
				bitBoard.attacks = knight::getMoves(square);
				break;
			case BISHOP:
				bitBoard.attacks = bishop::getMoves(bitBoardOperations::getBitmapAllPieces(bitBoard), square);
				break;
			case ROOK:
				bitBoard.attacks = rook::getMoves(bitBoardOperations::getBitmapAllPieces(bitBoard), square);
				break;
			case QUEEN: {
				const Bitmap allPiece = bitBoardOperations::getBitmapAllPieces(bitBoard);
				bitBoard.attacks = bishop::getMoves(allPiece, square) | rook::getMoves(allPiece, square);
				break;
			}
			case KING:
				bitBoard.attacks = king::getMoves(square);
		}
		bitBoard.attacks = bitBoardOperations::unsetIntersections(bitBoard.attacks, bitBoardOperations::getBitmapAllPiecesByColor(bitBoard, color));
	}

	//tenta mover a peça
	void tryToMakeMove(BitBoard& bitBoard, PieceType pieceType, Color color, Square origin) const {
		Square destiny;
		for (Bitmap destinations = bitBoard.attacks; destinations != 0; destinations = bitBoardOperations::remainder(destinations)) {
			destiny = bitBoardOperations::getSquareOfBitmap(destinations);

			BitBoard clone(bitBoard);
			MoveMaker moveMaker(origin, destiny);
			moveMaker.makeMove(clone);

			generateMoves(clone, ~color, false);
			if (bitBoardOperations::isKingCheck(clone, color))
				bitBoard.attacks = bitBoardOperations::unsetIntersections(bitBoard.attacks, bitBoardOperations::getBitmapOfSquare(destiny));
		}
	}

	//tenta fazer o movimento roque
	void tryToMakeCastle(BitBoard& bitBoard, PieceType pieceType, Color color) const {
		if (pieceType == KING) {
			Bitmap attacks = bitBoard.attacks;
			generateMoves(bitBoard, ~color, false);
			if (canMakeSmallCastle(bitBoard, color))
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.bitMaps[pieceType][color] << 2);
			if (canMakeBigCastle(bitBoard, color))
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.bitMaps[pieceType][color] >> 2);
			bitBoard.attacks = attacks;
		}
	}

	//este metodo verifica se o rei pode fazer o castelo
	const bool canMakeSmallCastle(const BitBoard& bitBoard, Color color) const {
		if (!king::wasKingMoved(bitBoard, color))
			if (!rook::wasSmallRookMoved(bitBoard, color))
				if (king::isPathSmallRookClear(bitBoard, color))
					return !bitBoardOperations::isKingCheck(bitBoard, color);
		return false;
	}

	//este metodo verifica se o rei pode fazer o castelo
	const bool canMakeBigCastle(const BitBoard& bitBoard, Color color) const {
		if (!king::wasKingMoved(bitBoard, color))
			if (!rook::wasBigRookMoved(bitBoard, color))
				if (king::isPathBigRookClear(bitBoard, color))
					return !bitBoardOperations::isKingCheck(bitBoard, color);
		return false;
	}
};