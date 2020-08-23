#pragma once
#include "BitBoardOperations.h"

namespace bishop {
	const Bitmap getMoves(const Bitmap& allPieces, const Square square) {
		const Bitmap bitmapBishop = bitBoardOperations::getBitmapOfSquare(square);
		const File file = bitBoardOperations::getFileOfSquare(square);
		const Rank rank = bitBoardOperations::getRankOfSquare(square);

		uShort index = file + rank;
		Bitmap aux1 = (allPieces & DIAGONAL_L[index]) - 2 * bitmapBishop;
		Bitmap aux2 = bitBoardOperations::reverse(allPieces & DIAGONAL_L[index]) - 2 * bitBoardOperations::reverse(bitmapBishop);
		const Bitmap ldMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & DIAGONAL_L[index];

		index = file + 7 - rank;
		aux1 = (allPieces & DIAGONAL_R[index]) - 2 * bitmapBishop;
		aux2 = bitBoardOperations::reverse(allPieces & DIAGONAL_R[index]) - 2 * bitBoardOperations::reverse(bitmapBishop);
		const Bitmap dlMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & DIAGONAL_R[index];

		return ldMoves | dlMoves;
	}
}