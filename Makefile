
all:
	@echo "Compiling"
	@clang++ main.cpp RequestHeaderParser/RequestHeader.cpp utils/utils.cpp -g3
	@echo "Compiled"
clean:
	@echo "Deleting out file"
	@rm a.out
	@echo "Deleted"