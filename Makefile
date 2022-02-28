
CC = clang++

FLAGS = -Wextra -Werror -Wall

DEBUG = -fsanitize=address

SRCS =	Utilities/Utilities.cpp\
		\
		FileDescriptorManager/FileDescriptorManager.cpp\
		\
		Spinner/Spinner.cpp\
		\
		Config/Config.cpp\
		\
		Cgi/Cgi.cpp\
		\
		MimeTypes/MimeTypes.cpp\
		\
		Http/Response.cpp\
		Http/Error_codes.cpp\
		Http/Request.cpp\
		\
		Server/Server.cpp\
		\
		main.cpp\
		\

OBJS =	Utilities.o\
		\
		FileDescriptorManager.o\
		\
		Spinner.o\
		\
		Config.o\
		\
		Cgi.o\
		\
		MimeTypes.o\
		\
		Response.o\
		Error_codes.o\
		Request.o\
		\
		Server.o\
		\
		main.o\
		\

all:
	@echo "Compiling"
	@$(CC) -c $(FLAGS) $(SRCS)
	@$(CC) $(OBJS) -o webserv
	@[[ ! -d objs ]] && mkdir objs; mv *.o ./objs
	@echo "Compiled"
clean:
	@rm -rf webserv
	@rm -rf objs

re: clean all
