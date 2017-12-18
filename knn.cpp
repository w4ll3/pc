#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <vector>
#include <fstream>
#include <cmath>
#include <pthread.h>

using namespace std;

int threads;
ifstream train_file;
ifstream test_file;
vector<string> train_base, test_base;

double elapsed = 0;
#define TIME()	struct timespec start, finish; \
				clock_gettime(CLOCK_MONOTONIC, &start);

#define ENDTIME()	clock_gettime(CLOCK_MONOTONIC, &finish); \
					elapsed = (finish.tv_sec - start.tv_sec); \
					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0; \
					printf("%.f", elapsed * 1000000000);

void *classify(void *arg) {
	int id = *((int *) arg);
	int split = test_base.size() / threads;
	int begin = split * id;
	int end = begin + split;

	for (int i = begin; i < end; i++) {
		float min = 1, distance = 0;
		istringstream test_ss(test_base[i]);

		vector<float> test;
		for (float v = 0; test_ss >> v; ) {
			test.push_back(v);
			test_ss.ignore();
		}

		for (auto const& it : train_base) {
			istringstream train_ss(it);
			distance = 0;

			for (float v = 0, j = 0; train_ss >> v; j++) {
				distance += pow(test[j] - v, 2);
				train_ss.ignore();
			}

			if (sqrt(distance) < min)
				min = sqrt(distance);
		}
	}
}

int main(int argc, char **argv) {
	pthread_t threads_vect[threads];

	threads = atoi(argv[3]);

	test_file.open(argv[1], ifstream::in);
	train_file.open(argv[2], ifstream::in);
	string line;
	while(getline(test_file, line)) test_base.push_back(line);
	while(getline(train_file, line)) train_base.push_back(line);

	int ids[threads];
	TIME()
	for (int i = 0; i < threads; i++) {
		ids[i] = i;
		pthread_create(&threads_vect[i], NULL, classify, ids + i);
	}
	ENDTIME()

	for (int i = 0; i < threads; i++)
		pthread_join(threads_vect[i], NULL);
	return 0;
}
