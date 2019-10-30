#include <unistd.h>
#include <iostream>
#include "adc.h"

using namespace std;
using namespace BBB;

void alarm(void*);

int main() {
	ADC pot(0);

	pot.setTriggerMode(HIGHER);
	pot.setTriggerLevel(3000);
	pot.onTrigger(alarm);

	while (1) {
		cout << "Value: " << pot.read() << endl;
		sleep(1);
	}
}


void alarm(void* p) {
	cout << "Alarm!" << endl;
}