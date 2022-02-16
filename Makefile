



# -fsanitize=address
make:
	 g++ -g -fsanitize=address  utilities/*.cpp server/*.cpp   FileDescriptorManager/FileDescriptorManager.cpp NetWorking/*.cpp http/*.cpp   spinner/*.cpp  config/*.cpp  cgi/*.cpp MimeTypes/MimeTypes.cpp  main.cpp -o webserv