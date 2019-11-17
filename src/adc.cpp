#include "adc.h"
#include <iostream>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <pthread.h>

using namespace std;

namespace BBB {

ADC::ADC(int pin) {
	this->pin = pin;
	this->threadRunning = false;
	this->triggerMode = ADC::NONE;
	this->callbackFunction = NULL;
	this->callbackArgument = NULL;

	this->path = string(ADC_PATH) + "in_voltage" + to_string(pin) + "_raw";
}


ADC::~ADC() {

}


int ADC::getPin() {
	return this->pin;
}


int ADC::read() {
	return stoi(readFile());
}


void ADC::setTriggerMode(ADC::TRIGGER_MODE mode) {
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

		if ((adc->triggerMode == ADC::LOWER && value < adc->triggerLevel)
			|| (adc->triggerMode == ADC::HIGHER && value > adc->triggerLevel)) {
			adc->callbackFunction(adc->callbackArgument);
		}
	}
	return 0;
}


void ADC::stopTrigger() {
	this->threadRunning = false;
}

} /* namespace BBB */