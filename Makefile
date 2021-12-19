




make:
	 g++  -g -fsanitize=address NetWorking/*.cpp http/*.cpp   spinner/*.cpp   server/*.cpp  cgi/*.cpp   main.cpp -o webserv