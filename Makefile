CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
CFLAGS=-Wall --debug

OBJ = main.o fibers.o arm_fibers.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

%o: %.s
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -Wl,--start-group -lgcc -lc -lm -lrdimon -o $@ $^

clean:
	@rm -rf main *o
