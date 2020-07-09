#pragma once
class MoveMaker {
private:
	const uint64_t EN_PASSANT_RANK[2] = {bitBoardOperations::RANK_5, bitBoardOperations::RANK_4};
	const uint64_t PANW_PROMOTION_RANK[2] = {bitBoardOperations::RANK_1, bitBoardOperations::RANK_8};

	const Position& origin;
	const Position& destiny;
	std::optional<Piece> capture;
	bool enPassantCapture, pawnPromotion, smallRook, bigRook;

	void setCapture(const BitBoard& bitBoard, const uint8_t& destiny) {
		auto piece = bitBoardOperations::getPieceFromSquare(bitBoard, destiny);
		if (piece.has_value())
			capture.emplace(piece->name, piece->color);
	}

	//Este método verifica se o peão vai realizar uma captura em en passant
	void setEnPassantCapture(BitBoard& bitBoard, const Piece& piece, const uint64_t origin, const uint64_t destiny) {
		if (piece.name != Piece::NAME::PAWN)
			return;

		const uint64_t enPassant = bitBoardOperations::getIntersections(bitBoard.flags, EN_PASSANT_RANK[otherColor((uint8_t) piece.color)]);
		if (enPassant != 0) {
			if (piece.color == Piece::COLOR::WHITE) {
				if (bitBoardOperations::hasIntersection(origin << 1, enPassant))
					enPassantCapture = bitBoardOperations::hasIntersection(destiny >> 8, enPassant);
				else if (bitBoardOperations::hasIntersection(origin >> 1, enPassant))
					enPassantCapture = bitBoardOperations::hasIntersection(destiny >> 8, enPassant);
			} else {
				if (bitBoardOperations::hasIntersection(origin << 1, enPassant))
					enPassantCapture = bitBoardOperations::hasIntersection(destiny << 8, enPassant);
				else if (bitBoardOperations::hasIntersection(origin >> 1, enPassant))
					enPassantCapture = bitBoardOperations::hasIntersection(destiny << 8, enPassant);
			}

			if (enPassantCapture)
				bitBoardOperations::unsetPieceOnSquare(bitBoard, (uint8_t) piece.name, (uint8_t) otherColor((uint8_t) piece.color),
													   bitBoardOperations::getSquareFromBitmap(enPassant));
		}
	}

	/*
	 * Este método verifica se o peão move duas casas para frente onde ele se coloca
	 * na posição de enPassant.
	 */
	void setEnPassantMove(BitBoard& bitBoard, const Piece& piece, const uint64_t origin, const uint64_t destiny) {
		bitBoard.flags = bitBoardOperations::unsetIntersections(bitBoard.flags, EN_PASSANT_RANK[otherColor((uint8_t) piece.color)]);
		if (piece.name == Piece::NAME::PAWN)
			if (bitBoardOperations::hasIntersection(origin, bitBoardOperations::INITIAL_POSITION[(uint8_t) piece.name][(uint8_t) piece.color]))
				bitBoard.flags = bitBoardOperations::getUnion(bitBoard.flags, bitBoardOperations::getIntersections(destiny, EN_PASSANT_RANK[(uint8_t) piece.color]));
	}

	// Este método verifca se ocorreu uma promoção do peão
	void setPawnPromotion(const BitBoard& bitBoard, const Piece& piece, const uint64_t destiny) {
		if (piece.name == Piece::NAME::PAWN)
			pawnPromotion = bitBoardOperations::hasIntersection(destiny, PANW_PROMOTION_RANK[(uint8_t) piece.color]);
	}

	//Este método verifica se o rei vai mover duas casas para um lado e efetua os	movimento do roque
	void setSmallRook(BitBoard& bitBoard, const Piece& piece, const uint64_t origin, const uint64_t destiny) {
		if (piece.name == Piece::NAME::KING) {
			const uint64_t kingBitmap = bitBoardOperations::INITIAL_POSITION[(uint8_t) piece.name][(uint8_t) piece.color];
			smallRook = bitBoardOperations::hasIntersection(kingBitmap, origin)
				&& bitBoardOperations::hasIntersection(kingBitmap << 2, destiny);

			if (smallRook) {
				uint8_t square = bitBoardOperations::getSquareFromBitmap(kingBitmap << 3);
				bitBoardOperations::unsetPieceOnSquare(bitBoard, (uint8_t) Piece::NAME::ROOK, (uint8_t) piece.color, square);
				square = bitBoardOperations::getSquareFromBitmap(kingBitmap << 1);
				bitBoardOperations::setPieceOnSquare(bitBoard, (uint8_t) Piece::NAME::ROOK, (uint8_t) piece.color, square);
			}
		}
	}

	//Este método verifica se o rei vai mover duas casas para um lado e efetua os	movimento do roque
	void setBigRook(BitBoard& bitBoard, const Piece& piece, const uint64_t origin, const uint64_t destiny) {
		if (piece.name == Piece::NAME::KING) {
			const uint64_t kingBitmap = bitBoardOperations::INITIAL_POSITION[(uint8_t) piece.name][(uint8_t) piece.color];
			bigRook = bitBoardOperations::hasIntersection(kingBitmap, origin)
				&& bitBoardOperations::hasIntersection(kingBitmap >> 2, destiny);

			if (bigRook) {
				uint8_t square = bitBoardOperations::getSquareFromBitmap(kingBitmap >> 4);
				bitBoardOperations::unsetPieceOnSquare(bitBoard, (uint8_t) Piece::NAME::ROOK, (uint8_t) piece.color, square);
				square = bitBoardOperations::getSquareFromBitmap(kingBitmap >> 1);
				bitBoardOperations::setPieceOnSquare(bitBoard, (uint8_t) Piece::NAME::ROOK, (uint8_t) piece.color, square);
			}
		}
	}

public:
	explicit MoveMaker(const Position& origin, const Position& destiny) :
		origin(origin),
		destiny(destiny),
		capture(std::nullopt),
		enPassantCapture(false),
		pawnPromotion(enPassantCapture),
		smallRook(pawnPromotion),
		bigRook(smallRook) {}
	~MoveMaker() { capture.reset(); }

	void makeMove(BitBoard& bitBoard) {
		const uint64_t originBitmap = bitBoardOperations::getBitmapFromSquare(origin.getSquare());
		const uint64_t destinyBitmap = bitBoardOperations::getBitmapFromSquare(destiny.getSquare());
		std::optional<Piece> piece = bitBoardOperations::getPieceFromSquare(bitBoard, origin.getSquare());

		setCapture(bitBoard, destiny.getSquare());
		setEnPassantCapture(bitBoard, *piece, originBitmap, destinyBitmap);
		setEnPassantMove(bitBoard, *piece, originBitmap, destinyBitmap);
		setPawnPromotion(bitBoard, *piece, destinyBitmap);

		bitBoardOperations::unsetPieceOnSquare(bitBoard, (uint8_t) piece->name, (uint8_t) piece->color, origin.getSquare());
		if (capture.has_value())
			bitBoardOperations::unsetPieceOnSquare(bitBoard, (uint8_t) capture->name, (uint8_t) capture->color, destiny.getSquare());
		bitBoardOperations::setPieceOnSquare(bitBoard, (uint8_t) piece->name, (uint8_t) piece->color, destiny.getSquare());
		//bitBoard.flags = bitBoardOperations::unsetIntersections(bitBoard.flags, originBitmap);

		setSmallRook(bitBoard, *piece, originBitmap, destinyBitmap);
		setBigRook(bitBoard, *piece, originBitmap, destinyBitmap);
	}

	const Position& getOrigin() const {
		return origin;
	}

	const Position& getDestiny() const {
		return destiny;
	}

	const Piece& getCapture() const {
		return capture.value();
	}

	const bool& isEnPassantCapture() {
		return enPassantCapture;
	}

	const bool& isPawnPromotion() const {
		return pawnPromotion;
	}

	const bool& isSmallRook() const {
		return smallRook;
	}

	const bool& isBigRook() const {
		return bigRook;
	}

	uint8_t otherColor(const uint8_t& color) const {
		if (color == 0)
			return 1;
		return 0;
	}
};