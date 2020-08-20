#pragma once
#include "BitBoard.h"
#include "BitBoardOperations.h"

namespace bishop {
	const uint64_t getMoves(const BitBoard& bitBoard, const uint8_t& square) {
		const uint64_t allPieces = bitBoardOperations::allPieces(bitBoard);
		const uint64_t bitmapBishop = bitBoardOperations::getBitmapFromSquare(square);
		const uint8_t file = bitBoardOperations::getFileFromSquare(square);
		const uint8_t rank = bitBoardOperations::getRankFromSquare(square);

		uint16_t index = file + rank;
		uint64_t aux1 = (allPieces & bitBoardOperations::DIAGONAL_L[index]) - 2 * bitmapBishop;
		uint64_t aux2 = bitBoardOperations::reverse(allPieces & bitBoardOperations::DIAGONAL_L[index]) - 2 * bitBoardOperations::reverse(bitmapBishop);
		const uint64_t ldMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & bitBoardOperations::DIAGONAL_L[index];

		index = file + 7 - rank;
		aux1 = (allPieces & bitBoardOperations::DIAGONAL_R[index]) - 2 * bitmapBishop;
		aux2 = bitBoardOperations::reverse(allPieces & bitBoardOperations::DIAGONAL_R[index]) - 2 * bitBoardOperations::reverse(bitmapBishop);
		const uint64_t dlMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & bitBoardOperations::DIAGONAL_R[index];

		return ldMoves | dlMoves;
	}
}