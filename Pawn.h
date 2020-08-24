#pragma once
#include "BitBoardOperations.h"

namespace pawn {
	const Bitmap getMoves(Bitmap allPieces, Color color, Bitmap enPassant, Square square) {
		const Bitmap bitmapPawn = bitBoardOperations::getBitmapOfSquare(square);
		Bitmap moves = 0;

		if (color == WHITE) {
			//movimento normal e de posição inicial
			if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn << 8)) {
				moves = (bitmapPawn << 8);
				if (bitBoardOperations::hasIntersection(RANKS[RANK_2], bitmapPawn))
					if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn << 16))
						moves = bitBoardOperations::getUnion(moves, bitmapPawn << 16);
			}
			//movimento de captura
			if (bitBoardOperations::hasIntersection(allPieces, bitmapPawn << 9) ||
				bitBoardOperations::hasIntersection(bitmapPawn << 1, enPassant))
				moves = bitBoardOperations::getUnion(moves, bitmapPawn << 9);
			if (bitBoardOperations::hasIntersection(allPieces, bitmapPawn << 7) ||
				bitBoardOperations::hasIntersection(bitmapPawn >> 1, enPassant))
				moves = bitBoardOperations::getUnion(moves, bitmapPawn << 7);
		}
		else if (color == BLACK) {
			//movimento normal e de posição inicial
			if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn >> 8)) {
				moves = (bitmapPawn >> 8);
				if (bitBoardOperations::hasIntersection(RANKS[RANK_7], bitmapPawn))
					if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn >> 16))
						moves = bitBoardOperations::getUnion(moves, bitmapPawn >> 16);
			}
			//movimento de captura
			if (bitBoardOperations::hasIntersection(allPieces, bitmapPawn >> 7) ||
				bitBoardOperations::hasIntersection(bitmapPawn << 1, enPassant))
				moves = bitBoardOperations::getUnion(moves, bitmapPawn >> 7);
			if (bitBoardOperations::hasIntersection(allPieces, bitmapPawn >> 9) ||
				bitBoardOperations::hasIntersection(bitmapPawn >> 1, enPassant))
				moves = bitBoardOperations::getUnion(moves, bitmapPawn >> 9);
		}

		if (bitBoardOperations::hasIntersection(bitmapPawn, FILES[FILE_A]))
			moves = bitBoardOperations::unsetIntersections(moves, FILES[FILE_H]);
		else if (bitBoardOperations::hasIntersection(bitmapPawn, FILES[FILE_H]))
			moves = bitBoardOperations::unsetIntersections(moves, FILES[FILE_A]);

		return moves;
	}
}