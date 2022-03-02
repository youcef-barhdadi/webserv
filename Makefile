
CC = clang++

NAME = webserv.out

FLAGS = -Wextra -Werror -Wall

DEBUG = -fsanitize=address

SRCS =	Utilities/Utilities.cpp\
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

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(OBJS) -o $(NAME)
	@echo "Compiled"

$(OBJS): $(SRCS)
	@echo "Compiling"
	@$(CC) -c $(FLAGS) $(SRCS)


clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: clean all
