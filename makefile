SRC := ./src
OBJ := ./obj
BIN := ./bin

SERVER_SRCS := $(wildcard $(SRC)/server/*.c)
CLIENT_SRCS := $(wildcard $(SRC)/client/*.c)
SERVER_OBJS:= $(patsubst $(SRC)/server/%.c, $(OBJ)/server/%.o, $(SERVER_SRCS))
CLIENT_OBJS := $(patsubst $(SRC)/client/%.c, $(OBJ)/client/%.o, $(CLIENT_SRCS))
SERVER := $(BIN)/server
CLIENT := $(BIN)/client

all: SERVER CLIENT

SERVER: $(SERVER_OBJS)
	mkdir -p $(BIN)
	gcc -o $(SERVER) $^

CLIENT: $(CLIENT_OBJS)
	mkdir -p $(BIN)
	gcc -o $(CLIENT) $^

$(OBJ)/server/%.o: $(SRC)/server/%.c
	mkdir -p $(OBJ)/server
	gcc -o $@ -c $<

$(OBJ)/client/%.o: $(SRC)/client/%.c
	mkdir -p $(OBJ)/client
	gcc -o $@ -c $<

server: $(SERVER)
	$(SERVER)

client: $(CLIENT)
	$(CLIENT)

clean:
	rm -rf $(OBJ)
	rm -rf $(BIN)
