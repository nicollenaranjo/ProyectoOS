CC=gcc

%.o: %.c
	$(CC) -c -o $@ $<
allmessage: message_sender01 message_sender02 message_receiver message_sender03_multiplemessages message_receiver02_multiplemessages.c
allnamepipes: pipe_writer01 pipe_writer02 pipe_reader01 pipe_reader02 ordinary_pipe
all: allmessage shared_memory_set01 shared_memory_get01 allnamepipes socket_client socket_server
	
ecs-egent1: ecs-egent1.o
	gcc -o ecs-egent1 ecs-egent1.o

ecs-egent2: ecs-egent2.o
	gcc -o ecs-egent2 ecs-egent2.o

ecs: ecs.o
	gcc -o ecs ecs.o

tar:
	tar cf ../examples.tar . 
	mv ../examples.tar .
	
clean:
	rm -f *.o
	rm -f *.class
	rm -f message_sender01 message_sender02 message_receiver shared_memory_get01 shared_memory_get02 shared_memory_set01 shared_memory_set02
	rm -f ordinary_pipe pipe_reader01 pipe_writer01