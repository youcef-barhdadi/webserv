
all:
	@echo "Compiling"
	@clang++ -Wall -Wextra -Werror -fsanitize=address main.cpp RequestHeaderParser/RequestHeader.cpp utils/utils.cpp
	@echo "Compiled"
clean:
	@echo "Deleting out file"
	@rm a.out
	@echo "Deleted"