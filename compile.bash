gcc server/server.c -o run/server -lmysqlclient;
gcc -rdynamic client/client.c -o run/client -lmysqlclient `pkg-config --cflags --libs gtk+-3.0`;