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
string line, train_name, test_name;
ifstream train_file;
vector<string> train_base;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

double elapsed = 0;
#define TIME()	struct timespec start, finish; \
				clock_gettime(CLOCK_MONOTONIC, &start);

#define ENDTIME()	clock_gettime(CLOCK_MONOTONIC, &finish); \
					elapsed = (finish.tv_sec - start.tv_sec); \
					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0; \
					printf("%.f", elapsed * 1000000000);

void *classify(void *arg) {
	int id = *((int *) arg);
	int leave = 1;

	string comp;
	ifstream test_file;
	test_file.open(test_name, ifstream::in);
	for (int i = 0; i < id; i++) {
		getline(test_file, comp);
		if(test_file.eof()) leave = 0;
	}

	while(leave) {
		float min = 1, distance = 0;
		istringstream test_ss(comp);
		vector<float> test;

		for (float v = 0; test_ss >> v; ) {
			test.push_back(v);
			test_ss.ignore();
		}

		for (auto const& it : train_base) {
			istringstream train_ss(it);
			distance = 0;

			for (float v = 0, j = 0; train_ss >> v; j++) {
				distance += sqrt(pow(test[j] - v, 2));
				train_ss.ignore();
			}

			if (distance < min)
				min = distance;
		}
		for (int i = 0; i < threads; i++) {
			getline(test_file, comp);
			if(test_file.eof()) leave = 0;
		}
	}
}

int main(int argc, char **argv) {
	TIME()
	threads = atoi(argv[3]);
	pthread_t threads_vect[threads];
	pthread_mutex_init(&lock, NULL);

	train_name = argv[2];
	test_name = argv[1];

	train_file.open(train_name, ifstream::in);
	while(getline(train_file, line)) train_base.push_back(line);

	int ids[threads];
	for (int i = 0; i < threads; i++) {
		ids[i] = i;
		pthread_create(&threads_vect[i], NULL, classify, ids + i);
	}

	for (int i = 0; i < threads; i++)
		pthread_join(threads_vect[i], NULL);

	ENDTIME()
	return 0;
}
