#pragma once
#include "BitBoardOperations.h"

namespace rook {
	const Bitmap getMoves(Bitmap allPieces, Square square) {
		const Bitmap bitmapRook = bitBoardOperations::getBitmapOfSquare(square);
		const File file = bitBoardOperations::getFileOfSquare(square);
		const Rank rank = bitBoardOperations::getRankOfSquare(square);

		Bitmap aux1 = allPieces - 2 * bitmapRook;
		Bitmap aux2 = bitBoardOperations::reverse(allPieces) - 2 * bitBoardOperations::reverse(bitmapRook);
		const Bitmap hMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & RANKS[rank];

		aux1 = (allPieces & FILES[file]) - 2 * bitmapRook;
		aux2 = bitBoardOperations::reverse(allPieces & FILES[file]) - 2 * bitBoardOperations::reverse(bitmapRook);
		const Bitmap vMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & FILES[file];

		return hMoves | vMoves;
	}

	const bool wasSmallRookMoved(const BitBoard& bitBoard, const Color color) {
		if (color == WHITE)
			return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapOfSquare(H1));
		return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapOfSquare(H8));
	}

	const bool wasBigRookMoved(const BitBoard& bitBoard, const Color color) {
		if (color == WHITE)
			return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapOfSquare(A1));
		return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapOfSquare(A8));
	}
}