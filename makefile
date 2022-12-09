
TARGET=main #target file name
 
all:
	gcc main.c -L/usr/include -lreadline -o $(TARGET)
	./$(TARGET)
 
clean:
	rm $(TARGET)