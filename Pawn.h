#pragma once
#include "BitBoardOperations.h"

namespace pawn {
	const Tbitmap whitePawn(const BitBoard& bitBoard, const Tbitmap allPieces, const Tbitmap bitmapPawn) {
		const Tbitmap enPassant = bitBoardOperations::getIntersections(bitBoard.flags, RANK_4);
		Tbitmap moves = 0;

		//movimento normal e de posição inicial
		if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn << 8)) {
			moves = (bitmapPawn << 8);
			if (bitBoardOperations::hasIntersection(RANK_2, bitmapPawn))
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

	const Tbitmap blackPawn(const BitBoard& bitBoard, const Tbitmap allPieces, const Tbitmap bitmapPawn) {
		const Tbitmap enPassant = bitBoardOperations::getIntersections(bitBoard.flags, RANK_5);
		Tbitmap moves = 0;

		//movimento normal e de posição inicial
		if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn >> 8)) {
			moves = (bitmapPawn >> 8);
			if (bitBoardOperations::hasIntersection(RANK_7, bitmapPawn))
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

	const Tbitmap getMoves(const BitBoard& bitBoard, const uChar colorPawn, const uChar& square) {
		const Tbitmap bitmapPawn = bitBoardOperations::getBitmapFromSquare(square);
		Tbitmap moves;

		if (colorPawn == WHITE)
			moves = whitePawn(bitBoard, bitBoardOperations::allPieces(bitBoard), bitmapPawn);
		else
			moves = blackPawn(bitBoard, bitBoardOperations::allPieces(bitBoard), bitmapPawn);

		if (bitBoardOperations::hasIntersection(bitmapPawn, FILE_A))
			moves = bitBoardOperations::unsetIntersections(moves, FILE_H);
		else if (bitBoardOperations::hasIntersection(bitmapPawn, FILE_H))
			moves = bitBoardOperations::unsetIntersections(moves, FILE_A);

		return moves;
	}
}