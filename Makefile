
all:
	@echo "Compiling"
	@clang++ -Wall -Wextra -Werror -fsanitize=address main.cpp RequestHeaderParser/RequestHeader.cpp Response/Response.cpp utils/utils.cpp MimeTypes/MimeTypes.cpp -o webserv
	@echo "Compiled"
clean:
	@echo "Deleting out file"
	@rm webserv
	@echo "Deleted"