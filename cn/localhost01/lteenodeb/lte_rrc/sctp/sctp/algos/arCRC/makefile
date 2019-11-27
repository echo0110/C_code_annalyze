include $(LTE_ROOT)/make.inc

CFLAGS	= -c -g $(OPTIM_FLAGS) -UARCRC_DEBUG_LOG
F_INCLUDE= -I ${ARCRC_BASE_DIR}/h
             


OBJS    = 	${ARCRC_BASE_DIR}/src/arCRC.o

target :${OBJS} 

	mv arCRC.o ${ARCRC_BASE_DIR}/obj/ 

clean   :
		$(RM) ${ARCRC_BASE_DIR}/obj/arCRC.o

#-------------------- Some rules. -------------#
.c.o:
	$(LTE_CC) ${F_INCLUDE} ${CFLAGS} $<
