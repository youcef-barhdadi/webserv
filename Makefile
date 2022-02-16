
all:
	@echo "Compiling"
	@clang++ -Wall -Wextra -Werror -fsanitize=address main.cpp Request/Request.cpp Response/Response.cpp utils/utils.cpp Response/MimeTypes/MimeTypes.cpp Server/Config.cpp Server/Server.cpp -o webserv
	@echo "Compiled"
clean:
	@echo "Deleting out file"
	@rm webserv
	@echo "Deleted"