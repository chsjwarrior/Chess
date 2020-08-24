#pragma once
#include "BitBoardOperations.h"

namespace king {
	const Bitmap getMoves(Square square) {
		const Bitmap bitmapKing = bitBoardOperations::getBitmapOfSquare(square);

		//casas acima e abaixo do rei
		Bitmap moves = bitBoardOperations::getUnion(bitmapKing << 8, bitmapKing >> 8);
		//casas a direita do rei
		moves = bitBoardOperations::getUnion(moves, bitmapKing << 1 & ~FILES[FILE_A]);
		moves = bitBoardOperations::getUnion(moves, bitmapKing << 9 & ~FILES[FILE_A]);
		moves = bitBoardOperations::getUnion(moves, bitmapKing << 7 & ~FILES[FILE_H]);
		//casas a esquerda do rei
		moves = bitBoardOperations::getUnion(moves, bitmapKing >> 1 & ~FILES[FILE_H]);
		moves = bitBoardOperations::getUnion(moves, bitmapKing >> 9 & ~FILES[FILE_H]);
		moves = bitBoardOperations::getUnion(moves, bitmapKing >> 7 & ~FILES[FILE_A]);

		return moves;
	}

	const bool wasKingMoved(const BitBoard& bitBoard, const Color color) {
		return !bitBoardOperations::hasIntersection(bitBoard.flags, INITIAL_POSITION[KING][color]);
	}

	/* Este método verifica se as casas do castelo do rei estão livres */
	const bool isPathSmallRookClear(const BitBoard& bitBoard, const Color color) {
		const Bitmap allPieces = bitBoardOperations::getBitmapAllPieces(bitBoard);
		const Bitmap kingBitmap = INITIAL_POSITION[KING][color];
		if (!bitBoardOperations::hasIntersection(bitBoard.attacks, kingBitmap << 1))
			if (!bitBoardOperations::hasIntersection(allPieces, kingBitmap << 1))
				if (!bitBoardOperations::hasIntersection(bitBoard.attacks, kingBitmap << 2))
					return (!bitBoardOperations::hasIntersection(allPieces, kingBitmap << 2));
		return false;
	}

	/* Este método verifica se a as casas do castelo da dama estão livres */
	const bool isPathBigRookClear(const BitBoard& bitBoard, const Color color) {
		const Bitmap allPieces = bitBoardOperations::getBitmapAllPieces(bitBoard);
		const Bitmap kingBitmap = INITIAL_POSITION[KING][color];
		if (!bitBoardOperations::hasIntersection(bitBoard.attacks, kingBitmap >> 1))
			if (!bitBoardOperations::hasIntersection(allPieces, kingBitmap >> 1))
				if (!bitBoardOperations::hasIntersection(bitBoard.attacks, kingBitmap >> 2))
					if (!bitBoardOperations::hasIntersection(allPieces, kingBitmap >> 2))
						return (!bitBoardOperations::hasIntersection(allPieces, kingBitmap >> 3));
		return false;
	}
}