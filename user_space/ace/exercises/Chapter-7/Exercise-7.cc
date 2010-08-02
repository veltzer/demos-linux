#include <ace/OS_NS_stdio.h>
#include <ace/OS_NS_string.h>

#include <ace/MMAP_Memory_Pool.h>
#include <ace/Malloc_T.h>
#include <ace/Null_Mutex.h>
#include <ace/PI_Malloc.h>

#include <ace/Read_Buffer.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

typedef ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex, ACE_PI_Control_Block>   SHARED_ALLOC;
typedef ACE_Malloc_LIFO_Iterator_T<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex, ACE_PI_Control_Block>
MALLOC_LIFO_RECORD;

SHARED_ALLOC *shared;

class Record {
public:
	Record(SHARED_ALLOC * shared, char *name) {
		size_t len = ACE_OS::strlen(name) + 1;
		char   *buf = reinterpret_cast<char *>(shared->malloc(len));

		ACE_OS::strcpy(buf, name);
		name_ = buf;
	}


	~Record() {
		shared->free(name_.addr());
	}

	char *name(void) {
		return(name_);
	}


private:
	ACE_Based_Pointer_Basic<char> name_;
};

int PrintMessages(SHARED_ALLOC *shared) {
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("The following messages were found:\n")));

	{
		MALLOC_LIFO_RECORD record(*shared);

		for (void *temp = 0; record.next(temp) != 0; record.advance()) {
			Record *record = reinterpret_cast<Record *>(temp);
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("%C\n"), record->name()));
		}
	}
	return(0);
}


int StoreMessages(SHARED_ALLOC *shared, char *buf) {
	void *memory = shared->malloc(sizeof(Record));

	if (memory == 0) {
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("Unable to malloc")), -1);
	}

	// Allocate and place record
	Record *newRecord = new(memory) Record(shared, buf);
	if (shared->bind(buf, newRecord) == -1) {
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("bind failed")), -1);
	}
	return(0);
}


int GetMessageType(char *data) {
	static ACE_Read_Buffer rb(ACE_STDIN);                                                                                                       // Read new line from stdin

	// read a single line from stdin
	// Allocate a new buffer.
	char *buffer = rb.read('\n');

	if (buffer == 0) {                                                                                                                                                                                                   // return message type zero when EOF is reached
		return(0);                                                                                                                                                                                                                      // Return 0 as message type
	} else {
		int type;
		sscanf(buffer, "%d", &type);
		ACE_OS::sprintf(data, "%s", buffer + 2);                                                                                                                                                                                                                   // Remove the type from the buffer
		return(type);
	}
}


// Backing files where the data is kept.
#define STORE_NAME ACE_TEXT("Exercise_7.store")
#define Address    ACE_DEFAULT_BASE_ADDR

int ACE_TMAIN(int argc, ACE_TCHAR *[]) {
	if (argc > 1) {                                                                                                  // Use an existing file
		ACE_MMAP_Memory_Pool_Options options(ACE_DEFAULT_BASE_ADDR, ACE_MMAP_Memory_Pool_Options::FIRSTCALL_FIXED);

		ACE_NEW_RETURN(shared, SHARED_ALLOC(STORE_NAME, STORE_NAME, &options), -1);
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("Mapped to base address %@\n"), shared->base_addr()));

		PrintMessages(shared);
	} else {
		ACE_MMAP_Memory_Pool_Options options(0, ACE_MMAP_Memory_Pool_Options::NEVER_FIXED);

		ACE_NEW_RETURN(shared, SHARED_ALLOC(STORE_NAME, STORE_NAME, &options), -1);
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("Mapped to base address %@\n"), shared->base_addr()));

		char buffer[100];
		int type = 1;
		while (type) {
			type = GetMessageType(buffer);
			StoreMessages(shared, buffer);
		}
	}

	shared->sync();
	delete shared;
	return(0);
}
