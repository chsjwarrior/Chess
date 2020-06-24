#pragma once
#include "BitBoard.h"
#include "BitBoardOperations.h"

namespace bishop {
    const uint64_t getMoves(const BitBoard& bitBoard, const Position& posBishop) {
		const uint64_t allPieces = bitBoard.allPieces();
		const uint64_t bitmapBishop = bitBoardOperations::getBitmapFromSquare(posBishop.getSquare());

		uint16_t index = posBishop.getFile() + posBishop.getRank();
		uint64_t aux1 = (allPieces & bitBoardOperations::DIAGONAL_L[index]) - 2 * bitmapBishop;
		uint64_t aux2 = bitBoardOperations::reverse(allPieces & bitBoardOperations::DIAGONAL_L[index]) - 2 * bitBoardOperations::reverse(bitmapBishop);
		const uint64_t ldMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & bitBoardOperations::DIAGONAL_L[index];

		index = posBishop.getFile() + 7 - posBishop.getRank();
		aux1 = (allPieces & bitBoardOperations::DIAGONAL_R[index]) - 2 * bitmapBishop;
		aux2 = bitBoardOperations::reverse(allPieces & bitBoardOperations::DIAGONAL_R[index]) - 2 * bitBoardOperations::reverse(bitmapBishop);
		const uint64_t dlMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & bitBoardOperations::DIAGONAL_R[index];

		return ldMoves | dlMoves;
	}
}