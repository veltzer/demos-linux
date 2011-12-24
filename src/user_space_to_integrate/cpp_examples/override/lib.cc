
const char* reply="hello";

extern "C" {
	const char* getenv(const char* var) {
		return reply;
	}
}
