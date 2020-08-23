#pragma once
#include "BitBoardOperations.h"

namespace pawn {
	const Bitmap whitePawn(const BitBoard& bitBoard, const Bitmap allPieces, const Bitmap bitmapPawn) {
		const Bitmap enPassant = bitBoardOperations::getIntersections(bitBoard.flags, RANKS[RANK_4]);
		Bitmap moves = 0;

		//movimento normal e de posição inicial
		if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn << 8)) {
			moves = (bitmapPawn << 8);
			if (bitBoardOperations::hasIntersection(RANKS[RANK_2], bitmapPawn))
				if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn << 16))
					moves = bitBoardOperations::getUnion(moves, bitmapPawn << 16);
		}

		//movimento de captura e en Passant
		if (bitBoardOperations::hasIntersection(allPieces, bitmapPawn << 9))
			moves = bitBoardOperations::getUnion(moves, bitmapPawn << 9);
		else if (bitBoardOperations::hasIntersection(bitmapPawn << 1, enPassant))
			moves = bitBoardOperations::getUnion(moves, bitmapPawn << 9);
		if (bitBoardOperations::hasIntersection(allPieces, bitmapPawn << 7))
			moves = bitBoardOperations::getUnion(moves, bitmapPawn << 7);
		else if (bitBoardOperations::hasIntersection(bitmapPawn >> 1, enPassant))
			moves = bitBoardOperations::getUnion(moves, bitmapPawn << 7);

		return moves;
	}

	const Bitmap blackPawn(const BitBoard& bitBoard, const Bitmap allPieces, const Bitmap bitmapPawn) {
		const Bitmap enPassant = bitBoardOperations::getIntersections(bitBoard.flags, RANKS[RANK_5]);
		Bitmap moves = 0;

		//movimento normal e de posição inicial
		if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn >> 8)) {
			moves = (bitmapPawn >> 8);
			if (bitBoardOperations::hasIntersection(RANKS[RANK_7], bitmapPawn))
				if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn >> 16))
					moves = bitBoardOperations::getUnion(moves, bitmapPawn >> 16);
		}

		//movimento de captura
		if (bitBoardOperations::hasIntersection(allPieces, bitmapPawn >> 7))
			moves = bitBoardOperations::getUnion(moves, bitmapPawn >> 7);
		else if (bitBoardOperations::hasIntersection(bitmapPawn << 1, enPassant))
			moves = bitBoardOperations::getUnion(moves, bitmapPawn >> 7);
		if (bitBoardOperations::hasIntersection(allPieces, bitmapPawn >> 9))
			moves = bitBoardOperations::getUnion(moves, bitmapPawn >> 9);
		else if (bitBoardOperations::hasIntersection(bitmapPawn >> 1, enPassant))
			moves = bitBoardOperations::getUnion(moves, bitmapPawn >> 9);

		return moves;
	}

	const Bitmap getMoves(const BitBoard& bitBoard, const Color color, const Square& square) {
		const Bitmap bitmapPawn = bitBoardOperations::getBitmapOfSquare(square);
		Bitmap moves;

		if (color == WHITE)
			moves = whitePawn(bitBoard, bitBoardOperations::allPieces(bitBoard), bitmapPawn);
		else
			moves = blackPawn(bitBoard, bitBoardOperations::allPieces(bitBoard), bitmapPawn);

		if (bitBoardOperations::hasIntersection(bitmapPawn, FILES[FILE_A]))
			moves = bitBoardOperations::unsetIntersections(moves, FILES[FILE_H]);
		else if (bitBoardOperations::hasIntersection(bitmapPawn, FILES[FILE_H]))
			moves = bitBoardOperations::unsetIntersections(moves, FILES[FILE_A]);

		return moves;
	}
}