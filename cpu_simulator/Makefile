CC = gcc
CFLAGS = -Wall -g

SRC = CPU_simulator.c structure.c scheduling.c evaluation.c
OBJ = $(SRC:.c=.o)
TARGET = cpu_sim

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f *.o $(TARGET)