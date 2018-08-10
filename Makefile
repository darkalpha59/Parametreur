CC=g++
CFLAGS=-Wextra -Wall
LDFLAGS= -ltinyxml2 -lComdlg32 -lftd2xx -lpthread
EXEC=prog
SRC=$(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	del *.o

mrproper: clean
	del $(EXEC).exe