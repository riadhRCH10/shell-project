
TARGET=main #target file name
 
all:
	gcc main.c libs/parser.c libs/errorHandler.c -L/usr/include -lreadline -o $(TARGET)
	./$(TARGET)
	
clean:
	rm $(TARGET)