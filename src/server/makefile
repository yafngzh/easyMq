OPT = -g -Wall -Wno-deprecated -Wno-non-template-friend -std=c++11
CXX = g++ $(OPT)
CC= gcc $(OPT)

BIN = mq_server_mcp.bin
INC = -I./ -I../../common
LIB = -lpthread -ldl

##################################
PUBLIC = ../../common/mcp/public/
INC += -I$(PUBLIC)

##################################
# mysql
INC += -I/usr/include/mysql/ -I/usr/local/mysql/include/
LIB += -L/usr/local/mysql/lib -lmysqlclient

##################################
COMMON_OBJ = frame_svrd.o frame_ctrl.o \
	$(PUBLIC)Base.o \
	$(PUBLIC)tlib_cfg.o \
	$(PUBLIC)tlib_log.o \
	$(PUBLIC)CodeQueue.o \
	$(PUBLIC)CodeQueueMutil.o \
	$(PUBLIC)EpollFlow.o \
	$(PUBLIC)Statistic.o \
	../../common/comm_mysql/comm_mysql.o \
	../../common/easyMsg.o

OBJS = $(COMMON_OBJ) \
	master_ctrl.o \
	frame_ctrl.o \
	frame_svrd.o \
	easyMQServer.o

all: $(BIN)

$(BIN): $(OBJS)
	rm -f $@
	$(CXX) -o $@ $(INC) $^ $(LIB)
	cp -f $@ ../../bin/

%.o: %.cpp
	$(CXX) $(INC) -c -o $@ $<
%.o: %.c
	$(CC) $(INC) -c -o $@ $<
clean:
	rm -f *.o $(BIN) $(OBJS)

cleanall:
	rm -f *.o $(BIN) $(OBJS)

