all:
	cd apps/eggsloader && \
	make && \
	cp build/*_egg*.bin ../../out/

	cd apps/npcase && \
	make && \
	cp build/*_egg*.bin ../../out/

	cd apps/tetris && \
	make && \
	cp build/*_egg*.bin ../../out/

	rm out/*_eggs.bin