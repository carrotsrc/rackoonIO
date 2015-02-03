#ifndef RULEVELS_H
#define RULEVELS_H

#include "framework/rack/RackUnit.h"
class RuLevels : public RackoonIO::RackUnit
{
	enum WorkState {
		IDLE,
		INIT,
		READY
	};

	int zs;
	WorkState workState;
	snd_pcm_t *handle;
	float masterGain;
	float fadeGain;
	short *finalPeriod;
	bool processed;

	void writeDebugPCM(short);

public:
	RuLevels();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
	void block(RackoonIO::Jack*);

	void midiMasterGain(int);
	void midiFadeGain(int);
};
#endif