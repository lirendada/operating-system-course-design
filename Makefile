CC=g++
CFLAGS=#-g -Wall

INCLUDES = -I./include
TARGET = main
SRCDIR = ./src
OBJDIR = ./obj
SRC = $(wildcard $(SRCDIR)/*.cc)						 #表示src目录下的所有.cc文件
OBJ = $(patsubst $(SRCDIR)/%.cc, $(OBJDIR)/%.o, $(SRC))  #将SRC后缀改为.o并且将目录改成目标目录

$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread
	
$(OBJDIR)/%.o : $(SRCDIR)/%.cc		#这里因为使用$<，所以必须使用模式匹配来处理，而不能使用非模式匹配，不然匹配的只有第一个源文件
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $< -lpthread

.PHONY:clean
clean:
	rm -rf $(TARGET) $(OBJDIR)/*.o