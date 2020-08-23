#pragma once
#include "BitBoardOperations.h"

namespace knight {
	const Bitmap getMoves(const Square square) {
		const Bitmap& bitmapKnight = bitBoardOperations::getBitmapOfSquare(square);

		Bitmap moves = bitmapKnight << 10 & ~(FILES[FILE_A] | FILES[FILE_B]);
		moves = bitBoardOperations::getUnion(moves, bitmapKnight << 17 & ~FILES[FILE_A]);
		moves = bitBoardOperations::getUnion(moves, bitmapKnight << 6 & ~(FILES[FILE_G] | FILES[FILE_H]));
		moves = bitBoardOperations::getUnion(moves, bitmapKnight << 15 & ~FILES[FILE_H]);

		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 10 & ~(FILES[FILE_G] | FILES[FILE_H]));
		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 17 & ~FILES[FILE_H]);
		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 6 & ~(FILES[FILE_A] | FILES[FILE_B]));
		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 15 & ~FILES[FILE_A]);

		return moves;
	}
}