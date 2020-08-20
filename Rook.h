#pragma once
#include "BitBoard.h"
#include "BitBoardOperations.h"

namespace rook {
	const uint64_t getMoves(const BitBoard& bitBoard, const uint8_t& square) {
		const uint64_t allPieces = bitBoardOperations::allPieces(bitBoard);
		const uint64_t& bitmapRook = bitBoardOperations::getBitmapFromSquare(square);
		const uint8_t file = bitBoardOperations::getFileFromSquare(square);
		const uint8_t rank = bitBoardOperations::getRankFromSquare(square);

		uint64_t aux1 = allPieces - 2 * bitmapRook;
		uint64_t aux2 = bitBoardOperations::reverse(allPieces) - 2 * bitBoardOperations::reverse(bitmapRook);
		const uint64_t hMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & bitBoardOperations::RANKS[rank];

		aux1 = (allPieces & bitBoardOperations::FILES[file]) - (2 * bitmapRook);
		aux2 = bitBoardOperations::reverse(allPieces & bitBoardOperations::FILES[file]) - 2 * bitBoardOperations::reverse(bitmapRook);
		const uint64_t vMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & bitBoardOperations::FILES[file];

		return hMoves | vMoves;
	}

	const bool wasSmallRookMoved(const BitBoard& bitBoard, const uint8_t colorRook) {
		if (colorRook == (uint8_t)Piece::Color::WHITE)
			return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapFromSquare(7));
		return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapFromSquare(63));
	}

	const bool wasBigRookMoved(const BitBoard& bitBoard, const uint8_t colorRook) {
		if (colorRook == (uint8_t)Piece::Color::WHITE)
			return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapFromSquare(0));
		return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapFromSquare(56));
	}
}