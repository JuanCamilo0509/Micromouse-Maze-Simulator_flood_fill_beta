CC = gcc
OBJS = main.c maze.c mouse.c solver.c include/queue.c include/stack.c
CFLAGS = -w $(shell sdl2-config --cflags)
LDFLAGS = -lSDL2 -lSDL2_image $(shell sdl2-config --libs) -lm
OBJFiles = maze.o main.o mouse.o solver.o
TARGET = simulation

all : $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(OBJS) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS) 

clean:
	rm -f $(OBJFILES) $(TARGET) *~
