#pragma once
#include "BitBoardOperations.h"

namespace bishop {
	const Tbitmap getMoves(const BitBoard& bitBoard, const uChar& square) {
		const Tbitmap allPieces = bitBoardOperations::allPieces(bitBoard);
		const Tbitmap bitmapBishop = bitBoardOperations::getBitmapFromSquare(square);
		const uChar file = bitBoardOperations::getFileFromSquare(square);
		const uChar rank = bitBoardOperations::getRankFromSquare(square);

		uShort index = file + rank;
		Tbitmap aux1 = (allPieces & DIAGONAL_L[index]) - 2 * bitmapBishop;
		Tbitmap aux2 = bitBoardOperations::reverse(allPieces & DIAGONAL_L[index]) - 2 * bitBoardOperations::reverse(bitmapBishop);
		const Tbitmap ldMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & DIAGONAL_L[index];

		index = file + 7 - rank;
		aux1 = (allPieces & DIAGONAL_R[index]) - 2 * bitmapBishop;
		aux2 = bitBoardOperations::reverse(allPieces & DIAGONAL_R[index]) - 2 * bitBoardOperations::reverse(bitmapBishop);
		const Tbitmap dlMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & DIAGONAL_R[index];

		return ldMoves | dlMoves;
	}
}