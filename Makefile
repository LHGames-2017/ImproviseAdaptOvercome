build:
	g++ -std=c++11 utils.cpp ai.cpp -o ai -lboost_system -lcrypto -lssl -lcpprest -lglib-2.0 -I/usr/include/glib-2.0/ -I /usr/lib/x86_64-linux-gnu/glib-2.0/include/ -I includes/

clean:
	rm ai
