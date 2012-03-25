CC=g++
CFLAGS=-c -Wall -std=c++0x
OBJ_DIR=obj
BIN_DIR=bin

all: anagrams bnf bowling cost espanol gears jungle led pills plinko postfix surjections

clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

anagrams: anagrams.cpp anagrams.in
	$(CC) $(CFLAGS) anagrams.cpp -o $(OBJ_DIR)/anagrams.o
	$(CC) $(OBJ_DIR)/anagrams.o -o $(BIN_DIR)/anagrams
	chmod 755 $(BIN_DIR)/anagrams
	cp anagrams.in $(BIN_DIR)/anagrams.in

bnf: bnf.cpp bnf.in
	$(CC) $(CFLAGS) bnf.cpp -o $(OBJ_DIR)/bnf.o
	$(CC) $(OBJ_DIR)/bnf.o -o $(BIN_DIR)/bnf
	chmod 755 $(BIN_DIR)/bnf
	cp bnf.in $(BIN_DIR)/bnf.in

bowling: bowling.cpp bowling.in
	$(CC) $(CFLAGS) bowling.cpp -o $(OBJ_DIR)/bowling.o
	$(CC) $(OBJ_DIR)/bowling.o -o $(BIN_DIR)/bowling
	chmod 755 $(BIN_DIR)/bowling
	cp bowling.in $(BIN_DIR)/bowling.in

cost: cost.cpp cost.in
	$(CC) $(CFLAGS) cost.cpp -o $(OBJ_DIR)/cost.o
	$(CC) $(OBJ_DIR)/cost.o -o $(BIN_DIR)/cost
	chmod 755 $(BIN_DIR)/cost
	cp cost.in $(BIN_DIR)/cost.in

espanol: espanol.cpp espanol.in
	$(CC) $(CFLAGS) espanol.cpp -o $(OBJ_DIR)/espanol.o
	$(CC) $(OBJ_DIR)/espanol.o -o $(BIN_DIR)/espanol
	chmod 755 $(BIN_DIR)/espanol
	cp espanol.in $(BIN_DIR)/espanol.in

gears: gears.cpp gears.in
	$(CC) $(CFLAGS) gears.cpp -o $(OBJ_DIR)/gears.o
	$(CC) $(OBJ_DIR)/gears.o -o $(BIN_DIR)/gears
	chmod 755 $(BIN_DIR)/gears
	cp gears.in $(BIN_DIR)/gears.in

jungle: jungle.cpp jungle.in
	$(CC) $(CFLAGS) jungle.cpp -o $(OBJ_DIR)/jungle.o
	$(CC) $(OBJ_DIR)/jungle.o -o $(BIN_DIR)/jungle
	chmod 755 $(BIN_DIR)/jungle
	cp jungle.in $(BIN_DIR)/jungle.in

led: led.cpp led.in
	$(CC) $(CFLAGS) led.cpp -o $(OBJ_DIR)/led.o
	$(CC) $(OBJ_DIR)/led.o -o $(BIN_DIR)/led
	chmod 755 $(BIN_DIR)/led
	cp led.in $(BIN_DIR)/led.in

pills: pills.cpp pills.in
	$(CC) $(CFLAGS) pills.cpp -o $(OBJ_DIR)/pills.o
	$(CC) $(OBJ_DIR)/pills.o -o $(BIN_DIR)/pills
	chmod 755 $(BIN_DIR)/pills
	cp pills.in $(BIN_DIR)/pills.in

plinko: plinko.cpp plinko.in
	$(CC) $(CFLAGS) plinko.cpp -o $(OBJ_DIR)/plinko.o
	$(CC) $(OBJ_DIR)/plinko.o -o $(BIN_DIR)/plinko
	chmod 755 $(BIN_DIR)/plinko
	cp plinko.in $(BIN_DIR)/plinko.in

postfix: postfix.cpp postfix.in
	$(CC) $(CFLAGS) postfix.cpp -o $(OBJ_DIR)/postfix.o
	$(CC) $(OBJ_DIR)/postfix.o -o $(BIN_DIR)/postfix
	chmod 755 $(BIN_DIR)/postfix
	cp postfix.in $(BIN_DIR)/postfix.in

surjections: surjections.cpp surjections.in
	$(CC) $(CFLAGS) surjections.cpp -o $(OBJ_DIR)/surjections.o
	$(CC) $(OBJ_DIR)/surjections.o -o $(BIN_DIR)/surjections
	chmod 755 $(BIN_DIR)/surjections
	cp surjections.in $(BIN_DIR)/surjections.in

