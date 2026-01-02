#include "../lib/unity.h"
#include "../src/board.h"
#include "../src/debug_io.h"
#include "../src/movegen.h"

void test_set_up_board() {
   board brd = gen_start_board();
   bool equal = (brd.black_castle_history.size == 1) &&
                (brd.white_castle_history.size == 1) &&
                (brd.capture_history.size == 1);
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
   move mov = from_long_alg_single("e2e4\0");
   move expected1 = (e4 << 6) | e2;
   TEST_ASSERT_EQUAL_INT16(expected1, mov);
}

void test_flip_piece() {
   board brd = gen_start_board();
   flip_piece(&brd, KING, false, brd_from_pos("e1"));
   flip_piece(&brd, KING, false, brd_from_pos("a4"));

   flip_piece(&brd, BISHOP, true, brd_from_pos("c8"));
   flip_piece(&brd, BISHOP, true, brd_from_pos("h5"));
   board expect;
   from_fen("rn1qkbnr/pppppppp/8/7b/K7/8/PPPPPPPP/RNBQ1BNR w kq - 0 1",
            &expect);

   // print_board(&brd);
   // print_board(&expect);
   TEST_ASSERT(board_cmp_bb(&brd, &expect));
}

void test_castle_short() {
   board brd;
   from_fen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", &brd);
   TEST_ASSERT(brd.to_play == false);
   TEST_ASSERT_EQUAL_INT(
       BOTH_CASTLE,
       arr_get_castle_right4096(&brd.white_castle_history, brd.ply_count));

   // white castle
   move mov = move_from(e1, g1);
   bitboa from = from_square(from_move(mov));
   bitboa to = from_square(to_move(mov));
   piece pc = get_piece(&brd, from);

   // so everything aligns properly
   // brd.ply_count++;
   // bool castled = try_castle(&brd, pc, from, to);
   make_move(&brd, mov);

   // TEST_ASSERT_MESSAGE(castled, "Didn't castle");
   board expect;
   from_fen("r3k2r/8/8/8/8/8/8/R4RK1 b kq - 1 1", &expect);
   // print_board(&brd);
   // print_board(&expect);
   TEST_ASSERT(board_cmp_bb(&expect, &brd));
   TEST_ASSERT_EQUAL_INT(
       *arr_last_castle_right4096(&brd.white_castle_history),
       *arr_last_castle_right4096(&expect.white_castle_history));
   TEST_ASSERT_EQUAL_INT(
       *arr_last_castle_right4096(&brd.black_castle_history),
       *arr_last_castle_right4096(&expect.black_castle_history));

   // black castle
   move mov2 = move_from(e8, g8);
   from = from_square(from_move(mov2));
   to = from_square(to_move(mov2));
   pc = get_piece(&brd, from);

   // so everything aligns properly
   // brd.ply_count++;
   // bool castled2 = try_castle(&brd, pc, from, to);
   // TEST_ASSERT(castled2);
   make_move(&brd, mov2);

   board expect2;
   from_fen("r4rk1/8/8/8/8/8/8/R4RK1 b - - 1 1", &expect2);
   TEST_ASSERT(board_cmp_bb(&expect2, &brd));
   TEST_ASSERT_EQUAL_INT(
       *arr_last_castle_right4096(&brd.white_castle_history),
       *arr_last_castle_right4096(&expect2.white_castle_history));
   TEST_ASSERT_EQUAL_INT(
       *arr_last_castle_right4096(&brd.black_castle_history),
       *arr_last_castle_right4096(&expect2.black_castle_history));
}

void test_castle_long() {
   board brd;
   from_fen("r3k2r/8/8/8/8/8/8/R3K2R w Qkq - 1 1", &brd);
   TEST_ASSERT(brd.to_play == false);
   TEST_ASSERT_EQUAL_INT(
       LONG_CASTLE,
       arr_get_castle_right4096(&brd.white_castle_history, brd.ply_count));

   // white castle
   move mov = move_from(e1, c1);
   bitboa from = from_square(from_move(mov));
   bitboa to = from_square(to_move(mov));
   piece pc = get_piece(&brd, from);

   // so everything aligns properly
   // brd.ply_count++;
   // bool castled = try_castle(&brd, pc, from, to);
   // TEST_ASSERT(castled);
   make_move(&brd, mov);

   board expect;
   from_fen("r3k2r/8/8/8/8/8/8/2KR3R b kq - 1 1", &expect);
   TEST_ASSERT(board_cmp_bb(&expect, &brd));
   TEST_ASSERT_EQUAL_INT(
       *arr_last_castle_right4096(&brd.white_castle_history),
       *arr_last_castle_right4096(&expect.white_castle_history));
   TEST_ASSERT_EQUAL_INT(
       *arr_last_castle_right4096(&brd.black_castle_history),
       *arr_last_castle_right4096(&expect.black_castle_history));

   TEST_ASSERT(brd.to_play == true);
   TEST_ASSERT_EQUAL_INT(
       BOTH_CASTLE,
       arr_get_castle_right4096(&brd.black_castle_history, brd.ply_count));

   // black castle
   move mov2 = move_from(e8, c8);
   from = from_square(from_move(mov2));
   to = from_square(to_move(mov2));
   pc = get_piece(&brd, from);
   // printf("from %zu to %zu\n", from_move(mov2), to_move(mov2));
   // printf("from %zu to %zu\n", from, to);

   // print_board(&brd);
   // print_board(&expect);
   // so everything aligns properly
   // brd.ply_count++;
   // bool castled2 = try_castle(&brd, pc, from, to);
   // TEST_ASSERT(castled2);
   make_move(&brd, mov2);

   board expect2;
   from_fen("2kr3r/8/8/8/8/8/8/2KR3R b - - 1 1", &expect2);
   TEST_ASSERT(board_cmp_bb(&expect2, &brd));
   TEST_ASSERT_EQUAL_INT(
       *arr_last_castle_right4096(&brd.white_castle_history),
       *arr_last_castle_right4096(&expect2.white_castle_history));
   TEST_ASSERT_EQUAL_INT(
       *arr_last_castle_right4096(&brd.black_castle_history),
       *arr_last_castle_right4096(&expect2.black_castle_history));
}

void test_castle_checks() {
   {
      board brd;
      from_fen("r3k2r/8/8/8/8/8/8/R3N2R b Qkq - 1 1", &brd);

      // white castle
      move mov = move_from(e1, c1);
      // bitboa from = from_square(from_move(mov));
      // bitboa to = from_square(to_move(mov));
      // piece pc = get_piece(&brd, from);

      make_move(&brd, mov);
      board expect;
      from_fen("r3k2r/8/8/8/8/8/8/2KR3R b Qkq - 1 1", &expect);
      TEST_ASSERT_MESSAGE(!board_cmp_bb(&expect, &brd),
                          "King not present board is wrong.");
      TEST_ASSERT_EQUAL_INT(
          *arr_last_castle_right4096(&expect.white_castle_history),
          *arr_last_castle_right4096(&brd.white_castle_history));
      TEST_ASSERT_EQUAL_INT(
          *arr_last_castle_right4096(&expect.black_castle_history),
          *arr_last_castle_right4096(&brd.black_castle_history));
   }
   {
      board brd;
      from_fen("r3k2r/8/8/8/8/8/8/R3K2R b Kkq - 1 1", &brd);

      // white castle
      move mov = move_from(e1, c1);
      bitboa from = from_square(from_move(mov));
      bitboa to = from_square(to_move(mov));
      piece pc = get_piece(&brd, from);

      // so everything aligns properly
      brd.ply_count++;
      bool castled = try_castle(&brd, pc, from, to);
      TEST_ASSERT_MESSAGE(!castled, "Tried to castle long when forbidden.");
      board expect;
      from_fen("r3k2r/8/8/8/8/8/8/2KR3R b kq - 1 1", &expect);
      TEST_ASSERT(!board_cmp_bb(&expect, &brd));
   }
}

void test_capture() {
   board brd = gen_start_board();
   move mov = from_long_alg_single("e2d8");
   // printf("%d\n", mov.capture);
   // printf("%d\n", mov.pc);
   // printf("%llu\n", mov.to);
   TEST_ASSERT_MESSAGE(
       (comb_from_comp(&brd, false) & comb_from_comp(&brd, true)) == 0,
       "Starting board has overlapping pieces.");

   piece pc = get_piece(&brd, from_square(e2));
   make_move(&brd, mov);
   TEST_ASSERT_MESSAGE(arr_get_piece4096(&brd.capture_history, brd.ply_count) ==
                           QUEEN,
                       "Piece to capture wasn't queen");
   TEST_ASSERT_MESSAGE(pc == PAWN, "Piece to move wasn't pawn");

   // print_board(&brd);
   // print_board_internal(&brd);
   // ensure no pieces overlap between the players before/after.
   // pieces from the same player _can_ overlap though
   TEST_ASSERT_MESSAGE(
       (comb_from_comp(&brd, false) & comb_from_comp(&brd, true)) == 0,
       "Board after move has overlapping pieces.");
}

void test_promotion() {
   board brd = gen_start_board();
   move mov = from_long_alg_single("e2d8n");

   piece pc = get_piece(&brd, from_square(e2));
   TEST_ASSERT_MESSAGE(pc == PAWN, "Piece to move wasn't pawn");

   // ensure no pieces overlap between the players before/after.
   // pieces from the same player _can_ overlap though
   TEST_ASSERT_MESSAGE(
       (comb_from_comp(&brd, false) & comb_from_comp(&brd, true)) == 0,
       "Starting board has overlapping pieces.");
   make_move(&brd, mov);
   TEST_ASSERT_MESSAGE(arr_get_piece4096(&brd.capture_history, brd.ply_count) ==
                           QUEEN,
                       "Captured piece wasn't queen");
   TEST_ASSERT_MESSAGE(
       (comb_from_comp(&brd, false) & comb_from_comp(&brd, true)) == 0,
       "Board after move has overlapping pieces.");
   TEST_ASSERT_MESSAGE((brd.wkn_bb & brd_from_pos("d8")) != 0,
                       "Piece wasn't promoted to knight.");
   TEST_ASSERT_MESSAGE((brd.wpa_bb & brd_from_pos("d8")) == 0,
                       "Piece wasn't nuked from pawn.");
}

void test_en_passant() {
   board brd;
   from_fen("8/8/8/8/3p4/8/4P3/8 w - - 0 1", &brd);

   move mov1 = from_long_alg_single("e2e4");
   bitboa from1 = from_square(from_move(mov1));
   bitboa to1 = from_square(to_move(mov1));
   piece pc1 = get_piece(&brd, from1);

   move mov2 = from_long_alg_single("d4e3");
   bitboa from2 = from_square(from_move(mov2));
   bitboa to2 = from_square(to_move(mov2));

   TEST_ASSERT_MESSAGE(pc1 == PAWN, "Didn't move pawn");
   make_move(&brd, mov1);
   TEST_ASSERT_MESSAGE(arr_get_bitboa4096(&brd.enp_history, brd.ply_count) != 0,
                       "Board didn't record passantable");

   piece pc2 = get_piece(&brd, from2);
   TEST_ASSERT_MESSAGE(pc2 == PAWN, "Didn't move pawn");
   // TEST_ASSERT_MESSAGE(arr_get_bitboa4096(&brd.enp_history, brd.ply_count) ==
   // 0,
   //                     "Labled unpassantable passantable");
   make_move(&brd, mov2);
   TEST_ASSERT_MESSAGE(arr_get_bitboa4096(&brd.enp_history, brd.ply_count) == 0,
                       "Board didn't clear passantable");

   board expect;
   from_fen("8/8/8/8/8/4p3/8/8 w - - 0 1", &expect);
   TEST_ASSERT_MESSAGE(board_cmp_bb(&expect, &brd), "Final boards not equal");
}

void test_from_algebraic() {
   char input_str[] = "e2e4 d7d5 e4d5 g8f6 d1f3 c7c5 d5c6 d8b6 c6c7 c8d7 c7b8r "
                      "b6d8 f1e2 h7h5 g1h3 h5h4 e1g1\0";
   board brd = gen_start_board();
   from_long_algebraic(input_str, &brd);

   board expect = gen_start_board();
   from_fen("rR1qkb1r/pp1bppp1/5n2/8/7p/5Q1N/PPPPBPPP/RNB2RK1 b kq - 1 9",
            &expect);

   TEST_ASSERT_EQUAL_INT(
       arr_get_castle_right4096(&brd.white_castle_history, brd.ply_count),
       arr_get_castle_right4096(&expect.white_castle_history,
                                expect.ply_count));

   // print_board(&brd);
   // print_board(&expect);
   TEST_ASSERT(board_cmp_bb(&expect, &brd));
}

void test_undo_moves() {
   char input_str[] = "e2e4 d7d5 e4d5 g8f6 d1f3 c7c5 d5c6 d8b6 c6c7 c8d7 c7b8r "
                      "b6d8 f1e2 h7h5 g1h3 h5h4 e1g1\0";
   board brd = gen_start_board();
   from_long_algebraic(input_str, &brd);

   board expect = gen_start_board();
   // print_board_internal(&brd);
   // print_board_internal(&expect);
   while (brd.ply_count != 0) {
      undo_move(&brd, arr_get_move4096(&brd.move_history, brd.ply_count));
   }
   TEST_ASSERT(board_cmp_bb(&expect, &brd));
   TEST_ASSERT_EQUAL_INT_MESSAGE(expect.white_castle_history.size,
                                 brd.white_castle_history.size,
                                 "white_castle_history size differs.");
   TEST_ASSERT_EQUAL_INT_MESSAGE(expect.black_castle_history.size,
                                 brd.black_castle_history.size,
                                 "black_castle_history size differs.");
   TEST_ASSERT_EQUAL_INT_MESSAGE(expect.enp_history.size, brd.enp_history.size,
                                 "enp_history size differs.");
   TEST_ASSERT_EQUAL_INT_MESSAGE(expect.move50count.size, brd.move50count.size,
                                 "move50count size differs.");
   TEST_ASSERT_EQUAL_INT_MESSAGE(expect.capture_history.size,
                                 brd.capture_history.size,
                                 "capture_history size differs.");
   TEST_ASSERT_EQUAL_INT_MESSAGE(expect.move_history.size,
                                 brd.move_history.size,
                                 "move_history size differs.");
}

void test_undo_en_passant() {
   board brd;
   from_fen("8/8/8/8/3p4/8/4P3/8 w - - 0 1", &brd);

   move mov1 = from_long_alg_single("e2e4");
   bitboa from1 = from_square(from_move(mov1));
   bitboa to1 = from_square(to_move(mov1));
   piece pc1 = get_piece(&brd, from1);

   move mov2 = from_long_alg_single("d4e3");
   bitboa from2 = from_square(from_move(mov2));
   bitboa to2 = from_square(to_move(mov2));

   TEST_ASSERT_MESSAGE(pc1 == PAWN, "Didn't move pawn");
   make_move(&brd, mov1);
   TEST_ASSERT_MESSAGE(arr_get_bitboa4096(&brd.enp_history, brd.ply_count) != 0,
                       "Board didn't record passantable");

   piece pc2 = get_piece(&brd, from2);
   TEST_ASSERT_MESSAGE(pc2 == PAWN, "Didn't move pawn");
   // TEST_ASSERT_MESSAGE(arr_get_bitboa4096(&brd.enp_history, brd.ply_count) ==
   // 0,
   //                     "Labled unpassantable passantable");
   make_move(&brd, mov2);
   TEST_ASSERT_MESSAGE(arr_get_bitboa4096(&brd.enp_history, brd.ply_count) == 0,
                       "Board didn't clear passantable");

   board expect2;
   from_fen("8/8/8/8/8/4p3/8/8 w - - 0 1", &expect2);
   TEST_ASSERT_MESSAGE(board_cmp_bb(&expect2, &brd), "Final boards not equal");

   board expect;
   from_fen("8/8/8/8/3p4/8/4P3/8 w - - 0 1", &expect);

   undo_move(&brd, mov2);
   undo_move(&brd, mov1);

   TEST_ASSERT_MESSAGE(board_cmp_bb(&expect, &brd), "Final boards not equal");
}

void test_undo_castle_short() {
   board brd;
   from_fen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", &brd);
   board expect;
   from_fen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", &expect);

   // white castle
   move mov1 = move_from(e1, g1);

   // so everything aligns properly
   // brd.ply_count++;
   // bool castled = try_castle(&brd, pc1, from1, to1);
   // TEST_ASSERT(castled);
   make_move(&brd, mov1);
   board expect2 = brd;

   // black castle
   move mov2 = move_from(e8, g8);

   // so everything aligns properly
   // brd.ply_count++;
   // bool castled2 = try_castle(&brd, pc2, from2, to2);
   // TEST_ASSERT(castled2);
   make_move(&brd, mov2);
   undo_move(&brd, mov2);

   // print_board_internal(&brd);
   // print_board_internal(&expect2);
   TEST_ASSERT_MESSAGE(board_cmp_bb(&brd, &expect2), "first undo failed");
   undo_move(&brd, mov1);
   TEST_ASSERT_MESSAGE(board_cmp_bb(&brd, &expect), "second undo failed");
}

void test_zobrist_make_unmake() {
   board brd = gen_start_board();
   init_zobrist(&brd);
   zobrist start = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);
   move mov = move_from(e2, e4);
   // printf("hash? %lu\n", start);

   make_move(&brd, mov);
   // printf("hash? %lu\n", *arr_last_zobrist4096(&brd.zobrist_history));
   undo_move(&brd, mov);

   // for (int i = 0; i < 20; i++) {
   //    printf("%lu\n", zobrist_lookup.pieces[1][1][i]);
   // }

   zobrist end = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);
   // printf("hash? %lu\n", end);
   TEST_ASSERT_EQUAL(start, end);
}

void test_zobrist_capture() {
   board brd = gen_start_board();
   init_zobrist(&brd);
   zobrist start = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);
   move mov = move_from(e2, e7);

   make_move(&brd, mov);
   undo_move(&brd, mov);

   zobrist end = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);
   TEST_ASSERT_EQUAL(start, end);
}

void test_zobrist_undo() {
   board brd = gen_start_board();
   init_zobrist(&brd);
   zobrist start = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);

   char input_str[] = "e2e4 d7d5 e4d5 g8f6 d1f3 c7c5 d5c6 d8b6 c6c7 c8d7 c7b8r "
                      "b6d8 f1e2 h7h5 g1h3 h5h4 e1g1";
   from_long_algebraic(input_str, &brd);

   while (brd.move_history.size > 1) {
      undo_move(&brd, arr_get_move4096(&brd.move_history, brd.ply_count));
   }

   zobrist end = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);
   // printf("%llu %llu\n", start, end);
   TEST_ASSERT_EQUAL(start, end);
}

void test_zobrist_init() {
   board brd = gen_start_board();
   init_zobrist(&brd);
   // zobrist start = zobrist_state;

   char input_str[] = "e2e4 d7d5 e4d5 g8f6 d1f3 c7c5 d5c6 d8b6 c6c7 c8d7 c7b8r "
                      "b6d8 f1e2 h7h5 g1h3 h5h4 e1g1";
   from_long_algebraic(input_str, &brd);
   zobrist end = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);
   from_fen("rR1qkb1r/pp1bppp1/5n2/8/7p/5Q1N/PPPPBPPP/RNB2RK1 b kq - 1 9",
            &brd);
   zobrist end2 = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);

   // printf("%llu %llu\n", end, end2);
   TEST_ASSERT_EQUAL(end, end2);
}

void test_zobrist_init_no_castle() {
   board brd = gen_start_board();
   init_zobrist(&brd);
   // zobrist start = zobrist_state;

   char input_str[] = "e2e4 d7d5 e4d5 g8f6 d1f3 c7c5 d5c6 d8b6 c6c7 c8d7 c7b8r "
                      "b6d8 f1e2 h7h5 g1h3 h5h4";
   from_long_algebraic(input_str, &brd);
   zobrist end = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);

   from_fen("rR1qkb1r/pp1bppp1/5n2/8/7p/5Q1N/PPPPBPPP/RNB1K2R w KQkq - 0 9",
            &brd);
   zobrist end2 = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);

   // printf("%llu %llu\n", end, end2);
   TEST_ASSERT_EQUAL(end, end2);
}

void test_zobrist_init_one_move() {
   board brd = gen_start_board();

   char input_str[] = "e2e4";
   from_long_algebraic(input_str, &brd);
   zobrist end = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);

   zobrist zobrist_state = 0;
   // en passant file ?
   bitboa enp = arr_get_bitboa4096(&brd.enp_history, brd.ply_count);
   if (enp != 0) {
      zobrist_state = zobrist_lookup.enp_file[bitscan_lsb(enp) % 8];
   }

   from_fen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",
            &brd);
   zobrist end2 = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);

   TEST_ASSERT_EQUAL(end2, end);
}

void test_zobrist_init_capture() {
   board brd = gen_start_board();

   char input_str[] = "b1c3 d7d5 c3d5";
   from_long_algebraic(input_str, &brd);
   zobrist end = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);

   from_fen("rnbqkbnr/ppp1pppp/8/3N4/8/8/PPPPPPPP/R1BQKBNR b KQkq - 0 2", &brd);
   zobrist end2 = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);

   TEST_ASSERT_EQUAL(end2, end);
}

void test_zobrist_init_en_passant() {
   board brd = gen_start_board();

   char input_str[] = "e2e4 b7b6 e4e5 d7d5 e5d6";
   from_long_algebraic(input_str, &brd);
   zobrist end = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);

   from_fen("rnbqkbnr/p1p1pppp/1p1P4/8/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 3",
            &brd);
   zobrist end2 = arr_get_zobrist4096(&brd.zobrist_history, brd.ply_count);

   TEST_ASSERT_EQUAL(end2, end);
}

int run_test_board() {
   generate_tables();
   setup_zobrist();

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

   // zobrist hashing
   RUN_TEST(test_zobrist_make_unmake);
   RUN_TEST(test_zobrist_capture);
   RUN_TEST(test_zobrist_undo);
   RUN_TEST(test_zobrist_init);
   RUN_TEST(test_zobrist_init_no_castle);
   RUN_TEST(test_zobrist_init_one_move);
   RUN_TEST(test_zobrist_init_capture);
   RUN_TEST(test_zobrist_init_en_passant);
   return UNITY_END();
}
