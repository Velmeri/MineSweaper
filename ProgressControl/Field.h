#pragma once
#include <stdlib.h>

using namespace std;

int MyRandom(int min, int max){
	return rand() % (max - min) + min;
}

void Generate(short field[], unsigned const int SIZE, unsigned const short COUNT) {
	{
		for (int i = 0; i < COUNT; i++) {
			unsigned short res = NULL;
			do {
				res = MyRandom(0, SIZE - 1);
			} while (field[res] == 1);
		}
	}
}