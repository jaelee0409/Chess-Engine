#pragma once

#include <stdio.h>
#include <string.h>

#define flipBit(bb, sq) ((bb) ^= (1ULL << (sq)))
#define setBit(bb, sq) ((bb) |= (1ULL << (sq)))
#define getBit(bb, sq) ((bb) & (1ULL << (sq)))
#define popBit(bb, sq) (getBit((bb), (sq)) ? flipBit((bb), (sq)) : 0)
#define countBits(bb) __popcnt64(bb)
#define getLSB(i, bb) _BitScanForward64(&i, bb)

#define startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

typedef unsigned long long U64;

static U64 pawnAttackTable[2][64];
static U64 knightAttackTable[64];
static U64 bishopAttackMask[64];
static U64 bishopAttackTable[64][512];
static U64 rookAttackMask[64];
static U64 rookAttackTable[64][4096];
static U64 kingAttackTable[64];

class Board
{
public:
    const U64 notAfile = 0xFEFEFEFEFEFEFEFE;
    const U64 notHfile = 0x7F7F7F7F7F7F7F7F;
    const U64 notABfile = 0xFCFCFCFCFCFCFCFC;
    const U64 notGHfile = 0x3F3F3F3F3F3F3F3F;

    const U64 bishopMagicNumbers[64] = {
        0x40040844404084ULL,
        0x2004208a004208ULL,
        0x10190041080202ULL,
        0x108060845042010ULL,
        0x581104180800210ULL,
        0x2112080446200010ULL,
        0x1080820820060210ULL,
        0x3c0808410220200ULL,
        0x4050404440404ULL,
        0x21001420088ULL,
        0x24d0080801082102ULL,
        0x1020a0a020400ULL,
        0x40308200402ULL,
        0x4011002100800ULL,
        0x401484104104005ULL,
        0x801010402020200ULL,
        0x400210c3880100ULL,
        0x404022024108200ULL,
        0x810018200204102ULL,
        0x4002801a02003ULL,
        0x85040820080400ULL,
        0x810102c808880400ULL,
        0xe900410884800ULL,
        0x8002020480840102ULL,
        0x220200865090201ULL,
        0x2010100a02021202ULL,
        0x152048408022401ULL,
        0x20080002081110ULL,
        0x4001001021004000ULL,
        0x800040400a011002ULL,
        0xe4004081011002ULL,
        0x1c004001012080ULL,
        0x8004200962a00220ULL,
        0x8422100208500202ULL,
        0x2000402200300c08ULL,
        0x8646020080080080ULL,
        0x80020a0200100808ULL,
        0x2010004880111000ULL,
        0x623000a080011400ULL,
        0x42008c0340209202ULL,
        0x209188240001000ULL,
        0x400408a884001800ULL,
        0x110400a6080400ULL,
        0x1840060a44020800ULL,
        0x90080104000041ULL,
        0x201011000808101ULL,
        0x1a2208080504f080ULL,
        0x8012020600211212ULL,
        0x500861011240000ULL,
        0x180806108200800ULL,
        0x4000020e01040044ULL,
        0x300000261044000aULL,
        0x802241102020002ULL,
        0x20906061210001ULL,
        0x5a84841004010310ULL,
        0x4010801011c04ULL,
        0xa010109502200ULL,
        0x4a02012000ULL,
        0x500201010098b028ULL,
        0x8040002811040900ULL,
        0x28000010020204ULL,
        0x6000020202d0240ULL,
        0x8918844842082200ULL,
        0x4010011029020020ULL
    };
    const U64 rookMagicNumbers[64] = {
        0x8a80104000800020ULL,
        0x140002000100040ULL,
        0x2801880a0017001ULL,
        0x100081001000420ULL,
        0x200020010080420ULL,
        0x3001c0002010008ULL,
        0x8480008002000100ULL,
        0x2080088004402900ULL,
        0x800098204000ULL,
        0x2024401000200040ULL,
        0x100802000801000ULL,
        0x120800800801000ULL,
        0x208808088000400ULL,
        0x2802200800400ULL,
        0x2200800100020080ULL,
        0x801000060821100ULL,
        0x80044006422000ULL,
        0x100808020004000ULL,
        0x12108a0010204200ULL,
        0x140848010000802ULL,
        0x481828014002800ULL,
        0x8094004002004100ULL,
        0x4010040010010802ULL,
        0x20008806104ULL,
        0x100400080208000ULL,
        0x2040002120081000ULL,
        0x21200680100081ULL,
        0x20100080080080ULL,
        0x2000a00200410ULL,
        0x20080800400ULL,
        0x80088400100102ULL,
        0x80004600042881ULL,
        0x4040008040800020ULL,
        0x440003000200801ULL,
        0x4200011004500ULL,
        0x188020010100100ULL,
        0x14800401802800ULL,
        0x2080040080800200ULL,
        0x124080204001001ULL,
        0x200046502000484ULL,
        0x480400080088020ULL,
        0x1000422010034000ULL,
        0x30200100110040ULL,
        0x100021010009ULL,
        0x2002080100110004ULL,
        0x202008004008002ULL,
        0x20020004010100ULL,
        0x2048440040820001ULL,
        0x101002200408200ULL,
        0x40802000401080ULL,
        0x4008142004410100ULL,
        0x2060820c0120200ULL,
        0x1001004080100ULL,
        0x20c020080040080ULL,
        0x2935610830022400ULL,
        0x44440041009200ULL,
        0x280001040802101ULL,
        0x2100190040002085ULL,
        0x80c0084100102001ULL,
        0x4024081001000421ULL,
        0x20030a0244872ULL,
        0x12001008414402ULL,
        0x2006104900a0804ULL,
        0x1004081002402ULL
    };

    const int bishopRelevantBitCount[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6,
    };

    const int rookRelevantBitCount[64] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12,
    };

    enum BoardSquares {
        a8, b8, c8, d8, e8, f8, g8, h8,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a1, b1, c1, d1, e1, f1, g1, h1,
        noSquare = -1
    };
    enum PieceColors {
        white,
        black,
        both
    };
    enum PieceTypes
    {
        whitePawn,
        blackPawn,
        whiteKnight,
        blackKnight,
        whiteBishop,
        blackBishop,
        whiteRook,
        blackRook,
        whiteQueen,
        blackQueen,
        whiteKing,
        blackKing
    };
    enum CastleFlags
    {
        none = 0,
        whiteKingSide = 1,
        whiteQueenSide = 2,
        blackKingSide = 4,
        blackQueenSide = 8
    };

    Board()
    {
        printf("Board Constructor\n");

        // initialize initial board state
        resetBoard();

        // initial pawns
        setWhitePawns(a2);
        setWhitePawns(b2);
        setWhitePawns(c2);
        setWhitePawns(d2);
        setWhitePawns(e2);
        setWhitePawns(f2);
        setWhitePawns(g2);
        setWhitePawns(h2);
        setBlackPawns(a7);
        setBlackPawns(b7);
        setBlackPawns(c7);
        setBlackPawns(d7);
        setBlackPawns(e7);
        setBlackPawns(f7);
        setBlackPawns(g7);
        setBlackPawns(h7);

        // initial knights
        setWhiteKnights(b1);
        setWhiteKnights(g1);
        setBlackKnights(b8);
        setBlackKnights(g8);

        // initial bishops
        setWhiteBishops(c1);
        setWhiteBishops(f1);
        setBlackBishops(c8);
        setBlackBishops(f8);

        // initial rook
        setWhiteRooks(a1);
        setWhiteRooks(h1);
        setBlackRooks(a8);
        setBlackRooks(h8);

        // initial queen
        setWhiteQueens(d1);
        setBlackQueens(d8);

        // initial king
        setWhiteKing(e1);
        setBlackKing(e8);

        updateOccupiedBitboards();
        updateEmptyBitboards();

        for (int square = 0; square < 64; ++square)
        {
            pawnAttackTable[white][square] = getPawnAttackBitboard(white, square);
            pawnAttackTable[black][square] = getPawnAttackBitboard(black, square);
            knightAttackTable[square] = getKnightAttackBitboard(square);
            kingAttackTable[square] = getKingAttackBitboard(square);
        }

        //initMagicNumbers();
        initSliderAttacks(0);
        initSliderAttacks(1);
    }

    // piece possible attacks methods
    U64 getPawnAttackBitboard(int side, int square);
    U64 getPawnMoveBitboard(int side, int square);
    U64 getKnightAttackBitboard(int square);

    U64 getBishopMaskBitboard(int square);
    U64 getBishopAttackBitboardRuntime(U64 board, int square);
    U64 getBishopAttackBitboard(U64 occ, int square);

    U64 getRookMaskBitboard(int square);
    U64 getRookAttackBitboardRuntime(U64 board, int square);
    U64 getRookAttackBitboard(U64 occ, int square);

    U64 getQueenAttackBitboard(U64 occ, int square);
    U64 getKingAttackBitboard(int square);
    void initSliderAttacks(bool isBishop);

    // move generator methods
    bool isSquareAttacked(int side, int square);

    // magic number methods
    unsigned int getRandomU32();
    U64 getRandomU64();
    U64 generateMagicNumber();
    U64 findMagicNumber(int square, int relevantBits, bool isBishop);
    void initMagicNumbers();

    U64 setOccupancy(U64 maskBitboard, int index, int maskBitCount);
    void printBitboard(U64 bitboard);

    // getters
    U64 getEmptyBitboard() const { return m_emptyBitboard; };
    U64 getOccupiedBitboard(int side) const { return m_occupiedBitboard[side]; };

    U64 getPieceBitboard(int pt) const { return m_pieces[pt]; };
    U64 getWhitePawns() const { return m_pieces[whitePawn]; }
    U64 getBlackPawns() const { return m_pieces[blackPawn]; }
    U64 getWhiteKnights() const { return m_pieces[whiteKnight]; }
    U64 getBlackKnights() const { return m_pieces[blackKnight]; }
    U64 getWhiteBishops() const { return m_pieces[whiteBishop]; }
    U64 getBlackBishops() const { return m_pieces[blackBishop]; }
    U64 getWhiteRooks() const { return m_pieces[whiteRook]; }
    U64 getBlackRooks() const { return m_pieces[blackRook]; }
    U64 getWhiteQueens() const { return m_pieces[whiteQueen]; }
    U64 getBlackQueens() const { return m_pieces[blackQueen]; }
    U64 getWhiteKing() const { return m_pieces[whiteKing]; }
    U64 getBlackKing() const { return m_pieces[blackKing]; }
    int getSide() const { return m_side; }
    int getEnPassantSquare() const { return m_enPassant; }
    int getCastlingRights() const { return m_castle; }

    // setters
    void setEmptyBitboard(int square) { setBit(m_emptyBitboard, square); };
    void updateEmptyBitboards() { m_emptyBitboard = ~m_occupiedBitboard[both]; };
    void resetEmptyBitboards() { m_emptyBitboard = 0ULL; }
    void addOccupiedBitboard(int side, U64 bitboard) { m_occupiedBitboard[side] |= bitboard; };
    void setOccupiedBitboardSquare(int side, int square) { setBit(m_occupiedBitboard[side], square); };
    void clearOccupiedBitboardSquare(int side, int square) { popBit(m_occupiedBitboard[side], square); };
    void updateOccupiedBitboards();
    void resetOccupiedBitboards();
    void resetBoard();

    void setPieceBitboard(Board::PieceTypes pt, int square) { setBit(m_pieces[pt], square); };
    //void popPieceBitboard(int square) { for (int i = 0; i < 12; ++i) popBit(m_pieces[i], square); };
    void popPieceBitboard(Board::PieceTypes pt, int square) { popBit(m_pieces[pt], square); };
    void setWhitePawns(int square) { setBit(m_pieces[whitePawn], square); }
    void setBlackPawns(int square) { setBit(m_pieces[blackPawn], square); }
    void setWhiteKnights(int square) { setBit(m_pieces[whiteKnight], square); }
    void setBlackKnights(int square) { setBit(m_pieces[blackKnight], square); }
    void setWhiteBishops(int square) { setBit(m_pieces[whiteBishop], square); }
    void setBlackBishops(int square) { setBit(m_pieces[blackBishop], square); }
    void setWhiteRooks(int square) { setBit(m_pieces[whiteRook], square); }
    void setBlackRooks(int square) { setBit(m_pieces[blackRook], square); }
    void setWhiteQueens(int square) { setBit(m_pieces[whiteQueen], square); }
    void setBlackQueens(int square) { setBit(m_pieces[blackQueen], square); }
    void setWhiteKing(int square) { setBit(m_pieces[whiteKing], square); }
    void setBlackKing(int square) { setBit(m_pieces[blackKing], square); }
    void setSide(bool side) { m_side = side; }
    void setEnPassantSquare(int square) { m_enPassant = square; }
    void setCastlingRights(int flags) { m_castle = flags; }

    void flipSide() { m_side = !m_side; setEnPassantSquare(noSquare); }

private:
    unsigned int m_random = 1804289383;
    U64 m_pieces[12];
    U64 m_emptyBitboard;
    U64 m_occupiedBitboard[3];
    bool m_side;
    int m_enPassant;
    int m_castle;
};
