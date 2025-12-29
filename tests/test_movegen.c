#include "../lib/unity.h"
#include "../src/debug_io.h"
#include "../src/movegen.h"

void test_in_check() {
   board brd;
   from_fen("rnbq1bnr/pppppppp/8/8/8/3k4/PPPPPPPP/RNBQKBNR w KQ - 0 1", &brd);

   TEST_ASSERT(in_check(&brd, true));
   TEST_ASSERT(!in_check(&brd, false));
}

void test_ong_idk() {
   board brd;
   from_fen(
       "r3k2r/p1ppqpb1/bn2pnp1/3PN3/Pp2P3/2N2Q1p/1PPBBPPP/R3K2R b KQkq a3 0 1",
       &brd);

   // print_bitboard(brd.enp_history.data[brd.ply_count]);
   // FILE *debugout = fopen("www.txt", "w");
   // int count = perft_debug(debugout, &brd, 1);
   // fclose(debugout);
   int count = perft(&brd, 1);
   TEST_ASSERT_EQUAL(44, count);
}

void test_ong_idk2() {
   board brd;
   from_fen("8/8/8/8/8/8/6k1/4K1R1 b - - 1 1", &brd);

   // print_bitboard(brd.enp_history.data[brd.ply_count]);
   // FILE *debugout = fopen("www.txt", "w");
   // int count = perft_debug(debugout, &brd, 1);
   // fclose(debugout);
   int count = perft(&brd, 1);
   TEST_ASSERT_EQUAL(4, count);
}

void test_some_kind_of_passant_bug() {
   board brd;
   from_fen("rnbqkbnr/1ppppppp/8/p7/7P/8/PPPPPPP1/RNBQKBNR w KQkq a6 0 2",
            &brd);
   arr_move256 move_list = new_arr_move256();
   add_pawn_moves(&brd, &move_list);
   bitboa tot = from_move_list(&move_list);

   // print_bitboard(tot);
   // printf("%lu", tot);
   //
   // print_bitboard(brd.enp_history.data[brd.ply_count]);
   TEST_ASSERT_EQUAL(551894843392ULL, tot);
}

void test_king_blocked_castle() {
   board brd;
   from_fen(
       "r3k2r/p1ppqpb1/bnN1pnp1/3P4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 1 1",
       &brd);

   // print_bitboard(brd.enp_history.data[brd.ply_count]);
   // FILE *debugout = fopen("www.txt", "w");
   // int count = perft_debug(debugout, &brd, 1);
   // fclose(debugout);
   int count = perft(&brd, 1);
   TEST_ASSERT_EQUAL(41, count);
}

void test_king_single() {
   board brd;
   from_fen("8/8/8/8/8/4B3/3K4/3p4 w - - 0 1", &brd);

   square sq = d2;
   bitboa occ = brd.wcb_bb | brd.bcb_bb;

   // for (square t = 0; t < 64; t++) {
   //    printf("%zu\n", king_moves[t]);
   // }

   bitboa bishop = get_king_move(&brd, sq);
   // print_bitboard(bishop);
   // printf("%lu", bishop);
   TEST_ASSERT_EQUAL(791580ULL, bishop);
}

void test_king_rook_top() {
   board brd;
   from_fen("r3k3/1K6/8/8/8/8/8/8 w q - 0 1", &brd);
   move mov = move_from(b7, a8);

   make_move(&brd, mov);

   bitboa king = get_king_move(&brd, e8);
   // print_bitboard(king);
   // printf("%zu\n", king);
   // printf("black castle_right? %d\n",
   //        arr_get_castle_right4096(&brd.black_castle_history,
   //        brd.ply_count));
   TEST_ASSERT_EQUAL(2898066360212914176ULL, king);
}

void test_bishop_single() {
   board brd;
   from_fen("4r3/8/2B5/8/R7/8/8/8 w - - 0 1", &brd);

   square sq = c6;
   bitboa occ = brd.wcb_bb | brd.bcb_bb;

   bitboa bishop = get_bishop_move(&brd, c6);
   // print_bitboard(bishop);
   // printf("%lu", bishop);
   TEST_ASSERT_EQUAL(1227793891632103552ULL, bishop);
}

void test_knight_single() {
   board brd;
   from_fen("4r3/4R3/2N5/4b3/R7/8/8/8 w - - 0 1", &brd);

   bitboa knight = get_knight_move(&brd, c6);
   // printf("bb %zu\n", knight);
   // print_bitboard(knight);
   TEST_ASSERT_EQUAL(0xa0100110a000000ULL, knight);
}

void test_rook_single() {
   board brd;
   from_fen("8/8/4B3/8/8/2Q1R1rr/8/4n3 w - - 0 1", &brd);

   bitboa rook = get_rook_move(&brd, e3);
   // printf("%lu\n", rook);
   // print_bitboard(rook);
   TEST_ASSERT_EQUAL(68994732048ULL, rook);
}

void test_rook_count_single() {
   board brd;
   from_fen("8/8/4B3/8/8/2Q1R1rr/8/4n3 w - - 0 1", &brd);
   arr_move256 move_list = new_arr_move256();
   add_rook_moves(&brd, &move_list);

   // printf("%zu\n", move_list.size);
   TEST_ASSERT_EQUAL(7, move_list.size);
}

void test_queen_single() {
   board brd;
   from_fen("8/8/1N2B3/2k3N1/8/2Q1q1rr/8/4n3 b - - 0 1", &brd);

   bitboa queen = get_queen_move(&brd, e3);
   // printf("%lu\n", queen);
   // print_bitboard(queen);
   TEST_ASSERT_EQUAL(17936725850180ULL, queen);
}

void test_queen_count_single() {
   board brd;
   from_fen("8/8/1N2B3/2k3N1/8/2Q1q1rr/8/4n3 b - - 0 1", &brd);
   arr_move256 move_list = new_arr_move256();
   add_queen_moves(&brd, &move_list);

   TEST_ASSERT_EQUAL(14, move_list.size);
}

void test_pawn_push_single() {
   {
      board brd;
      from_fen("8/5p2/8/8/8/8/3P4/8 w - - 0 1", &brd);
      arr_move256 move_list = new_arr_move256();
      add_pawn_moves(&brd, &move_list);

      bitboa tot = from_move_list(&move_list);
      // print_bitboard(tot);
      // printf("%lu\n", tot);
      TEST_ASSERT_EQUAL(2, move_list.size);
      TEST_ASSERT_EQUAL(134742016ULL, tot);
   }
   {
      board brd;
      from_fen("8/5p2/8/8/8/8/3P4/8 b - - 0 1", &brd);
      arr_move256 move_list = new_arr_move256();
      add_pawn_moves(&brd, &move_list);

      bitboa tot = from_move_list(&move_list);
      // print_bitboard(tot);
      // printf("%lu\n", tot);
      TEST_ASSERT_EQUAL(2, move_list.size);
      TEST_ASSERT_EQUAL(35321811042304ULL, tot);
   }
   {
      board brd;
      from_fen("8/8/8/3P4/5p2/8/8/8 w - - 0 1", &brd);
      arr_move256 move_list = new_arr_move256();
      add_pawn_moves(&brd, &move_list);
      bitboa tot = from_move_list(&move_list);

      // print_bitboard(tot);
      // printf("%lu\n", tot);
      TEST_ASSERT_EQUAL(1, move_list.size);
      TEST_ASSERT_EQUAL(8796093022208ULL, tot);
   }
   {
      board brd;
      from_fen("8/8/8/3P4/5p2/8/8/8 b - - 0 1", &brd);
      arr_move256 move_list = new_arr_move256();
      add_pawn_moves(&brd, &move_list);
      bitboa tot = from_move_list(&move_list);

      // print_bitboard(tot);
      // printf("%lu\n", tot);
      TEST_ASSERT_EQUAL(1, move_list.size);
      TEST_ASSERT_EQUAL(2097152ULL, tot);
   }
}

void test_pawn_capture_single() {
   {
      board brd;
      from_fen("8/8/8/4p3/3P4/8/8/8 w - - 0 1", &brd);
      arr_move256 move_list = new_arr_move256();
      add_pawn_moves(&brd, &move_list);

      bitboa tot = from_move_list(&move_list);
      // print_bitboard(tot);
      // printf("%lu\n", tot);
      TEST_ASSERT_EQUAL(2, move_list.size);
      TEST_ASSERT_EQUAL(103079215104ULL, tot);
   }
   {
      board brd;
      from_fen("8/8/8/4p3/3P4/8/8/8 b - - 0 1", &brd);
      arr_move256 move_list = new_arr_move256();
      add_pawn_moves(&brd, &move_list);

      bitboa tot = from_move_list(&move_list);
      // print_bitboard(tot);
      // printf("%lu\n", tot);
      TEST_ASSERT_EQUAL(2, move_list.size);
      TEST_ASSERT_EQUAL(402653184ULL, tot);
   }
   {
      board brd;
      from_fen("8/8/8/2p5/3P4/8/8/8 w - - 0 1", &brd);
      arr_move256 move_list = new_arr_move256();
      add_pawn_moves(&brd, &move_list);

      bitboa tot = from_move_list(&move_list);
      // print_bitboard(tot);
      // printf("%lu\n", tot);
      TEST_ASSERT_EQUAL(2, move_list.size);
      TEST_ASSERT_EQUAL(51539607552ULL, tot);
   }
   {
      board brd;
      from_fen("8/8/8/2p5/3P4/8/8/8 b - - 0 1", &brd);
      arr_move256 move_list = new_arr_move256();
      add_pawn_moves(&brd, &move_list);

      bitboa tot = from_move_list(&move_list);
      // print_bitboard(tot);
      // printf("%lu\n", tot);
      TEST_ASSERT_EQUAL(2, move_list.size);
      TEST_ASSERT_EQUAL(201326592ULL, tot);
   }
}

void test_en_passant_single() {
   {
      board brd;
      from_fen("8/2p5/8/1P6/8/8/8/8 b - - 0 1", &brd);
      make_move(&brd, move_from(c7, c5));
      arr_move256 move_list = new_arr_move256();
      add_pawn_moves(&brd, &move_list);

      bitboa tot = from_move_list(&move_list);
      // print_bitboard(tot);
      // printf("%lu\n", tot);
      TEST_ASSERT_EQUAL(2, move_list.size);
      TEST_ASSERT_EQUAL(6597069766656ULL, tot);
   }
   {
      board brd;
      from_fen("8/2p5/8/3P4/8/8/8/8 b - - 0 1", &brd);
      make_move(&brd, move_from(c7, c5));
      arr_move256 move_list = new_arr_move256();
      add_pawn_moves(&brd, &move_list);

      bitboa tot = from_move_list(&move_list);
      // print_bitboard(tot);
      // printf("%lu\n", tot);
      TEST_ASSERT_EQUAL(2, move_list.size);
      TEST_ASSERT_EQUAL(13194139533312ULL, tot);
   }
   {
      board brd;
      from_fen("8/8/8/8/2p5/8/3P4/8 w - - 0 1", &brd);
      make_move(&brd, move_from(d2, d4));
      arr_move256 move_list = new_arr_move256();
      add_pawn_moves(&brd, &move_list);

      bitboa tot = from_move_list(&move_list);
      // puts("lol?");
      // print_bitboard(tot);
      // printf("%lu\n", tot);
      TEST_ASSERT_EQUAL(2, move_list.size);
      TEST_ASSERT_EQUAL(786432ULL, tot);
   }
   {
      board brd;
      from_fen("8/8/8/8/4p3/8/3P4/8 w - - 0 1", &brd);
      make_move(&brd, move_from(d2, d4));
      arr_move256 move_list = new_arr_move256();
      add_pawn_moves(&brd, &move_list);

      bitboa tot = from_move_list(&move_list);
      // print_bitboard(tot);
      // printf("%lu\n", tot);
      TEST_ASSERT_EQUAL(2, move_list.size);
      TEST_ASSERT_EQUAL(1572864ULL, tot);
   }
}

void test_kings_right_pawn_illegal_move_into_black_west_capture() {
   board brd;
   from_fen("8/8/7k/7p/7P/7K/8/8 w - - 0 1", &brd);

   move mov = move_from(h3, g4);
   make_move(&brd, mov);

   TEST_ASSERT(bpawn_capture_west(&brd));
   // print_bitboard(get_attack_bb(&brd));
   TEST_ASSERT(in_check(&brd, false));
}

void test_do_not_white_short_castle_through_bishop() {
   board brd;
   from_fen("rnbqkbnr/pppppppp/8/8/5N2/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1", &brd);

   square sq = e1;
   bitboa occ = brd.wcb_bb | brd.bcb_bb;

   bitboa king = get_king_move(&brd, sq);
   // print_bitboard(king);
   // printf("%lu", king);
   TEST_ASSERT_EQUAL(0, king);

   from_fen("rnbqkbnr/pppppppp/8/8/4BN2/8/PPPPPPPP/RNBQK2R w KQkq - 0 1", &brd);
   king = get_king_move(&brd, sq);
   // print_bitboard(king);
   // printf("%lu", king);
   TEST_ASSERT_EQUAL(96, king);
}

void test_perft1() {
   // board brd = gen_start_board();
   board brd;
   from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", &brd);
   CLOCK_START();
   uint64_t res = perft(&brd, 1);

   CLOCK_END();
   // printf("%zu\n", res);
   TEST_ASSERT_EQUAL(20, res);
}

void test_perft2() {
   board brd = gen_start_board();
   CLOCK_START();
   uint64_t res = perft(&brd, 2);

   CLOCK_END();
   // printf("%zu\n", res);
   TEST_ASSERT_EQUAL(400, res);
}

void test_perft3() {
   board brd = gen_start_board();
   CLOCK_START();
   uint64_t res = perft(&brd, 3);

   CLOCK_END();
   // printf("%zu\n", res);
   TEST_ASSERT_EQUAL(8902, res);
}

void test_perft4() {
   board brd = gen_start_board();
   CLOCK_START();
   uint64_t res = perft(&brd, 4);

   CLOCK_END();
   // printf("%zu\n", res);
   TEST_ASSERT_EQUAL(197'281, res);
}

void test_perft5() {
   board brd = gen_start_board();
   CLOCK_START();
   uint64_t res = perft(&brd, 5);

   CLOCK_END();
   // printf("%zu\n", res);
   TEST_ASSERT_EQUAL(4'865'609, res);
}

void test_perft6() {
   board brd = gen_start_board();
   CLOCK_START();
   uint64_t res = perft(&brd, 6);

   CLOCK_END();
   // printf("%zu\n", res);
   TEST_ASSERT_EQUAL(119'060'324, res);
}

void test_perft7() {
   board brd = gen_start_board();
   CLOCK_START();
   uint64_t res = perft(&brd, 7);

   CLOCK_END();
   // printf("%zu\n", res);
   TEST_ASSERT_EQUAL(3'195'901'860, res);
}

int run_test_movegen() {
   generate_tables();
   UNITY_BEGIN();
   RUN_TEST(test_in_check);
   RUN_TEST(test_king_single);
   RUN_TEST(test_bishop_single);
   RUN_TEST(test_knight_single);
   RUN_TEST(test_rook_single);
   RUN_TEST(test_rook_count_single);
   RUN_TEST(test_queen_single);
   RUN_TEST(test_queen_count_single);
   RUN_TEST(test_pawn_push_single);
   RUN_TEST(test_pawn_capture_single);
   RUN_TEST(test_en_passant_single);
   RUN_TEST(test_do_not_white_short_castle_through_bishop);
   RUN_TEST(test_kings_right_pawn_illegal_move_into_black_west_capture);
   RUN_TEST(test_king_rook_top);
   RUN_TEST(test_ong_idk);
   RUN_TEST(test_ong_idk2);
   RUN_TEST(test_king_blocked_castle);
   RUN_TEST(test_some_kind_of_passant_bug);

   RUN_TEST(test_perft1);
   RUN_TEST(test_perft2);
   RUN_TEST(test_perft3);
   RUN_TEST(test_perft4);
   RUN_TEST(test_perft5);
   RUN_TEST(test_perft6);
   // RUN_TEST(test_perft7);
   return UNITY_END();
}
