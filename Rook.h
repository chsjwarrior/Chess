#pragma once
#include "BitBoardOperations.h"

namespace rook {
	const Tbitmap getMoves(const Tbitmap& allPieces, const uChar& square) {
		const Tbitmap bitmapRook = bitBoardOperations::getBitmapFromSquare(square);
		const uChar file = bitBoardOperations::getFileFromSquare(square);
		const uChar rank = bitBoardOperations::getRankFromSquare(square);

		Tbitmap aux1 = allPieces - 2 * bitmapRook;
		Tbitmap aux2 = bitBoardOperations::reverse(allPieces) - 2 * bitBoardOperations::reverse(bitmapRook);
		const Tbitmap hMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & RANKS[rank];

		aux1 = (allPieces & FILES[file]) - 2 * bitmapRook;
		aux2 = bitBoardOperations::reverse(allPieces & FILES[file]) - 2 * bitBoardOperations::reverse(bitmapRook);
		const Tbitmap vMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & FILES[file];

		return hMoves | vMoves;
	}

	const bool wasSmallRookMoved(const BitBoard& bitBoard, const uChar colorRook) {
		if (colorRook == WHITE)
			return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapFromSquare(7));
		return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapFromSquare(63));
	}

	const bool wasBigRookMoved(const BitBoard& bitBoard, const uChar colorRook) {
		if (colorRook == WHITE)
			return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapFromSquare(0));
		return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapFromSquare(56));
	}
}