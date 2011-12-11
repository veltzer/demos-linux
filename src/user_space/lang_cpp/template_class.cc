#include <stdio.h> // for printf(3)

template<class T>
class AverageMaker {
	private:
		T data;
		int samples;

	public:
		void init() {
			data=0;
			samples=0;
		}
		void addSample(T t) {
			data+=t;
			samples++;
		}
		T getAverage() {
			return data/samples;
		}
};

int main() {
	AverageMaker<double> amd;
	amd.init();
	amd.addSample(3.5);
	amd.addSample(2.2);
	amd.addSample(7.1);
	printf("average is %lf\n",amd.getAverage());
	return 0;
}
