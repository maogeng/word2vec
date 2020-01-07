CC = gcc
#Using -Ofast instead of -O3 might result in faster code, but is supported only by newer GCC versions
CFLAGS = -lm -pthread -O3 -march=native -Wall -funroll-loops -Wno-unused-result

all: word2vec word2phrase distance word-analogy compute-accuracy

word2vec : word2vec.c
	$(CC) word2vec.c -o word2vec $(CFLAGS)
word2phrase : word2phrase.c
	$(CC) word2phrase.c -o word2phrase $(CFLAGS)
distance : distance.c
	$(CC) distance.c -o distance $(CFLAGS)
word-analogy : word-analogy.c
	$(CC) word-analogy.c -o word-analogy $(CFLAGS)
compute-accuracy : compute-accuracy.c
	$(CC) compute-accuracy.c -o compute-accuracy $(CFLAGS)
	chmod +x *.sh
eval: eval.c
	$(CC) eval.c -o eval $(CFLAGS)
bin2text: bin2text.c
	$(CC) bin2text.c -o bin2text $(CFLAGS)
text2bin: text2bin.c
	$(CC) text2bin.c -o text2bin $(CFLAGS)


clean:
	rm -rf word2vec word2phrase distance word-analogy compute-accuracy
