
TARGET=main #target file name
 
all:
	gcc main.c libs/errorHandler.c libs/parser.c libs/executor.c -L/usr/include -lreadline -o $(TARGET)
	./$(TARGET)
	
clean:
	rm $(TARGET)