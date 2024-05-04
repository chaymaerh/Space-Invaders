#
# Makefile pour le jeu spaceinvader
#
TARGET= spaceinvader
CC = clang
CFLAGS= -g -W -Wall -Wextra -MMD -IGraphique
LDFLAGS=-lm -lgraph -LGraphique -lSDL -lSDL_ttf

default: step1 step2

Graphique/libgraph.o: Graphique/libgraph.c Graphique/libgraph.h Graphique/variables.h
spaceinvader.o: spaceinvader.c spaceinvader.h variables.h
	gcc $(CFLAGS) -c $<

step1:
	make -C Graphique


step2: ${TARGET}

$(TARGET): spaceinvader.o
	gcc $^ -o $(TARGET) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGET)
	make -C Graphique -f Makefile $@

tidy:
	clang-tidy -checks="*" spaceinvader.c

format:
	clang-format -i spaceinvader.c
