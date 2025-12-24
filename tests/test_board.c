#include "../lib/unity.h"
#include "../src/board.h"
#include "../src/debug_io.h"

void test_set_up_board() {
   board brd = gen_start_board();
   bool equal = board_cmp(&default_start_board, &brd);
   TEST_ASSERT(equal);
}

void test_get_piece_start_board() {
   board brd = gen_start_board();
   piece pc1 = get_piece(&brd, brd_from_pos("d2"));
   TEST_ASSERT_EQUAL_INT(PAWN, pc1);
   piece pc2 = get_piece(&brd, brd_from_pos("h6"));
   TEST_ASSERT_EQUAL_INT(NO_PIECE, pc2);
   piece pc3 = get_piece(&brd, brd_from_pos("e8"));
   TEST_ASSERT_EQUAL_INT(KING, pc3);
}

void test_get_move_from_single_string() {
   board brd = gen_start_board();
   move mov = {};
   from_long_alg_single(&mov, &brd, "e2e4", false);
   move expected1 = {.from = brd_from_pos("e2"),
                     .to = brd_from_pos("e4"),
                     .pc = PAWN,
                     .capture = NO_PIECE,
                     .promotion = NO_PIECE,
                     .castle = BOTH_CASTLE,
                     .to_play = false,
                     .en_passant = brd_from_pos("e4"),
                     .prev_en_passant = 0};
   TEST_ASSERT(move_cmp(&expected1, &mov));
}

void test_flip_piece() {
   board brd = gen_start_board();
   flip_piece(&brd, KING, false, brd_from_pos("e1"));
   flip_piece(&brd, KING, false, brd_from_pos("a4"));

   flip_piece(&brd, BISHOP, true, brd_from_pos("c8"));
   flip_piece(&brd, BISHOP, true, brd_from_pos("h5"));
   board expect = {0x000000000000FF00UL,
                   0x0000000000000024UL,
                   0x0000000000000081UL,
                   0x0000000000000008UL,
                   0x0000000001000000UL,
                   0x0000000000000042UL,
                   0x000000000100FFF7UL,
                   0x00FF000000000000UL,
                   0x2000008000000000UL,
                   0x8100000000000000UL,
                   0x0800000000000000UL,
                   0x1000000000000000UL,
                   0x4200000000000000UL,
                   0xDFFF008000000000UL,
                   3,
                   3,
                   0};
   expect.bcb_bb = comb_from_comp(&expect, true);
   expect.wcb_bb = comb_from_comp(&expect, false);
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
                0};
   brd.wcb_bb = comb_from_comp(&brd, false);
   brd.bcb_bb = comb_from_comp(&brd, true);

   // white castle
   move mov = {brd_from_pos("e1"),
               brd_from_pos("g1"),
               KING,
               NO_PIECE,
               NO_PIECE,
               BOTH_CASTLE,
               false,
               0,
               .prev_en_passant = 0};

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
                   0};
   expect.wcb_bb = comb_from_comp(&expect, false);
   expect.bcb_bb = comb_from_comp(&expect, true);
   TEST_ASSERT(board_cmp(&expect, &brd));

   // black castle
   move mov2 = {brd_from_pos("e8"),
                brd_from_pos("g8"),
                KING,
                NO_PIECE,
                NO_PIECE,
                SHORT_CASTLE,
                true,
                0,
                .prev_en_passant = 0};
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
                0};
   brd.wcb_bb = comb_from_comp(&brd, false);
   brd.bcb_bb = comb_from_comp(&brd, true);

   // white castle
   move mov = {brd_from_pos("e1"),
               brd_from_pos("c1"),
               KING,
               NO_PIECE,
               NO_PIECE,
               BOTH_CASTLE,
               false,
               0,
               .prev_en_passant = 0};

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
                   0};
   expect.wcb_bb = comb_from_comp(&expect, false);
   expect.bcb_bb = comb_from_comp(&expect, true);
   TEST_ASSERT(board_cmp(&expect, &brd));

   // black castle
   move mov2 = {brd_from_pos("e8"),
                brd_from_pos("c8"),
                KING,
                NO_PIECE,
                NO_PIECE,
                BOTH_CASTLE,
                true,
                false,
                0};
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
                   0};
      brd.wcb_bb = comb_from_comp(&brd, false);
      brd.bcb_bb = comb_from_comp(&brd, true);

      // white castle
      move mov = {brd_from_pos("e1"),
                  brd_from_pos("c1"),
                  KNIGHT,
                  NO_PIECE,
                  NO_PIECE,
                  BOTH_CASTLE,
                  false,
                  0,
                  .prev_en_passant = 0};

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
                   0};
      brd.wcb_bb = comb_from_comp(&brd, false);
      brd.bcb_bb = comb_from_comp(&brd, true);

      // white castle
      move mov = {brd_from_pos("e1"),
                  brd_from_pos("c1"),
                  KING,
                  NO_PIECE,
                  NO_PIECE,
                  SHORT_CASTLE,
                  false,
                  0,
                  .prev_en_passant = 0};

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
                      0};
      expect.wcb_bb = comb_from_comp(&expect, false);
      expect.bcb_bb = comb_from_comp(&expect, true);
      TEST_ASSERT(!board_cmp(&expect, &brd));
   }
}

void test_capture() {
   board brd = gen_start_board();
   move mov;
   from_long_alg_single(&mov, &brd, "e2d8", false);
   // printf("%d\n", mov.capture);
   // printf("%d\n", mov.pc);
   // printf("%llu\n", mov.to);
   TEST_ASSERT_MESSAGE(mov.capture == QUEEN, "Piece to capture wasn't queen");
   TEST_ASSERT_MESSAGE(mov.pc == PAWN, "Piece to move wasn't pawn");

   // ensure no pieces overlap between the players before/after.
   // pieces from the same player _can_ overlap though
   TEST_ASSERT_MESSAGE(
       (comb_from_comp(&brd, false) & comb_from_comp(&brd, true)) == 0,
       "Starting board has overlapping pieces.");
   make_move(&brd, &mov);
   TEST_ASSERT_MESSAGE(
       (comb_from_comp(&brd, false) & comb_from_comp(&brd, true)) == 0,
       "Board after move has overlapping pieces.");
}

void test_promotion() {
   board brd = gen_start_board();
   move mov;
   from_long_alg_single(&mov, &brd, "e2d8n", false);

   TEST_ASSERT_MESSAGE(mov.capture == QUEEN, "Piece to capture wasn't queen");
   TEST_ASSERT_MESSAGE(mov.pc == PAWN, "Piece to move wasn't pawn");

   // ensure no pieces overlap between the players before/after.
   // pieces from the same player _can_ overlap though
   TEST_ASSERT_MESSAGE(
       (comb_from_comp(&brd, false) & comb_from_comp(&brd, true)) == 0,
       "Starting board has overlapping pieces.");
   make_move(&brd, &mov);
   TEST_ASSERT_MESSAGE(
       (comb_from_comp(&brd, false) & comb_from_comp(&brd, true)) == 0,
       "Board after move has overlapping pieces.");
   TEST_ASSERT_MESSAGE((brd.wkn_bb & brd_from_pos("d8")) != 0,
                       "Piece wasn't promoted to knight.");
   TEST_ASSERT_MESSAGE((brd.wpa_bb & brd_from_pos("d8")) == 0,
                       "Piece wasn't nuked from pawn.");
}

void test_en_passant() {
   board brd = {brd_from_pos("e2"),
                0,
                0,
                0,
                0,
                0,
                0,
                brd_from_pos("d4"),
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0};
   brd.wcb_bb = comb_from_comp(&brd, false);
   brd.bcb_bb = comb_from_comp(&brd, true);

   move mov1;
   move mov2;

   from_long_alg_single(&mov1, &brd, "e2e4", false);
   TEST_ASSERT_MESSAGE(mov1.pc == PAWN, "Didn't move pawn");
   TEST_ASSERT_MESSAGE(mov1.en_passant != 0, "Didn't catch passantable");
   make_move(&brd, &mov1);
   TEST_ASSERT_MESSAGE(brd.enp != 0, "Board didn't record passantable");

   from_long_alg_single(&mov2, &brd, "d4e3", true);
   TEST_ASSERT_MESSAGE(mov2.pc == PAWN, "Didn't move pawn");
   TEST_ASSERT_MESSAGE(mov2.en_passant == 0,
                       "Labled unpassantable passantable");
   make_move(&brd, &mov2);
   TEST_ASSERT_MESSAGE(brd.enp == 0, "Board didn't clear passantable");

   board expect = {0, 0, 0, 0, 0, 0, 0, brd_from_pos("e3"), 0, 0,
                   0, 0, 0, 0, 0, 0, 0};
   expect.bcb_bb = comb_from_comp(&expect, true);
   TEST_ASSERT_MESSAGE(board_cmp(&expect, &brd), "Final boards not equal");
}

void test_from_algebraic() {
   vec_move moves = new_vec_move(200);
   char input_str[] = "e2e4 d7d5 e4d5 g8f6 d1f3 c7c5 d5c6 d8b6 c6c7 c8d7 c7b8r "
                      "h7h6 f1e2 h6h5 g1h3 h5h4 e1g1\0";
   board brd;
   from_long_algebraic(input_str, &brd, &moves);

   board expect = gen_start_board();
   // setup pawns
   flip_piece(&expect, PAWN, true, brd_from_pos("c7"));
   flip_piece(&expect, PAWN, true, brd_from_pos("h7"));
   flip_piece(&expect, PAWN, true, brd_from_pos("d7"));
   flip_piece(&expect, PAWN, true, brd_from_pos("h4"));
   flip_piece(&expect, PAWN, false, brd_from_pos("e2"));
   // setup queens
   flip_piece(&expect, QUEEN, true, brd_from_pos("d8"));
   flip_piece(&expect, QUEEN, true, brd_from_pos("b6"));
   flip_piece(&expect, QUEEN, false, brd_from_pos("d1"));
   flip_piece(&expect, QUEEN, false, brd_from_pos("f3"));
   // setup knights
   flip_piece(&expect, KNIGHT, true, brd_from_pos("b8"));
   flip_piece(&expect, KNIGHT, true, brd_from_pos("g8"));
   flip_piece(&expect, KNIGHT, true, brd_from_pos("f6"));
   flip_piece(&expect, KNIGHT, false, brd_from_pos("g1"));
   flip_piece(&expect, KNIGHT, false, brd_from_pos("h3"));
   // setup rooks
   flip_piece(&expect, ROOK, false, brd_from_pos("b8"));
   flip_piece(&expect, ROOK, false, brd_from_pos("h1"));
   flip_piece(&expect, ROOK, false, brd_from_pos("f1"));
   // setup bishops
   flip_piece(&expect, BISHOP, false, brd_from_pos("f1"));
   flip_piece(&expect, BISHOP, false, brd_from_pos("e2"));
   flip_piece(&expect, BISHOP, true, brd_from_pos("c8"));
   flip_piece(&expect, BISHOP, true, brd_from_pos("d7"));
   // setup kings
   flip_piece(&expect, KING, false, brd_from_pos("e1"));
   flip_piece(&expect, KING, false, brd_from_pos("g1"));
   expect.white_castle = 0;

   TEST_ASSERT(board_cmp(&expect, &brd));
}

DECLARE_VEC(board)
DEFINE_VEC(board)
void test_undo_moves() {
   vec_move moves = new_vec_move(200);
   char input_str[] = "e2e4 d7d5 e4d5 g8f6 d1f3 c7c5 d5c6 d8b6 c6c7 c8d7 c7b8r "
                      "h7h6 f1e2 h6h5 g1h3 h5h4 e1g1\0";
   board brd;
   from_long_algebraic(input_str, &brd, &moves);
   vec_board boards = new_vec_board(200);
   vec_push_board(&boards, gen_start_board());
   for (size_t i = 0; i < moves.size; i++) {
      board prev_brd = vec_get_board(&boards, i);
      move prev_mov = vec_get_move(&moves, i);
      make_move(&prev_brd, &prev_mov);
      vec_push_board(&boards, prev_brd);
   }

   // printf("Move %zu:\n", boards.size - 1);
   // board last = vec_get_board(&boards, boards.size - 1);
   // print_board(&brd);
   // print_board(&last);
   // print_board_internal(&brd);
   // print_board_internal(&last);

   board expect = gen_start_board();
   for (size_t i = moves.size - 1; i < SIZE_MAX; i--) {
      // move mov = vec_get_move(&moves, i);
      // print_mov_internal(&mov);

      // printf("Move %zu:\n", i);
      // board prev_brd = vec_get_board(&boards, i);
      pop_move(&brd, &moves);
      // print_board(&brd);
      // print_board(&prev_brd);
      // print_board_internal(&brd);
      // print_board_internal(&prev_brd);
   }

   // print_board_internal(&brd);
   // print_board_internal(&expect);
   TEST_ASSERT(board_cmp(&expect, &brd));
}

void test_undo_en_passant() {
   board brd = {brd_from_pos("e2"),
                0,
                0,
                0,
                0,
                0,
                0,
                brd_from_pos("d4"),
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0};
   brd.wcb_bb = comb_from_comp(&brd, false);
   brd.bcb_bb = comb_from_comp(&brd, true);

   move mov1;
   move mov2;

   from_long_alg_single(&mov1, &brd, "e2e4", false);
   TEST_ASSERT_MESSAGE(mov1.pc == PAWN, "Didn't move pawn");
   TEST_ASSERT_MESSAGE(mov1.en_passant != 0, "Didn't catch passantable");
   // print_board(&brd);
   make_move(&brd, &mov1);
   TEST_ASSERT_MESSAGE(brd.enp != 0, "Board didn't record passantable");
   // print_board(&brd);

   from_long_alg_single(&mov2, &brd, "d4e3", true);
   TEST_ASSERT_MESSAGE(mov2.pc == PAWN, "Didn't move pawn");
   TEST_ASSERT_MESSAGE(mov2.en_passant == 0,
                       "Labled unpassantable passantable");
   make_move(&brd, &mov2);
   TEST_ASSERT_MESSAGE(brd.enp == 0, "Board didn't clear passantable");
   // print_board(&brd);

   board expect = {brd_from_pos("e2"),
                   0,
                   0,
                   0,
                   0,
                   0,
                   0,
                   brd_from_pos("d4"),
                   0,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0};
   expect.bcb_bb = comb_from_comp(&expect, true);
   expect.wcb_bb = comb_from_comp(&expect, false);
   undo_move(&brd, &mov2);
   // print_board(&brd);
   undo_move(&brd, &mov1);
   // print_board(&brd);

   // print_board_internal(&brd);
   // print_board_internal(&expect);
   TEST_ASSERT_MESSAGE(board_cmp(&expect, &brd), "Final boards not equal");
}

void test_undo_castle_short() {
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
                0};
   brd.wcb_bb = comb_from_comp(&brd, false);
   brd.bcb_bb = comb_from_comp(&brd, true);
   board expect = brd;

   // white castle
   move mov = {brd_from_pos("e1"),
               brd_from_pos("g1"),
               KING,
               NO_PIECE,
               NO_PIECE,
               BOTH_CASTLE,
               false,
               0,
               .prev_en_passant = 0};

   bool castled = try_castle(&brd, &mov);
   TEST_ASSERT(castled);
   board expect2 = brd;

   // black castle
   move mov2 = {brd_from_pos("e8"),
                brd_from_pos("g8"),
                KING,
                NO_PIECE,
                NO_PIECE,
                BOTH_CASTLE,
                true,
                0,
                .prev_en_passant = 0};
   bool castled2 = try_castle(&brd, &mov2);
   TEST_ASSERT(castled2);
   try_undo_castle(&brd, &mov2);
   // print_board_internal(&brd);
   // print_board_internal(&expect2);
   TEST_ASSERT_MESSAGE(board_cmp(&brd, &expect2), "first undo failed");
   try_undo_castle(&brd, &mov);
   TEST_ASSERT_MESSAGE(board_cmp(&brd, &expect), "second undo failed");
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
   RUN_TEST(test_capture);
   RUN_TEST(test_promotion);
   RUN_TEST(test_en_passant);
   RUN_TEST(test_from_algebraic);
   RUN_TEST(test_undo_en_passant);
   RUN_TEST(test_undo_castle_short);
   RUN_TEST(test_undo_moves);
   return UNITY_END();
}
