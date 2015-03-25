#include <math.h>
#include "RuSine.h"
using namespace RackoonIO;
using namespace ExampleCode;
#define PHASE 0

RuSine::RuSine()
: RackUnit(std::string("RuSine")) {
	workState = IDLE;
	mBlockSize = 512;
	mAmplitude = 8000;
	mWaveSample = mWaveTime = 0.0f;
	mFs = 44100;
	mF1 = 0;
	mFn = mF0 = mFreq = 220;
	mSamplePeriod = (float)1/mFs;
	mInstPhase = mLastPhase = 0;

	addPlug("sinewave");
	addJack("power", JACK_AC);

	/* We use this method to export a method
	 * representing the action 'freq' so it
	 * can be bound to a MIDI controller
	 * in the configuration file
	 */
	MIDI_BIND("freq", RuSine::midiFrequency);
}

FeedState RuSine::feed(Jack *jack) {
	return FEED_OK;
}

void RuSine::setConfig(string config, string value) {
}

RackState RuSine::init() {
	CONSOLE_MSG("RuSine", "Frequence " << mFreq << " hz");
	CONSOLE_MSG("RuSine", "Amplitude " << mAmplitude);
	CONSOLE_MSG("RuSine", "Sample Period " << mSamplePeriod << " seconds/sample");
	mSinewaveJack = getPlug("sinewave")->jack;
	mSinewaveJack->frames = mBlockSize;
	mInstPhase = 0;
	workState = READY;
	CONSOLE_MSG("RuSine", "Initialised");
	return RACK_UNIT_OK;
}

RackState RuSine::cycle() {
	if(workState == READY) writeFrames();


	workState = (mSinewaveJack->feed(mPeriod) == FEED_OK)
		? READY : WAITING;
	return RACK_UNIT_OK;
}

void RuSine::block(Jack *jack) {
}

void RuSine::writeFrames() {
	mRecombobulate.lock();
	/* The output is interleaved so
	 * what we need to do is output the
	 * same value on both channels
	 */
	mPeriod = cacheAlloc(1);


	if(mF1 != mFn) {
		mF1 = mFn;
		mDelta = (2*M_PI*mF1)/mFs;
	}

	for(int i = 0; i < mBlockSize; i++) {
		mInstPhase = fmod((mLastPhase+mDelta), (2*M_PI));
		mLastPhase = mInstPhase;
		PcmSample y = (PcmSample) sin(mInstPhase);
		mPeriod[i++] = y;
		mPeriod[i] = y;
		mWaveTime += mSamplePeriod;
	}
	mRecombobulate.unlock();
}

/* Serious chirp problems */
void RuSine::modulatePhase() {

}
/** This method can be bound to a MIDI controller
 */
void RuSine::midiFrequency(int value) {
	mRecombobulate.lock();

	if(value == 64) {
		mFn = mF0;
	} else
	if(value < 64) {
		mFn = mF0 - ((63-value)*2);
	} else {
		mFn = mF0 + (((value-64))*2);
	}

	std::cout << "Target: " << mF1 << "Hz "<<endl;
	mRecombobulate.unlock();
}
