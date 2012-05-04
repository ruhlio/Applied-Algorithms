CC=g++
CFLAGS=-c -Wall -std=c++0x -g
OBJ_DIR=obj
BIN_DIR=bin

all: anagrams bnf bowling cost espanol jungle led pills plinko postfix surjections

clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

anagrams: anagrams.cpp anagrams.in
	$(CC) $(CFLAGS) anagrams.cpp -o $(OBJ_DIR)/anagrams.o
	$(CC) $(OBJ_DIR)/anagrams.o -o $(BIN_DIR)/anagrams
	chmod +x $(BIN_DIR)/anagrams
	cp anagrams.in $(BIN_DIR)/anagrams.in

bnf: bnf.cpp bnf.in
	$(CC) $(CFLAGS) bnf.cpp -o $(OBJ_DIR)/bnf.o
	$(CC) $(OBJ_DIR)/bnf.o -o $(BIN_DIR)/bnf
	chmod +x $(BIN_DIR)/bnf
	cp bnf.in $(BIN_DIR)/bnf.in

bowling: bowling.cpp bowling.in
	$(CC) $(CFLAGS) bowling.cpp -o $(OBJ_DIR)/bowling.o
	$(CC) $(OBJ_DIR)/bowling.o -o $(BIN_DIR)/bowling
	chmod +x $(BIN_DIR)/bowling
	cp bowling.in $(BIN_DIR)/bowling.in

cost: cost.cpp cost.in
	$(CC) $(CFLAGS) cost.cpp -o $(OBJ_DIR)/cost.o
	$(CC) $(OBJ_DIR)/cost.o -o $(BIN_DIR)/cost
	chmod +x $(BIN_DIR)/cost
	cp cost.in $(BIN_DIR)/cost.in

espanol: espanol.cpp espanol.in
	$(CC) $(CFLAGS) espanol.cpp -o $(OBJ_DIR)/espanol.o
	$(CC) $(OBJ_DIR)/espanol.o -o $(BIN_DIR)/espanol
	chmod +x $(BIN_DIR)/espanol
	cp espanol.in $(BIN_DIR)/espanol.in

firefly: firefly.cpp firefly.in
	$(CC) $(CFLAGS) firefly.cpp -o $(OBJ_DIR)/firefly.o
	$(CC) $(OBJ_DIR)/firefly.o -o $(BIN_DIR)/firefly
	chmod +x $(BIN_DIR)/firefly
	cp firefly.in $(BIN_DIR)/firefly.in

gears: gears.cpp gears.in
	$(CC) $(CFLAGS) gears.cpp -o $(OBJ_DIR)/gears.o
	$(CC) $(OBJ_DIR)/gears.o -o $(BIN_DIR)/gears
	chmod +x $(BIN_DIR)/gears
	cp gears.in $(BIN_DIR)/gears.in

jungle: jungle.cpp jungle.in
	$(CC) $(CFLAGS) jungle.cpp -o $(OBJ_DIR)/jungle.o
	$(CC) $(OBJ_DIR)/jungle.o -o $(BIN_DIR)/jungle
	chmod +x $(BIN_DIR)/jungle
	cp jungle.in $(BIN_DIR)/jungle.in

led: led.cpp led.in
	$(CC) $(CFLAGS) led.cpp -o $(OBJ_DIR)/led.o
	$(CC) $(OBJ_DIR)/led.o -o $(BIN_DIR)/led
	chmod +x $(BIN_DIR)/led
	cp led.in $(BIN_DIR)/led.in

ospf: ospf.cpp ospf.in
	$(CC) $(CFLAGS) ospf.cpp -o $(OBJ_DIR)/ospf.o
	$(CC) $(OBJ_DIR)/ospf.o -o $(BIN_DIR)/ospf
	chmod +x $(BIN_DIR)/ospf
	cp ospf.in $(BIN_DIR)/ospf.in

barcode: barcode.cpp barcode.in
	$(CC) $(CFLAGS) barcode.cpp -o $(OBJ_DIR)/barcode.o
	$(CC) $(OBJ_DIR)/barcode.o -o $(BIN_DIR)/barcode
	chmod +x $(BIN_DIR)/barcode
	cp ospf.in $(BIN_DIR)/barcode.in

pancakes: pancakes.cpp pancakes.in
	$(CC) $(CFLAGS) pancakes.cpp -o $(OBJ_DIR)/pancakes.o
	$(CC) $(OBJ_DIR)/pancakes.o -o $(BIN_DIR)/pancakes
	chmod +x $(BIN_DIR)/pancakes
	cp pancakes.in $(BIN_DIR)/pancakes.i

pills: pills.cpp pills.in
	$(CC) $(CFLAGS) pills.cpp -o $(OBJ_DIR)/pills.o
	$(CC) $(OBJ_DIR)/pills.o -o $(BIN_DIR)/pills
	chmod +x $(BIN_DIR)/pills
	cp pills.in $(BIN_DIR)/pills.in

plinko: plinko.cpp plinko.in
	$(CC) $(CFLAGS) plinko.cpp -o $(OBJ_DIR)/plinko.o
	$(CC) $(OBJ_DIR)/plinko.o -o $(BIN_DIR)/plinko
	chmod +x $(BIN_DIR)/plinko
	cp plinko.in $(BIN_DIR)/plinko.in

postfix: postfix.cpp postfix.in
	$(CC) $(CFLAGS) postfix.cpp -o $(OBJ_DIR)/postfix.o
	$(CC) $(OBJ_DIR)/postfix.o -o $(BIN_DIR)/postfix
	chmod +x $(BIN_DIR)/postfix
	cp postfix.in $(BIN_DIR)/postfix.in

surjections: surjections.cpp surjections.in
	$(CC) $(CFLAGS) surjections.cpp -o $(OBJ_DIR)/surjections.o
	$(CC) $(OBJ_DIR)/surjections.o -o $(BIN_DIR)/surjections
	chmod +x $(BIN_DIR)/surjections
	cp surjections.in $(BIN_DIR)/surjections.in

sync: sync.cpp sync.in
	$(CC) $(CFLAGS) -fpermissive sync.cpp -o $(OBJ_DIR)/sync.o
	$(CC) $(OBJ_DIR)/sync.o -o $(BIN_DIR)/sync
	chmod +x $(BIN_DIR)/sync
	cp sync.in $(BIN_DIR)/sync.in
