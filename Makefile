




make:
<<<<<<< HEAD
	 g++  -g -fsanitize=address NetWorking/*.cpp http/*.cpp   spinner/*.cpp   server/*.cpp  cgi/*.cpp   main.cpp -o webserv
=======
	 g++ -fsanitize=address -g  utilities/*.cpp server/*.cpp   FileDescriptorManager/FileDescriptorManager.cpp NetWorking/*.cpp http/*.cpp   spinner/*.cpp    cgi/*.cpp   main.cpp -o webserv
>>>>>>> 6391303bb264aa78e96bc2b5bd0b764103bdc343
