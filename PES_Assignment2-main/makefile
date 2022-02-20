
CC = gcc

CFLAGS = -Wall -Werror

DEPS = llfifo.h cbfifo.h
OBJ = llfifo.o test_llfifo.o cbfifo.o test_cbfifo.o assignment2_tester.o

%.o: %.c $(DEPS) 
  $(CC) $(CFLAGS) -c -o $@ $<

finally: $(OBJ) 
  $(CC) $(CFLAGS) -o $@ $^ 
  
clean: 
  rm -r $(OBJ) finally
