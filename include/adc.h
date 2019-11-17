#ifndef __ADC__
#define __ADC__

#include <string>

#define ADC_PATH	"/sys/bus/iio/devices/iio:device0/"

namespace BBB {

typedef void (*CallbackType)(void*);


class ADC {
public:
	enum TRIGGER_MODE {NONE, LOWER, HIGHER};

	ADC(int pin);
	~ADC();

	virtual int getPin();
	virtual int read();
	virtual void setTriggerMode(ADC::TRIGGER_MODE mode);
	virtual void setTriggerLevel(int value);
	virtual int onTrigger(CallbackType, void* arg=NULL);
	virtual void stopTrigger();

private:
	int pin;
	int triggerLevel;
	bool threadRunning = false;
	ADC::TRIGGER_MODE triggerMode = NONE;
	
	std::string path;

	CallbackType callbackFunction;
	void* callbackArgument;

	pthread_t thread;

	std::string readFile();

	friend void* threadedPoll(void *value);
};

void* threadedPoll(void *value);

} /* namespace BBB */
#endif /* __ADC__ */
