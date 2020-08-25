#pragma once
#include "Types.h"

class History {
private:

public:
	History() {}
	~History() {}

	Bitmap generatePositionKey(BitBoard& bitBoard) {
		Bitmap key = 0;
		for (uChar pieceType = 0; pieceType < 6; pieceType++)
			for (uChar color = 0; color < 2; color++) 
				key ^= bitBoard.bitMaps[pieceType][color];

		if (bitBoard.whiteTime) 
			key ^= WHITE;		
		else 
			key ^= BLACK;
		
		if (bitBoard.flags & (RANKS[RANK_4] | RANKS[RANK_5]) != 0)
			key ^= bitBoard.flags & (RANKS[RANK_4] | RANKS[RANK_5]);

			//key prar o Catelo.

		return key;
	}

	void push(BitBoard& bitBoard) {

	}

	void pop() {

	}
};