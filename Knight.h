#ifndef KNIGHT_H_INCLUDED
#define KNIGHT_H_INCLUDED
#include "BitBoard.h"
#include "BitBoardOperations.h"

namespace knight {
    const uint64_t getMoves(const BitBoard& bitBoard, const Position& posKnight) {
		const uint64_t& bitmapKnight = bitBoardOperations::getBitmapFromSquare(posKnight.getSquare());

		uint64_t moves = bitmapKnight << 10 & ~(bitBoardOperations::FILE_A | bitBoardOperations::FILE_B);
		moves = bitBoardOperations::getUnion(moves, bitmapKnight << 17 & ~bitBoardOperations::FILE_A);
		moves = bitBoardOperations::getUnion(moves, bitmapKnight << 6 & ~(bitBoardOperations::FILE_G | bitBoardOperations::FILE_H));
		moves = bitBoardOperations::getUnion(moves, bitmapKnight << 15 & ~bitBoardOperations::FILE_H);

		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 10 & ~(bitBoardOperations::FILE_G | bitBoardOperations::FILE_H));
		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 17 & ~bitBoardOperations::FILE_H);
		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 6 & ~(bitBoardOperations::FILE_A | bitBoardOperations::FILE_B));
		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 15 & ~bitBoardOperations::FILE_A);

		return moves;
	}
}
#endif // KNIGHT_H_INCLUDED
