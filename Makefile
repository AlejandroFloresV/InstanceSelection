CC = g++
SOL = run
OUT = output/parse
DAT = data/norm

all: solver extra

solver:
	rm $(SOL)
	$(CC) $(SOL).cpp -o $(SOL)

extra:
	rm $(DAT)
	$(CC) $(DAT).cpp -o $(DAT)
	rm $(OUT)
	$(CC) $(OUT).cpp -o $(OUT)
