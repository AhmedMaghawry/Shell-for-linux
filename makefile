HEADERS = command_parser.h commands.h environment.h file_processing.h variables.h
OBJECTS = main.c command_parser.c commands.c environment.c file_processing.c variables.c

default: shell

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

shell: $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f shell
