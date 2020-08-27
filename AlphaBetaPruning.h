#pragma once
#include "MoveGenerator.h"

class AlphaBetaPruning {
public:
	AlphaBetaPruning() = delete;
	explicit AlphaBetaPruning(Color color, short depth) : COLOR(color), DEPTH(depth), running(false), nodes(0), moves() {
		//heuristic = new Heuristic();
	}

	void search(BitBoard& bitBoard) {
		if (running)
			return;
		running = true;

		Square origin, destiny;
		int value = -999;
		for (uChar pieceType = 0; pieceType < 6; pieceType++) {
			for (Bitmap pieceBitmap = bitBoard.bitMaps[pieceType][COLOR]; pieceBitmap != 0; pieceBitmap = bitBoardOperations::remainder(pieceBitmap)) {
				origin = bitBoardOperations::getSquareOfBitmap(pieceBitmap);
				if (moves.hasPossibleMoves(bitBoard, (PieceType)pieceType, COLOR, origin))
					for (Bitmap destinations = bitBoard.attacks; destinations != 0; destinations = bitBoardOperations::remainder(destinations)) {
						destiny = bitBoardOperations::getSquareOfBitmap(destinations);

						BitBoard clone(bitBoard);
						MoveMaker moveMaker(origin, destiny);
						moveMaker.makeMove(clone);
						//bitBoardOperations::printBitmap("BitBoard:", bitBoardOperations::getBitmapAllPieces(clone));
						//bitBoardOperations::printBitmap("Flags:", bitBoard.flags);
						//bitBoardOperations::printBitmap("Attacks:", bitBoard.attacks);

						//if (moveMaker.isPawnPromotion())
							//heuristic.PawnPromotion(clone, color, destiny);

						int aux = alphaBetaPruning(clone, DEPTH - 1, -999, 999, false);
						if (aux > value) {
							bitBoard.move = clone.move;
							value = aux;
						}
						std::cout << (uInt)origin << '|' << (uInt)destiny << ": " << nodes << std::endl;
						nodes = 0;
					}
			}
		}

		running = false;
	}

	const bool isRunning() { return running; }
	const unsigned int getNodes() { return nodes; }

private:
	const Color COLOR;
	const short DEPTH;
	bool running;
	unsigned int nodes;
	MoveGenerator moves;
	//Heuristic heuristic;

	int alphaBetaPruning(BitBoard& bitBoard, short depth, int alpha, int beta, bool isMax) {
		if (depth == 0)
			return 2000; //return heuristic value;

		nodes++;
		Square origin, destiny;
		bool cutOff = false;
		int value;

		if (isMax) {
			value = -999;
			for (uChar pieceType = 0; pieceType < 6 && cutOff == false; pieceType++) {
				for (Bitmap pieceBitmap = bitBoard.bitMaps[pieceType][COLOR]; pieceBitmap != 0 && cutOff == false; pieceBitmap = bitBoardOperations::remainder(pieceBitmap)) {
					origin = bitBoardOperations::getSquareOfBitmap(pieceBitmap);
					if (moves.hasPossibleMoves(bitBoard, (PieceType)pieceType, COLOR, origin))
						for (Bitmap destinations = bitBoard.attacks; destinations != 0 && cutOff == false; destinations = bitBoardOperations::remainder(destinations)) {
							destiny = bitBoardOperations::getSquareOfBitmap(destinations);

							BitBoard clone(bitBoard);
							MoveMaker moveMaker(origin, destiny);
							moveMaker.makeMove(clone);
							//bitBoardOperations::printBitmap("BitBoard:", bitBoardOperations::getBitmapAllPieces(clone));
							//bitBoardOperations::printBitmap("Flags:", bitBoard.flags);
							//bitBoardOperations::printBitmap("Attacks:", bitBoard.attacks);

							//if (moveMaker.isPawnPromotion())
								//heuristic.PawnPromotion(clone, color, destiny);

							value = max(value, alphaBetaPruning(clone, depth - 1, alpha, beta, false));
							alpha = max(alpha, value);
							if (alpha >= beta)
								cutOff = true;
						}
				}
			}
		}
		else {
			value = 999;
			for (uChar pieceType = 0; pieceType < 6 && cutOff == false; pieceType++) {
				for (Bitmap pieceBitmap = bitBoard.bitMaps[pieceType][~COLOR]; pieceBitmap != 0 && cutOff == false; pieceBitmap = bitBoardOperations::remainder(pieceBitmap)) {
					origin = bitBoardOperations::getSquareOfBitmap(pieceBitmap);
					if (moves.hasPossibleMoves(bitBoard, (PieceType)pieceType, ~COLOR, origin))
						for (Bitmap destinations = bitBoard.attacks; destinations != 0 && cutOff == false; destinations = bitBoardOperations::remainder(destinations)) {
							destiny = bitBoardOperations::getSquareOfBitmap(destinations);

							BitBoard clone(bitBoard);
							MoveMaker moveMaker(origin, destiny);
							moveMaker.makeMove(clone);
							//bitBoardOperations::printBitmap("BitBoard:", bitBoardOperations::getBitmapAllPieces(bitBoard));
							//bitBoardOperations::printBitmap("Flags:", bitBoard.flags);
							//bitBoardOperations::printBitmap("Attacks:", bitBoard.attacks);

							//if (moveMaker.isPawnPromotion())
								//heuristic.PawnPromotion(clone, color, destiny);

							value = min(value, alphaBetaPruning(clone, depth - 1, alpha, beta, true));
							beta = min(beta, value);
							if (beta <= alpha)
								cutOff = true;
						}
				}
			}
		}
		return value;
	}
};