all:
	clang++ clnt.cpp -o clnt
	clang++ Utils.cpp Socket.cpp serv.cpp -o serv

serv: all
	./serv

clnt: all
	./clnt 127.0.0.1 9190