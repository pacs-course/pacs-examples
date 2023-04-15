test: test_move test_nomove
test_move:
	echo "Testing move semantic"
	make clean
	$(MAKE) -B all DEBUG=no
	valgrind --tool=massif --massif-out-file=move.out ./$(EXEC)
	ms_print move.out > move_report.txt
	-rm move.out
test_nomove:
	echo "Testing move semantic"
	make clean
	$(MAKE) -B all CPPFLAGS="$(CPPFLAGS) -DNOMOVE" DEBUG=no
	valgrind --tool=massif --massif-out-file=move.out ./$(EXEC)
	ms_print move.out > no_move_report.txt
	-rm move.out
