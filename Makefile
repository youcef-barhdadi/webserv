
all:
	@echo "Compiliing"
	@clang++ server/server.cpp RequestHeaderParser/RequestHeader.cpp utils/utils.cpp
	@echo "Compiled"
clean:
	@echo "Deleting out file"
	@rm a.out
	@echo "Deleted"