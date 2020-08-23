#pragma once
#include "MoveGenerator.h"
class MinMax {
private:
	const short DEPTH;
	unsigned int nodes;
	MoveGenerator moves;
	//Heuristic heuristic;

public:
	MinMax(short depth) : DEPTH(depth), nodes(0), moves() {
		//heuristic = new Heuristic();
	}

	void search(BitBoard& bitBoard, const uChar color) {
		uChar origin, destiny;
		double score = -9999;
		Tbitmap bestMove = 0;

		for (uChar namePiece = 0; namePiece < 6; namePiece++) {
			for (Tbitmap pieceBitmap = bitBoard.bitMaps[namePiece][color]; pieceBitmap != 0; pieceBitmap = bitBoardOperations::remainder(pieceBitmap)) {
				origin = bitBoardOperations::getSquareFromBitmap(pieceBitmap);
				if (moves.hasPossibleMoves(bitBoard, namePiece, color, origin))
					for (Tbitmap destinations = bitBoard.attacks; destinations != 0; destinations = bitBoardOperations::remainder(destinations)) {
						destiny = bitBoardOperations::getSquareFromBitmap(destinations);

						BitBoard clone(bitBoard);
						MoveMaker moveMaker(origin, destiny);
						//std::cout << (uint32_t)origin << "|" << (uint32_t)destiny << std::endl;
						moveMaker.makeMove(clone);
						bitBoardOperations::printBitmap("BitBoard:", bitBoardOperations::allPieces(bitBoard));
						bitBoardOperations::printBitmap("Flags:", bitBoard.flags);
						bitBoardOperations::printBitmap("Attacks:", bitBoard.attacks);

						//if (moveMaker.isPawnPromotion())
							//heuristic.PawnPromotion(clone, color, destiny);

						double value = alphaBeta(clone, bitBoardOperations::getOtherColor(color), DEPTH - 1, -10000, +10000, false);
						if (value >= score) {
							score = value;
							bestMove = clone.move;
						}
					}
			}
		}

		//std::cout << "Nodos: " << nodes << std::endl;
		bitBoard.move = bestMove;
	}

private:
	int alphaBeta(BitBoard& bitBoard, uChar color, short depth, double alpha, double beta, bool isMax) {
		nodes++;
		if (depth <= 0 || !moves.hasPossibleMoves(bitBoard, color))
			return 0;
		//return -heuristic.getScore(bitBoard, color);

		uChar origin, destiny;
		int score = isMax ? -9999 : +9999;

		bool breakLoop = false;
		for (uChar namePiece = 0; namePiece < 6 && breakLoop; namePiece++) {
			for (Tbitmap pieceBitmap = bitBoard.bitMaps[namePiece][color]; pieceBitmap != 0 && breakLoop; pieceBitmap = bitBoardOperations::remainder(pieceBitmap)) {
				origin = bitBoardOperations::getSquareFromBitmap(pieceBitmap);
				if (moves.hasPossibleMoves(bitBoard, namePiece, color, origin))
					for (Tbitmap destinations = bitBoard.attacks; destinations != 0; destinations = bitBoardOperations::remainder(destinations)) {
						destiny = bitBoardOperations::getSquareFromBitmap(destinations);

						BitBoard clone(bitBoard);
						MoveMaker moveMaker(origin, destiny);
						moveMaker.makeMove(clone);

						//if (moveMaker.isPawnPromotion())
							//heuristic.PawnPromotion(clone, color, destiny);

						if (isMax) {
							score = std::max(score,
											 alphaBeta(clone, bitBoardOperations::getOtherColor(color), depth - 1, alpha, beta, !isMax));
							alpha = std::max(alpha, score);
						}
						else {
							score = std::min(score,
											 alphaBeta(clone, bitBoardOperations::getOtherColor(color), depth - 1, alpha, beta, !isMax));
							beta = std::min(beta, score);
						}
						breakLoop = beta <= alpha;
					}
			}
		}
		return score;
	}
};