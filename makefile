CFLAGS      = -std=c11 -g -static -Wall -pedantic-errors
docker_spel = docker run --rm -v $(CURDIR):/compiler -w /compiler compilerbook
out_dir     = bin

$(out_dir)/main: \
	src/main.c $(out_dir)/error.o $(out_dir)/parse.o $(out_dir)/codegen.o $(out_dir)/vector.o
	$(docker_spel) gcc -o $@ $^ $(LDFLAGS)

$(out_dir)/error.o: src/error.c
	$(docker_spel) gcc -c -o $@ $< $(CFLAGS)

$(out_dir)/parse.o: src/parse.c src/struct.h src/enum.h src/compiler.h
	$(docker_spel) gcc -c -o $@ $< $(CFLAGS)

$(out_dir)/codegen.o: \
	src/codegen.c src/struct.h src/enum.h src/compiler.h
	$(docker_spel) gcc -c -o $@ $< $(CFLAGS)

$(out_dir)/vector.o: src/vector.c src/vector.h
	$(docker_spel) gcc -c -o $@ $< $(CFLAGS)

run: bin/main
	$(docker_spel) $< $(first)

asm: tmp.s
	$(docker_spel) cc -o tmp $<

asm_run: tmp
	$(docker_spel) ./$<

test: test.sh bin/main 
	$(docker_spel) ./$<

clean:
	rm -fR bin/*

.PHONY: test clean
