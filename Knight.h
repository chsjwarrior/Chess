#pragma once
#include "BitBoardOperations.h"

namespace knight {
	const Tbitmap getMoves(const uChar& square) {
		const Tbitmap& bitmapKnight = bitBoardOperations::getBitmapFromSquare(square);

		Tbitmap moves = bitmapKnight << 10 & ~(FILE_A | FILE_B);
		moves = bitBoardOperations::getUnion(moves, bitmapKnight << 17 & ~FILE_A);
		moves = bitBoardOperations::getUnion(moves, bitmapKnight << 6 & ~(FILE_G | FILE_H));
		moves = bitBoardOperations::getUnion(moves, bitmapKnight << 15 & ~FILE_H);

		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 10 & ~(FILE_G | FILE_H));
		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 17 & ~FILE_H);
		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 6 & ~(FILE_A | FILE_B));
		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 15 & ~FILE_A);

		return moves;
	}
}