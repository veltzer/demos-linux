#include <ace/OS_main.h>
#include <ace/Stream.h>
#include <ace/Task.h>
#include <ace/Message_Block.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

typedef ACE_Stream<ACE_MT_SYNCH> MT_Stream;
typedef ACE_Module<ACE_MT_SYNCH> MT_Module;
typedef ACE_Task<ACE_MT_SYNCH> MT_Task;

static int ProducerData = 0;
class Consumer : public MT_Task {
public:
	// Initialize Consumer.
	virtual int open(void *) {
		// <activate> is inherited from class Task.
		return(activate(THR_BOUND));
	}


	// Enqueue the message on the Message_Queue
	// for subsequent processing in <svc>.
	virtual int put(ACE_Message_Block *mb, ACE_Time_Value *tv = 0) {
		// <putq> is inherited from class Task.
		return(putq(mb, tv));
	}


	// Receive message from producer and print to stdout.
	virtual int svc(void);
};

// Define the Producer interface
class Producer : public MT_Task {
public:
	// Initialize Producer.
	virtual int open(void *) {
		// activate() is inherited from class Task.
		return(activate(THR_BOUND));
	}


	// Read data from stdin and pass to consumer.
	virtual int svc(void);
};

int Producer::svc(void) {
	if (ProducerData) {
		while (true) {
			printf("Second Program Running\n");
			sleep(1);
		}
	}
	while (true) {
		ACE_Message_Block *mb;
		// Allocate a new message.
		ACE_NEW_RETURN(mb, ACE_Message_Block(BUFSIZ), -1);
		// Keep reading stdin, until we reach EOF.
		ssize_t n = ACE_OS::read(ACE_STDIN, mb->wr_ptr(), mb->size());
		if (n <= 0) {
			// Send shutdown message to other thread and exit.
			mb->length(0);
			put_next(mb);
			break;
		} else {
			// Adjust write pointer.
			mb->wr_ptr(n);
			// Send the message to the other thread.
			this->put_next(mb);
		}
	}
	return(0);
}


int Consumer::svc(void) {
	ACE_Message_Block *mb = 0;

	// Keep looping, reading a message from the queue,
	// until we get a 0 length message, then quit.
	while (true) {
		int result = getq(mb);
		if (result == -1) {
			break;
		}
		int length = mb->length();
		if (length > 0) {
			ACE_OS::write(ACE_STDOUT, mb->rd_ptr(), length);
		}
		mb->release();
		if (length == 0) {
			break;
		}
	}
	return(0);
}


int main(int argc, char *argv[]) {
	// Control hierarchically-related active objects.
	MT_Stream stream;

	// All processing is performed in the
	// Stream after <push>’s complete.
	stream.push(new MT_Module("Consumer", new Consumer));
	stream.push(new MT_Module("Producer", new Producer));
	ProducerData = 1;
	stream.push(new MT_Module("Producer1", new Producer));
	// Barrier synchronization: wait for
	// the threads, to exit, then exit the main thread.
	ACE_Thread_Manager::instance()->wait();
	return(0);
}
