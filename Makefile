




make:
	 g++ -fsanitize=address -g  utilities/*.cpp server/*.cpp   FileDescriptorManager/FileDescriptorManager.cpp NetWorking/*.cpp http/*.cpp   spinner/*.cpp    cgi/*.cpp MimeTypes/MimeTypes.cpp  main.cpp -o webserv