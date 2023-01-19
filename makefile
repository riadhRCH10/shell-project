
TARGET=main
 
all:
	sudo gcc main.c libs/errorHandler.c libs/parser.c libs/executor.c libs/init-shell.c -L/usr/include -lreadline -o /bin/$(TARGET)
	./$(TARGET)
	
clean:
	sudo rm /bin/$(TARGET)