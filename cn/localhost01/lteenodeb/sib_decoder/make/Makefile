# Change COMPILE_STUB=true to compile sibDecoderStub
COMPILE_STUB=false
#COMPILE_STUB=true

CC=$(LTE_CC) $(OPTIM_FLAGS)
CFLAGS=-c  
INCLUDE=-I../include/ -I../include/rtxsrc/ -I../include/rtsrc/ -I../include/rtpersrc/
LIB_PATH=../lib/
BIN := ../bin/
SRC := ../src/
OBJ := ../obj/
SRC_FILES := $(SRC)sibParsing.c $(SRC)sib_decoder.c
OBJ_FILES := $(OBJ)sibParsing.o  $(OBJ)sib_decoder.o
ALL_OBJ_FILES := $(OBJ)*.o
STUB_SRC_FILES := $(SRC)sib_decoder_stub.c
STUB_OBJ_FILES := $(OBJ)sib_decoder_stub.o
LIB_STUB := ${LIB_PATH}libasndecoder.a -lm -lpthread
ifeq '$(COMPILE_STUB)' 'true'
all: CREATE_DIRS libasndecoder.a sibDecoderStub
else
all: CREATE_DIRS libasndecoder.a 
endif

libasndecoder.a: $(OBJ_FILES)
	ar cq $(@) $(ALL_OBJ_FILES)
	mv $(@) $(LIB_PATH)

$(OBJ_FILES): $(SRC_FILES)
	$(CC) $(INCLUDE) $(CFLAGS)$(SRC_FILES)
	@mkdir -p -v $(OBJ)
	mv *.o $(OBJ)

ifeq '$(COMPILE_STUB)' 'true'
sibDecoderStub: $(STUB_OBJ_FILES)
		$(CC) $(STUB_OBJ_FILES) $(LIB_STUB) -o sibDecoderStub 
		@mkdir -p -v $(BIN)
		mv $(@) $(BIN)

$(STUB_OBJ_FILES): $(STUB_SRC_FILES)
		$(CC) $(INCLUDE) $(CFLAGS) $(STUB_SRC_FILES)
	    mv *.o $(OBJ)
endif

.PHONY:CREATE_DIRS
CREATE_DIRS:
	mkdir -p $(LIB_PATH)

clean: 
	rm -rf $(OBJ)*o $(LIB_PATH)libasndecoder.a $(BIN)sibDecoderStub
