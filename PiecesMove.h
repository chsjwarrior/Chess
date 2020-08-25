#pragma once
#include "BitBoardOperations.h"

/*
This class has functions that returns the attacks bitmaps from each piece
*/
namespace pieceMoves {
	const Bitmap getBishopMoves(Bitmap allPieces, Square square) {
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

	const Bitmap getRookMoves(Bitmap allPieces, Square square) {
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

	const Bitmap getKnightMoves(Square square) {
		const Bitmap& bitmapKnight = bitBoardOperations::getBitmapOfSquare(square);

		Bitmap moves = bitmapKnight << 10 & ~(FILES[FILE_A] | FILES[FILE_B]);
		moves = bitBoardOperations::getUnion(moves, bitmapKnight << 17 & ~FILES[FILE_A]);
		moves = bitBoardOperations::getUnion(moves, bitmapKnight << 6 & ~(FILES[FILE_G] | FILES[FILE_H]));
		moves = bitBoardOperations::getUnion(moves, bitmapKnight << 15 & ~FILES[FILE_H]);

		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 10 & ~(FILES[FILE_G] | FILES[FILE_H]));
		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 17 & ~FILES[FILE_H]);
		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 6 & ~(FILES[FILE_A] | FILES[FILE_B]));
		moves = bitBoardOperations::getUnion(moves, bitmapKnight >> 15 & ~FILES[FILE_A]);

		return moves;
	}

	const Bitmap getKingMoves(Square square) {
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

	const Bitmap getPawnMoves(Bitmap allPieces, Color color, Bitmap enPassant, Square square) {
		const Bitmap bitmapPawn = bitBoardOperations::getBitmapOfSquare(square);
		Bitmap moves = 0;

		if (color == WHITE) {
			//movimento normal e de posição inicial
			if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn << 8)) {
				moves = (bitmapPawn << 8);
				if (bitBoardOperations::hasIntersection(RANKS[RANK_2], bitmapPawn))
					if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn << 16))
						moves = bitBoardOperations::getUnion(moves, bitmapPawn << 16);
			}
			//movimento de captura
			if (bitBoardOperations::hasIntersection(allPieces, bitmapPawn << 9) ||
				bitBoardOperations::hasIntersection(bitmapPawn << 1, enPassant))
				moves = bitBoardOperations::getUnion(moves, bitmapPawn << 9);
			if (bitBoardOperations::hasIntersection(allPieces, bitmapPawn << 7) ||
				bitBoardOperations::hasIntersection(bitmapPawn >> 1, enPassant))
				moves = bitBoardOperations::getUnion(moves, bitmapPawn << 7);
		}
		else if (color == BLACK) {
			//movimento normal e de posição inicial
			if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn >> 8)) {
				moves = (bitmapPawn >> 8);
				if (bitBoardOperations::hasIntersection(RANKS[RANK_7], bitmapPawn))
					if (!bitBoardOperations::hasIntersection(allPieces, bitmapPawn >> 16))
						moves = bitBoardOperations::getUnion(moves, bitmapPawn >> 16);
			}
			//movimento de captura
			if (bitBoardOperations::hasIntersection(allPieces, bitmapPawn >> 7) ||
				bitBoardOperations::hasIntersection(bitmapPawn << 1, enPassant))
				moves = bitBoardOperations::getUnion(moves, bitmapPawn >> 7);
			if (bitBoardOperations::hasIntersection(allPieces, bitmapPawn >> 9) ||
				bitBoardOperations::hasIntersection(bitmapPawn >> 1, enPassant))
				moves = bitBoardOperations::getUnion(moves, bitmapPawn >> 9);
		}

		if (bitBoardOperations::hasIntersection(bitmapPawn, FILES[FILE_A]))
			moves = bitBoardOperations::unsetIntersections(moves, FILES[FILE_H]);
		else if (bitBoardOperations::hasIntersection(bitmapPawn, FILES[FILE_H]))
			moves = bitBoardOperations::unsetIntersections(moves, FILES[FILE_A]);

		return moves;
	}
}