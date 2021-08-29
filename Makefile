
CFLAG=-I./libs

all: bin/im_cli bin/im_srv

bin/im_cli: client/im_client.c libs/logger.c client/readline.c
	gcc -o ./bin/im_cli ${CFLAG} client/im_client.c libs/logger.c client/readline.c

bin/im_srv: server/im_server.c libs/logger.c
	gcc -o ./bin/im_srv ${CFLAG} server/im_server.c libs/logger.c

clean:
	rm -f ./bin/*