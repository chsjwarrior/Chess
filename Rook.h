#ifndef ROOK_H_INCLUDED
#define ROOK_H_INCLUDED
#include "BitBoard.h"
#include "BitBoardOperations.h"

namespace rook {
	const uint64_t getMoves(const BitBoard& bitBoard, const Position& posRook) {
		const uint64_t allPieces = bitBoard.allPieces();
		const uint64_t& bitmapRook = bitBoardOperations::getBitmapFromSquare(posRook.getSquare());

		uint64_t aux1 = allPieces - 2 * bitmapRook;
		uint64_t aux2 = bitBoardOperations::reverse(allPieces) - 2 * bitBoardOperations::reverse(bitmapRook);
		const uint64_t hMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & bitBoardOperations::RANKS[posRook.getRank()];

		aux1 = (allPieces & bitBoardOperations::FILES[posRook.getFile()]) - (2 * bitmapRook);
		aux2 = bitBoardOperations::reverse(allPieces & bitBoardOperations::FILES[posRook.getFile()]) - 2 * bitBoardOperations::reverse(bitmapRook);
		const uint64_t vMoves = (aux1 ^ bitBoardOperations::reverse(aux2)) & bitBoardOperations::FILES[posRook.getFile()];

		return hMoves | vMoves;
	}

	const bool wasSmallRookMoved(const BitBoard& bitBoard, const uint8_t& colorRook) {
		if (colorRook == Piece::COLOR::WHITE)
			return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapFromSquare(7));
		return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapFromSquare(63));
	}

	const bool wasBigRookMoved(const BitBoard& bitBoard, const uint8_t& colorRook) {
		if (colorRook == Piece::COLOR::WHITE)
			return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapFromSquare(0));
		return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapFromSquare(56));
	}
}
#endif // ROOK_H_INCLUDED