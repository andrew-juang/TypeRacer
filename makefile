SRC := ./src
OBJ := ./obj
BIN := ./bin

SERVER_SRCS := $(wildcard $(SRC)/server/*.c)
CLIENT_SRCS := $(wildcard $(SRC)/client/*.c)
PROTO_SRCS := $(wildcard $(SRC)/trprotocol/*.c)

SERVER_DEPS := $(wildcard $(SRC)/server/*.h)
CLIENT_DEPS := $(wildcard $(SRC)/client/*.h)
PROTO_DEPS := $(wildcard $(SRC)/trprotocol/*.h)

SERVER_OBJS := $(patsubst $(SRC)/server/%.c, $(OBJ)/server/%.o, $(SERVER_SRCS))
CLIENT_OBJS := $(patsubst $(SRC)/client/%.c, $(OBJ)/client/%.o, $(CLIENT_SRCS))
PROTO_OBJS := $(patsubst $(SRC)/trprotocol/%.c, $(OBJ)/trprotocol/%.o, $(PROTO_SRCS))

SERVER := $(BIN)/server
CLIENT := $(BIN)/client

LINKS = -lncurses

all: SERVER CLIENT

SERVER: $(SERVER_OBJS)
	mkdir -p $(BIN)
	gcc -o $(SERVER) $^

CLIENT: $(CLIENT_OBJS)
	mkdir -p $(BIN)
	gcc -o $(CLIENT) $^ $(LINKS)

$(OBJ)/server/%.o: $(SRC)/server/%.c $(SERVER_DEPS) $(PROTO_DEPS)
	mkdir -p $(OBJ)/server
	gcc -o $@ -c $<

$(OBJ)/client/%.o: $(SRC)/client/%.c $(CLIENT_DEPS) $(PROTO_DEPS)
	mkdir -p $(OBJ)/client
	gcc -o $@ -c $<

$(OBJ)/trprotocol/%.o: $(SRC)/trprotocol/%.c $(PROTO_DEPS)
	mkdir -p $(OBJ)/trprotocol
	gcc -o $@ -c $<

server: $(SERVER)
	$(SERVER)

client: $(CLIENT)
	$(CLIENT)

clean:
	rm -rf $(OBJ)
	rm -rf $(BIN)
