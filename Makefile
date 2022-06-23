CXX=g++
OBJ=xxd.o
BIN=xxd
SRC=xxd.cpp
CFLAGS=-O3

$(BIN): $(OBJ)
	$(CXX) -o $(BIN) $(OBJ) $(CFLAGS)

$(OBJ): $(SRC)
	$(CXX) -c $(SRC) -o $(OBJ) $(CFLAGS)

all: $(OBJ) $(BIN)

clean:
	rm -f $(OBJ) $(BIN)

