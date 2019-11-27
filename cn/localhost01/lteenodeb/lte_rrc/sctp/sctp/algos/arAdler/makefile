include $(LTE_ROOT)/make.inc

CFLAGS	=	-c -g $(OPTIM_FLAGS) -UARADLER_DEBUG_LOG 
F_INCLUDE= -I ${ARADLER_BASE_DIR}/h
             


OBJS    = 	${ARADLER_BASE_DIR}/src/arAdler.o

target :${OBJS} 

	mv arAdler.o ${ARADLER_BASE_DIR}/obj/  

clean   :
		$(RM) ${ARADLER_BASE_DIR}/obj/arAdler.o

#-------------------- Some rules. -------------#
.c.o:
	$(LTE_CC) ${F_INCLUDE} ${CFLAGS} $< 
