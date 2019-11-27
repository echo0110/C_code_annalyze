# Change COMPILE_STUB=true to compile umtsSibDecoderStub
COMPILE_STUB=false
#COMPILE_STUB=true

CC=$(LTE_CC) $(OPTIM_FLAGS)
CFLAGS=-c  
INCLUDE=-I../include/ -I../include/rtxsrc/ -I../include/rtsrc/ -I../include/rtpersrc/
LIB_PATH=../lib/
BIN := ../bin/
SRC := ../src/
OBJ := ../obj/
SRC_FILES := $(SRC)umtsSibParsing.c $(SRC)umts_sib_decoder.c
OBJ_FILES := $(OBJ)umtsSibParsing.o  $(OBJ)umts_sib_decoder.o
ALL_OBJ_FILES := $(OBJ)*.o
STUB_SRC_FILES := $(SRC)umts_sib_decoder_stub.c
STUB_OBJ_FILES := $(OBJ)umts_sib_decoder_stub.o
LIB_STUB := ${LIB_PATH}libasndecoder_umts.a -lm -lpthread
ifeq '$(COMPILE_STUB)' 'true'
all: libasndecoder_umts.a umtsSibDecoderStub
else
all: libasndecoder_umts.a 
endif

libasndecoder_umts.a: $(OBJ_FILES)
	ar cq $(@) $(ALL_OBJ_FILES)
	@mkdir -p -v $(LIB_PATH)
	mv $(@) $(LIB_PATH)

$(OBJ_FILES): $(SRC_FILES)
	$(CC) $(INCLUDE) $(CFLAGS)$(SRC_FILES)
	@mkdir -p -v $(OBJ)
	mv *.o $(OBJ)


ifeq '$(COMPILE_STUB)' 'true'
sibDecoderStub: $(STUB_OBJ_FILES)
		$(CC) $(STUB_OBJ_FILES) $(LIB_STUB) -o umtsSibDecoderStub 
		@mkdir -p -v $(BIN)
		mv $(@) $(BIN)

$(STUB_OBJ_FILES): $(STUB_SRC_FILES)
		$(CC) $(INCLUDE) $(CFLAGS) $(STUB_SRC_FILES)
	    mv *.o $(OBJ)
endif

clean: 
	rm -rf $(OBJ)*o $(LIB_PATH)libasndecoder_umts.a $(BIN)umtsSibDecoderStub
	rm -rf $(LIB_PATH)
	rm -rf $(OBJ)
