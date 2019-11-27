CC = $(LTE_CC)
logger:
	$(CC) -c ./src/advlogging.c -I./include
	ar -rcs libadvlogger.a ./advlogging.o
	cp ./libadvlogger.a ../../library
clean:
	rm -f ./*.o
	rm -f libadvlogger.a 
