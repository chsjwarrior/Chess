#pragma once
#include "BitBoard.h"
#include "BitBoardOperations.h"

namespace king {
	const uint64_t getMoves(const BitBoard& bitBoard, const Position& posKing) {
		const uint64_t bitmapKing = bitBoardOperations::getBitmapFromSquare(posKing.getSquare());

		/* casas acima e abaixo do rei */
		uint64_t moves = bitBoardOperations::getUnion(bitmapKing << 8, bitmapKing >> 8);
		/* casas a direita do rei */
		moves = bitBoardOperations::getUnion(moves, bitmapKing << 1 & ~bitBoardOperations::FILE_A);
		moves = bitBoardOperations::getUnion(moves, bitmapKing << 9 & ~bitBoardOperations::FILE_A);
		moves = bitBoardOperations::getUnion(moves, bitmapKing << 7 & ~bitBoardOperations::FILE_H);
		/* casas a esquerda do rei */
		moves = bitBoardOperations::getUnion(moves, bitmapKing >> 1 & ~bitBoardOperations::FILE_H);
		moves = bitBoardOperations::getUnion(moves, bitmapKing >> 9 & ~bitBoardOperations::FILE_H);
		moves = bitBoardOperations::getUnion(moves, bitmapKing >> 7 & ~bitBoardOperations::FILE_A);

		return moves;
	}

	const bool wasKingMoved(const BitBoard& bitBoard, const uint8_t& colorKing) {
		return !bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::INITIAL_POSITION[(uint8_t) Piece::NAME::KING][colorKing]);
	}

	/* Este método verifica se as casas do castelo do rei estão livres */
	const bool isPathSmallRookClear(const BitBoard& bitBoard, const uint8_t color) {
		const uint64_t allPieces = bitBoard.allPieces();
		const uint64_t kingBitmap = bitBoardOperations::INITIAL_POSITION[(uint8_t) Piece::NAME::KING][color];
		if (!bitBoardOperations::hasIntersection(bitBoard.attacks, kingBitmap << 1))
			if (!bitBoardOperations::hasIntersection(allPieces, kingBitmap << 1))
				if (!bitBoardOperations::hasIntersection(bitBoard.attacks, kingBitmap << 2))
					return (!bitBoardOperations::hasIntersection(allPieces, kingBitmap << 2));
		return false;
	}

	/* Este método verifica se a as casas do castelo da dama estão livres */
	const bool isPathBigRookClear(const BitBoard& bitBoard, const uint8_t color) {
		const uint64_t allPieces = bitBoard.allPieces();
		const uint64_t kingBitmap = bitBoardOperations::INITIAL_POSITION[(uint8_t) Piece::NAME::KING][color];
		if (!bitBoardOperations::hasIntersection(bitBoard.attacks, kingBitmap >> 1))
			if (!bitBoardOperations::hasIntersection(allPieces, kingBitmap >> 1))
				if (!bitBoardOperations::hasIntersection(bitBoard.attacks, kingBitmap >> 2))
					if (!bitBoardOperations::hasIntersection(allPieces, kingBitmap >> 2))
						return (!bitBoardOperations::hasIntersection(allPieces, kingBitmap >> 3));
		return false;
	}
}