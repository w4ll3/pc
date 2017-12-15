#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <vector>
#include <fstream>
#include <cmath>
#include <pthread.h>

using namespace std;

int threads, size;
ifstream train_file;
ifstream test_file;
vector<string> train_base;

double elapsed = 0;
#define TIME()	struct timespec start, finish; \
				clock_gettime(CLOCK_MONOTONIC, &start);

#define ENDTIME()	clock_gettime(CLOCK_MONOTONIC, &finish); \
					elapsed = (finish.tv_sec - start.tv_sec); \
					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0; \
					printf("%f", elapsed);

void *classify(void *arg) {
	int id = (int) *((int*) arg);
	string line;

	while(getline(train_file, line)) train_base.push_back(line);
	int i = 0;
	while(getline(test_file, line)) {

		string best;
		float min = 1, distance = 0;
		istringstream test_ss(line);
		vector<float> test;

		for (float v = 0; test_ss >> v; ) {
			test.push_back(v);
			test_ss.ignore();
		}

		string test_tag(test_ss.str().substr(test_ss.str().find_last_of(",") + 1));

		for (auto it : train_base) {
			istringstream train_ss(it);
			distance = 0;

			for (float v = 0, j = 0; train_ss >> v; j++) {
				distance += pow(test.at(j) - v, 2);
				train_ss.ignore();
			}


			if (distance < min) {
				min = distance;
				best = train_ss.str().substr(train_ss.str().find_last_of(",") + 1);
			 }
		}
		i++;
	}
	cout << i << endl;
}

int main(int argc, char **argv) {
	TIME()
	int test_size, train_size;
	threads = atoi(argv[3]);
	pthread_t threads_vect[threads];

	train_file.open(argv[2], ifstream::in);
	test_file.open(argv[1], ifstream::in);

	string size_train = regex_replace(
		string(argv[2]),
		regex("[^0-9]*([0-9]+).*"),
		string("$1")
	);

	string size_test = regex_replace(
		string(argv[1]),
		regex("[^0-9]*([0-9]+).*"),
		string("$1")
	);

	istringstream testsize_ss(size_train);
	istringstream trainsize_ss(size_train);
	trainsize_ss >> size;

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
