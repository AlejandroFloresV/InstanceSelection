CC = g++
OBJ = run
OUT = output/parse
DAT = data/norm

all: solver extra

solver:
	rm $(OBJ)
	$(CC) $(OBJ).cpp -o $(OBJ)

extra:
	rm $(DAT)
	$(CC) $(DAT).cpp -o $(DAT)
	rm $(OUT)
	$(CC) $(OUT).cpp -o $(OUT)
