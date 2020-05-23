#ifndef EVALUATOR_H_INCLUDED
#define EVALUATOR_H_INCLUDED

	/* Este método verifica se o rei foi movido */
	const bool wasKingMoved(const BitBoard& bitBoard, const uint8_t& colorKing) {
        /*
		if (!bitBoard.isPositionOccupied(Constants.INITIAL_POSITION[Piece.KING][colorKing],
				bitBoard.getBitmapPiece(Piece.KING, colorKing))) {
			bitBoard.flags = bitBoard.unsetBitsOnBitmap(bitBoard.flags,
					Constants.INITIAL_POSITION[Piece.KING][colorKing]);
			return true;
		}*/
		return false;
	}

	/* Este método verifica se a torre do rei foi movida */
	const bool wasSmallRookMoved(const BitBoard& bitBoard, const uint8_t& colorRook) {		
        /*
		if (!bitBoard.isPositionOccupied(initialPosition, bitBoard.getBitmapPiece(Piece.ROOK, colorRook))) {
			bitBoard.flags = bitBoard.unsetBitsOnBitmap(bitBoard.flags, initialPosition);
			return true;
		}*/
		return false;
	}

	/* Este método verifica se as casas do castelo do rei estão livres */
	const bool isPathSmallRookClear(const BitBoard& bitBoard, const uint8_t& color) {
		const uint64_t allPieces = bitBoard.getBitmapAllPieces();
		const uint64_t kingBitmap = constants::INITIAL_POSITION[piece::KING][color];
		if (!bitBoard.isPositionOccupied(bitBoard.getAttacks(), kingBitmap << 1))
			if (!bitBoard.isPositionOccupied(allPieces, kingBitmap << 1))
				if (!bitBoard.isPositionOccupied(bitBoard.getAttacks(), kingBitmap << 2))
					return (!bitBoard.isPositionOccupied(allPieces, kingBitmap << 2));
		return false;
	}

	/* Este método verifica se a torre da dame foi movida */
	const bool wasBigRookMoved(const BitBoard& bitBoard, const uint8_t& colorRook) {		
        /*
		if (!bitBoard.isPositionOccupied(initialPosition, bitBoard.getBitmapPiece(Piece.ROOK, colorRook))) {
			bitBoard.flags = bitBoard.unsetBitsOnBitmap(bitBoard.flags, initialPosition);
			return true;
		}*/
		return false;
	}

	
#endif // EVALUATOR_H_INCLUDED
