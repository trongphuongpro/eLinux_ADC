#include "adc.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <pthread.h>

using namespace std;

namespace BBB {

ADC::ADC(int pin) {
	this->pin = pin;
	this->threadRunning = false;
	this->triggerMode = NONE;
	this->callbackFunction = NULL;
	this->callbackArgument = NULL;

	ostringstream s;
	s << "in_voltage_" << pin << "_raw";

	this->name = s.str();
	this->path = ADC_PATH + this->name;
}


ADC::~ADC() {

}


int ADC::getPin() {
	return this->pin;
}


int ADC::read() {
	return stoi(readFile());
}


void ADC::setTriggerMode(TRIGGER_MODE mode) {
	this->triggerMode = mode;
}


void ADC::setTriggerLevel(int value) {
	this->triggerLevel = value;
}


int ADC::onTrigger(CallbackType callback, void* arg) {
	this->threadRunning = true;
	this->callbackFunction = callback;
	this->callbackArgument = arg;

	if (pthread_create(&this->thread, 
						NULL,
						threadedPoll,
						this)) {

		perror("GPIO: Failed to create the poll thread");
    	this->threadRunning = false;
    	return -1;
	}

	return 0;
}


string ADC::readFile() {
	ifstream fs;
	fs.open((this->path).c_str());

	if (!fs.is_open()) {
		perror("ADC: read failed to open file ");
	}
	string input;
	getline(fs, input);
	fs.close();

	return input;
}


void* threadedPoll(void *object) {
	ADC* adc = static_cast<ADC*>(object);

	while (adc->threadRunning) {
		int value = adc->read();

		if ((adc->triggerMode == LOWER && value < adc->triggerLevel)
			|| (adc->triggerMode == HIGHER && value > adc->triggerLevel)) {
			adc->callbackFunction(adc->callbackArgument);
		}
	}
	return 0;
}


void ADC::stopTrigger() {
	this->threadRunning = false;
}

} /* namespace BBB */