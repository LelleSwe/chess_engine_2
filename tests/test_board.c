#include "../lib/unity.h"
#include "../src/board.h"
#include "../src/debug_io.h"

// void test_algebraic() {
//    char input_str[] = "e2e3 e1e2 e2e3\0";
//    board _board = from_long_algebraic(input_str);
//
//    print_board(&_board);
// }

void test_set_up_board() {
   board brd = gen_start_board();
   bool equal = board_cmp(&default_start_board, &brd);
   TEST_ASSERT(equal);
}

void test_get_piece_start_board() {
   board brd = gen_start_board();
   piece pc1 = get_piece(&brd, 0x8000);
   TEST_ASSERT_EQUAL_INT(PAWN, pc1);
   piece pc2 = get_piece(&brd, 0x8000000);
   TEST_ASSERT_EQUAL_INT(NO_PIECE, pc2);
   piece pc3 = get_piece(&brd, 0x800000000000000);
   TEST_ASSERT_EQUAL_INT(KING, pc3);
}

void test_get_move_from_single_string() {
   board brd = gen_start_board();
   move mov = {};
   from_long_alg_single(&mov, &brd, "e2e4", false);
   move expected1 = {0x800UL,     0x8000000UL, PAWN, NO_PIECE,
                     BOTH_CASTLE, false,       false};
   TEST_ASSERT(move_cmp(&expected1, &mov));
}

void test_flip_piece() {
   board brd = gen_start_board();
   flip_piece(&brd, KING, false, brd_from_pos("e1"));
   flip_piece(&brd, KING, false, brd_from_pos("a4"));

   flip_piece(&brd, BISHOP, true, brd_from_pos("c8"));
   flip_piece(&brd, BISHOP, true, brd_from_pos("h5"));
   const board expect = {0x000000000000FF00UL,
                         0x0000000000000024UL,
                         0x0000000000000081UL,
                         0x0000000000000010UL,
                         0x0000000080000000UL,
                         0x0000000000000042UL,
                         0x000000008000FFF7UL,
                         0x00FF000000000000UL,
                         0x0400000100000000UL,
                         0x8100000000000000UL,
                         0x1000000000000000UL,
                         0x0800000000000000UL,
                         0x4200000000000000UL,
                         0xDFFF000100000000UL,
                         3,
                         3,
                         0,
                         0};
   TEST_ASSERT(board_cmp(&brd, &expect));
}

void test_castle_short() {
   board brd = {0,
                0,
                brd_from_pos("a1") | brd_from_pos("h1"),
                0,
                brd_from_pos("e1"),
                0,
                0,
                0,
                0,
                brd_from_pos("a8") | brd_from_pos("h8"),
                0,
                brd_from_pos("e8"),
                0,
                0,
                BOTH_CASTLE,
                BOTH_CASTLE,
                0,
                0};
   brd.wcb_bb = comb_from_comp(&brd, false);
   brd.bcb_bb = comb_from_comp(&brd, true);

   // white castle
   move mov = {brd_from_pos("e1"),
               brd_from_pos("g1"),
               KING,
               NO_PIECE,
               BOTH_CASTLE,
               false,
               false};

   bool castled = try_castle(&brd, &mov);
   TEST_ASSERT(castled);
   board expect = {0,
                   0,
                   brd_from_pos("a1") | brd_from_pos("f1"),
                   0,
                   brd_from_pos("g1"),
                   0,
                   0,
                   0,
                   0,
                   brd_from_pos("a8") | brd_from_pos("h8"),
                   0,
                   brd_from_pos("e8"),
                   0,
                   0,
                   NO_CASTLE,
                   BOTH_CASTLE,
                   0,
                   0};
   expect.wcb_bb = comb_from_comp(&expect, false);
   expect.bcb_bb = comb_from_comp(&expect, true);
   TEST_ASSERT(board_cmp(&expect, &brd));

   // black castle
   move mov2 = {brd_from_pos("e8"),
                brd_from_pos("g8"),
                KING,
                NO_PIECE,
                SHORT_CASTLE,
                true,
                false};
   bool castled2 = try_castle(&brd, &mov2);
   TEST_ASSERT(castled2);
   board expect2 = {0,
                    0,
                    brd_from_pos("a1") | brd_from_pos("f1"),
                    0,
                    brd_from_pos("g1"),
                    0,
                    0,
                    0,
                    0,
                    brd_from_pos("a8") | brd_from_pos("f8"),
                    0,
                    brd_from_pos("g8"),
                    0,
                    0,
                    NO_CASTLE,
                    NO_CASTLE,
                    0,
                    0};
   expect2.wcb_bb = comb_from_comp(&expect2, false);
   expect2.bcb_bb = comb_from_comp(&expect2, true);
   TEST_ASSERT(board_cmp(&expect2, &brd));
}

void test_castle_long() {
   board brd = {0,
                0,
                brd_from_pos("a1") | brd_from_pos("h1"),
                0,
                brd_from_pos("e1"),
                0,
                0,
                0,
                0,
                brd_from_pos("a8") | brd_from_pos("h8"),
                0,
                brd_from_pos("e8"),
                0,
                0,
                LONG_CASTLE,
                BOTH_CASTLE,
                0,
                0};
   brd.wcb_bb = comb_from_comp(&brd, false);
   brd.bcb_bb = comb_from_comp(&brd, true);

   // white castle
   move mov = {brd_from_pos("e1"),
               brd_from_pos("c1"),
               KING,
               NO_PIECE,
               BOTH_CASTLE,
               false,
               false};

   bool castled = try_castle(&brd, &mov);
   TEST_ASSERT(castled);
   board expect = {0,
                   0,
                   brd_from_pos("d1") | brd_from_pos("h1"),
                   0,
                   brd_from_pos("c1"),
                   0,
                   0,
                   0,
                   0,
                   brd_from_pos("a8") | brd_from_pos("h8"),
                   0,
                   brd_from_pos("e8"),
                   0,
                   0,
                   NO_CASTLE,
                   BOTH_CASTLE,
                   0,
                   0};
   expect.wcb_bb = comb_from_comp(&expect, false);
   expect.bcb_bb = comb_from_comp(&expect, true);
   TEST_ASSERT(board_cmp(&expect, &brd));

   // black castle
   move mov2 = {brd_from_pos("e8"),
                brd_from_pos("c8"),
                KING,
                NO_PIECE,
                BOTH_CASTLE,
                true,
                false};
   bool castled2 = try_castle(&brd, &mov2);
   TEST_ASSERT(castled2);
   board expect2 = {0,
                    0,
                    brd_from_pos("d1") | brd_from_pos("h1"),
                    0,
                    brd_from_pos("c1"),
                    0,
                    0,
                    0,
                    0,
                    brd_from_pos("d8") | brd_from_pos("h8"),
                    0,
                    brd_from_pos("c8"),
                    0,
                    0,
                    NO_CASTLE,
                    NO_CASTLE,
                    0,
                    0};
   expect2.wcb_bb = comb_from_comp(&expect2, false);
   expect2.bcb_bb = comb_from_comp(&expect2, true);
   TEST_ASSERT(board_cmp(&expect2, &brd));
}

void test_castle_checks() {
   {
      board brd = {0,
                   0,
                   brd_from_pos("a1") | brd_from_pos("h1"),
                   0,
                   0,
                   brd_from_pos("e1"),
                   0,
                   0,
                   0,
                   brd_from_pos("a8") | brd_from_pos("h8"),
                   0,
                   brd_from_pos("e8"),
                   0,
                   0,
                   LONG_CASTLE,
                   BOTH_CASTLE,
                   0,
                   0};
      brd.wcb_bb = comb_from_comp(&brd, false);
      brd.bcb_bb = comb_from_comp(&brd, true);

      // white castle
      move mov = {brd_from_pos("e1"),
                  brd_from_pos("c1"),
                  KNIGHT,
                  NO_PIECE,
                  BOTH_CASTLE,
                  false,
                  false};

      bool castled = try_castle(&brd, &mov);
      TEST_ASSERT_MESSAGE(!castled, "Castled even though king wasn't present.");
      board expect = {0,
                      0,
                      brd_from_pos("d1") | brd_from_pos("h1"),
                      0,
                      brd_from_pos("c1"),
                      0,
                      0,
                      0,
                      0,
                      brd_from_pos("a8") | brd_from_pos("h8"),
                      0,
                      brd_from_pos("e8"),
                      0,
                      0,
                      NO_CASTLE,
                      BOTH_CASTLE,
                      0,
                      0};
      expect.wcb_bb = comb_from_comp(&expect, false);
      expect.bcb_bb = comb_from_comp(&expect, true);
      TEST_ASSERT_MESSAGE(!board_cmp(&expect, &brd),
                          "King not present board is wrong.");
   }
   {
      board brd = {0,
                   0,
                   brd_from_pos("a1") | brd_from_pos("h1"),
                   0,
                   brd_from_pos("e1"),
                   0,
                   0,
                   0,
                   0,
                   brd_from_pos("a8") | brd_from_pos("h8"),
                   0,
                   brd_from_pos("e8"),
                   0,
                   0,
                   SHORT_CASTLE,
                   BOTH_CASTLE,
                   0,
                   0};
      brd.wcb_bb = comb_from_comp(&brd, false);
      brd.bcb_bb = comb_from_comp(&brd, true);

      // white castle
      move mov = {brd_from_pos("e1"),
                  brd_from_pos("c1"),
                  KING,
                  NO_PIECE,
                  SHORT_CASTLE,
                  false,
                  false};

      bool castled = try_castle(&brd, &mov);
      TEST_ASSERT_MESSAGE(!castled, "Tried to castle long when forbidden.");
      board expect = {0,
                      0,
                      brd_from_pos("d1") | brd_from_pos("h1"),
                      0,
                      brd_from_pos("c1"),
                      0,
                      0,
                      0,
                      0,
                      brd_from_pos("a8") | brd_from_pos("h8"),
                      0,
                      brd_from_pos("e8"),
                      0,
                      0,
                      NO_CASTLE,
                      BOTH_CASTLE,
                      0,
                      0};
      expect.wcb_bb = comb_from_comp(&expect, false);
      expect.bcb_bb = comb_from_comp(&expect, true);
      TEST_ASSERT(!board_cmp(&expect, &brd));
   }
}

int run_test_board() {
   UNITY_BEGIN();
   RUN_TEST(test_set_up_board);
   RUN_TEST(test_get_piece_start_board);
   RUN_TEST(test_get_move_from_single_string);
   RUN_TEST(test_flip_piece);
   RUN_TEST(test_castle_short);
   RUN_TEST(test_castle_long);
   RUN_TEST(test_castle_checks);
   return UNITY_END();
}
