#pragma once
#include "PiecesMove.h"
#include "MoveMaker.h"

class MoveGenerator {
public:
	const bool hasPossibleMoves(BitBoard& bitBoard, PieceType pieceType, Color color, Square square) const {
		generateValidMoves(bitBoard, pieceType, color, square);
		return bitBoard.attacks != 0;
	}

	const bool hasPossibleMoves(BitBoard& bitBoard, Color color) const {
		generateMoves(bitBoard, color, true);
		return bitBoard.attacks != 0;
	}

	//gera um bitmap das casas alcançadas pela color
	void generateMoves(BitBoard& bitBoard, Color color, bool validMoves) const {
		Bitmap attacks = 0;
		Square square;
		for (uChar pieceType = 0; pieceType < 6; pieceType++) {
			for (Bitmap pieceBitmap = bitBoard.bitMaps[pieceType][color]; pieceBitmap != 0; pieceBitmap = bitBoardOperations::remainder(pieceBitmap)) {
				square = bitBoardOperations::getSquareOfBitmap(pieceBitmap);
				if (validMoves)
					generateValidMoves(bitBoard, static_cast<PieceType>(pieceType), color, square);
				else
					generateMoves(bitBoard, static_cast<PieceType>(pieceType), color, square);
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.attacks);
			}
		}
		bitBoard.attacks = attacks;
	}

	//gera um bitmap das casas alcançadas pela peça sem colocar o rei da mesma cor em xeque
	const void generateValidMoves(BitBoard& bitBoard, const PieceType pieceType, const Color color, const Square square) const {
		generateMoves(bitBoard, pieceType, color, square);
		tryToMakeMove(bitBoard, pieceType, color, square);
		tryToMakeCastle(bitBoard, pieceType, color);
	}

private:
	//retorna um bitmap das casas em ataque
	void generateMoves(BitBoard& bitBoard, PieceType pieceType, Color color, Square square) const {
		switch (pieceType) {
			case PAWN:
				bitBoard.attacks = pieceMoves::getPawnMoves(bitBoardOperations::getBitmapAllPieces(bitBoard), color, bitBoard.flags & (RANKS[RANK_4] | RANKS[RANK_5]), square);
				break;
			case KNIGHT:
				bitBoard.attacks = pieceMoves::getKnightMoves(square);
				break;
			case BISHOP:
				bitBoard.attacks = pieceMoves::getBishopMoves(bitBoardOperations::getBitmapAllPieces(bitBoard), square);
				break;
			case ROOK:
				bitBoard.attacks = pieceMoves::getRookMoves(bitBoardOperations::getBitmapAllPieces(bitBoard), square);
				break;
			case QUEEN: {
				const Bitmap allPiece = bitBoardOperations::getBitmapAllPieces(bitBoard);
				bitBoard.attacks = pieceMoves::getBishopMoves(allPiece, square) | pieceMoves::getRookMoves(allPiece, square);
				break;
			}
			case KING:
				bitBoard.attacks = pieceMoves::getKingMoves(square);
		}
		bitBoard.attacks = bitBoardOperations::unsetIntersections(bitBoard.attacks, bitBoardOperations::getBitmapAllPiecesByColor(bitBoard, color));
	}

	//tenta mover a peça sem colocar o rei em xeque
	void tryToMakeMove(BitBoard& bitBoard, PieceType pieceType, Color color, Square origin) const {
		Square destiny;
		for (Bitmap destinations = bitBoard.attacks; destinations != 0; destinations = bitBoardOperations::remainder(destinations)) {
			destiny = bitBoardOperations::getSquareOfBitmap(destinations);

			BitBoard clone(bitBoard);
			MoveMaker moveMaker(origin, destiny);
			moveMaker.makeMove(clone);

			generateMoves(clone, ~color, false);
			if (bitBoardOperations::isKingCheck(clone, color))
				bitBoard.attacks = bitBoardOperations::unsetIntersections(bitBoard.attacks, bitBoardOperations::getBitmapOfSquare(destiny));
		}
	}

	//tenta fazer o movimento do roque
	void tryToMakeCastle(BitBoard& bitBoard, PieceType pieceType, Color color) const {
		if (pieceType == KING) {
			Bitmap attacks = bitBoard.attacks;
			generateMoves(bitBoard, ~color, false);
			if (canMakeSmallCastle(bitBoard, color))
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.bitMaps[pieceType][color] << 2);
			if (canMakeBigCastle(bitBoard, color))
				attacks = bitBoardOperations::getUnion(attacks, bitBoard.bitMaps[pieceType][color] >> 2);
			bitBoard.attacks = attacks;
		}
	}

	//este metodo verifica se o rei pode fazer o castelo
	const bool canMakeSmallCastle(const BitBoard& bitBoard, Color color) const {
		if (bitBoardOperations::hasIntersection(bitBoard.flags, INITIAL_POSITION[KING][color])) {//wasKingNotMoved
			Square rookInitialSquare = color == WHITE ? H1 : H8;
			if (bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapOfSquare(rookInitialSquare))) {//wasSmallRookNotMoved
				const Bitmap relativeRank[2] = {RANKS[RANK_1], RANKS[RANK_8]};//isPathSmallRookClear
				const Bitmap allPieces = bitBoardOperations::getBitmapAllPieces(bitBoard);
				if (!bitBoardOperations::hasIntersection(bitBoardOperations::getIntersections(PATH_SMALL_ROOK, relativeRank[color]), allPieces))
					if (!bitBoardOperations::hasIntersection(bitBoardOperations::getIntersections(PATH_SMALL_ROOK, relativeRank[color]), bitBoard.attacks))
						return !bitBoardOperations::isKingCheck(bitBoard, color);
			}
		}
		return false;
	}

	//este metodo verifica se o rei pode fazer o castelo
	const bool canMakeBigCastle(const BitBoard& bitBoard, Color color) const {
		if (bitBoardOperations::hasIntersection(bitBoard.flags, INITIAL_POSITION[KING][color])) {//wasKingNotMoved
			Square rookInitialSquare = color == WHITE ? A1 : A8;
			if (bitBoardOperations::hasIntersection(bitBoard.flags, bitBoardOperations::getBitmapOfSquare(rookInitialSquare))) {//wasBigRookNotMoved
				const Bitmap relativeRank[2] = {RANKS[RANK_1], RANKS[RANK_8]};//isPathBigRookClear
				const Bitmap allPieces = bitBoardOperations::getBitmapAllPieces(bitBoard);
				if (!bitBoardOperations::hasIntersection(bitBoardOperations::getIntersections(PATH_BIG_ROOK, relativeRank[color]), allPieces))
					if (!bitBoardOperations::hasIntersection(bitBoardOperations::getIntersections(PATH_BIG_ROOK, relativeRank[color]), bitBoard.attacks))
						return !bitBoardOperations::isKingCheck(bitBoard, color);
			}
		}
		return false;
	}
};