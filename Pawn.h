#ifndef PAWN_H_INCLUDED
#define PAWN_H_INCLUDED
#include "BitBoard.h"
#include "BitBoardOperations.h"

namespace pawn {
	const uint64_t whitePawn(const BitBoard& bitBoard, const uint64_t& allPieces, const uint64_t& bitmapPawn) {
		const uint64_t enPassant = bitBoardOperations::getIntersections(bitBoard.flags, bitBoardOperations::RANK_4);
		uint64_t moves = 0;

		/* movimento normal e de posição inicial */
		if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn << 8)) {
			moves = (bitmapPawn << 8);
			if (bitBoardOperations::hasIntersection(bitBoardOperations::RANK_2, bitmapPawn))
				if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn << 16))
					moves = bitBoardOperations::getUnion(moves, bitmapPawn << 16);
		}

		/* movimento de captura e en Passant */
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

	const uint64_t blackPawn(const BitBoard& bitBoard, const uint64_t& allPieces, const uint64_t& bitmapPawn) {
		const uint64_t enPassant = bitBoardOperations::getIntersections(bitBoard.flags, bitBoardOperations::RANK_5);
		uint64_t moves = 0;

		/* movimento normal e de posição inicial */
		if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn >> 8)) {
			moves = (bitmapPawn >> 8);
			if (bitBoardOperations::hasIntersection(bitBoardOperations::RANK_7, bitmapPawn))
				if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn >> 16))
					moves = bitBoardOperations::getUnion(moves, bitmapPawn >> 16);
		}

		/* movimento de captura */
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

	const uint64_t getMoves(const BitBoard& bitBoard, const uint8_t& colorPawn, const Position& posPawn) {
		const uint64_t bitmapPawn = bitBoardOperations::getBitmapFromSquare(posPawn.getSquare());
		uint64_t moves;

		if (colorPawn == pieces::WHITE)
			moves = whitePawn(bitBoard, bitBoard.allPieces(), bitmapPawn);
		else
			moves = blackPawn(bitBoard, bitBoard.allPieces(), bitmapPawn);

		if (bitBoardOperations::hasIntersection(bitmapPawn, bitBoardOperations::FILE_A))
			moves = bitBoardOperations::unsetIntersections(moves, bitBoardOperations::FILE_H);
		else if (bitBoardOperations::hasIntersection(bitmapPawn, bitBoardOperations::FILE_H))
			moves = bitBoardOperations::unsetIntersections(moves, bitBoardOperations::FILE_A);

		return moves;
	}
}
#endif // PAWN_H_INCLUDED