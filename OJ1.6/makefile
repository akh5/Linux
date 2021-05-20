oj_server:oj_server.cpp MyDB.cpp 
	g++ $^ -o $@ `mysql_config --cflags --libs` -lpthread -std=c++11 -I ~/third_part/include -L ~/third_part/lib -lctemplate -g -ljsoncpp
