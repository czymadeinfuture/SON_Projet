/* ------------------------------------------------------------
name: "playerwithslider"
Code generated with Faust 2.72.4 (https://faust.grame.fr)
Compilation options: -lang cpp -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

class mydspSIG0 {
	
  private:
	
	int iVec0[2];
	int iRec1[2];
	
  public:
	
	int getNumInputsmydspSIG0() {
		return 0;
	}
	int getNumOutputsmydspSIG0() {
		return 1;
	}
	
	void instanceInitmydspSIG0(int sample_rate) {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			iRec1[l1] = 0;
		}
	}
	
	void fillmydspSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec0[0] = 1;
			iRec1[0] = (iVec0[1] + iRec1[1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec1[0]));
			iVec0[1] = iVec0[0];
			iRec1[1] = iRec1[0];
		}
	}

};

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)new mydspSIG0(); }
static void deletemydspSIG0(mydspSIG0* dsp) { delete dsp; }

static float ftbl0mydspSIG0[65536];

class mydsp : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int iVec1[2];
	FAUSTFLOAT fHslider1;
	int fSampleRate;
	float fConst1;
	float fRec2[2];
	FAUSTFLOAT fHslider2;
	int iRec3[2];
	FAUSTFLOAT fHslider3;
	float fVec2[2];
	FAUSTFLOAT fHslider4;
	float fConst2;
	float fRec0[2];
	FAUSTFLOAT fHslider5;
	
 public:
	mydsp() {}

	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.12.0");
		m->declare("compile_options", "-lang cpp -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "playerwithslider.dsp");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.3.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.7.0");
		m->declare("name", "playerwithslider");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.4.0");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.5.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
		mydspSIG0* sig0 = newmydspSIG0();
		sig0->instanceInitmydspSIG0(sample_rate);
		sig0->fillmydspSIG0(65536, ftbl0mydspSIG0);
		deletemydspSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 3.1415927f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.5f);
		fHslider1 = FAUSTFLOAT(0.5f);
		fHslider2 = FAUSTFLOAT(0.001f);
		fHslider3 = FAUSTFLOAT(0.02f);
		fHslider4 = FAUSTFLOAT(5e+03f);
		fHslider5 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			iVec1[l2] = 0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec2[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iRec3[l4] = 0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fVec2[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec0[l6] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual mydsp* clone() {
		return new mydsp();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("playerwithslider");
		ui_interface->addHorizontalSlider("Compressor Ratio", &fHslider5, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.1f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("Compressor Threshold", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.1f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("LFO Depth", &fHslider2, FAUSTFLOAT(0.001f), FAUSTFLOAT(0.0f), FAUSTFLOAT(5.0f), FAUSTFLOAT(0.0001f));
		ui_interface->declare(&fHslider1, "Hz", "");
		ui_interface->addHorizontalSlider("LFO Frequency", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.1f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider4, "Hz", "");
		ui_interface->addHorizontalSlider("LPF Cutoff Frequency", &fHslider4, FAUSTFLOAT(5e+03f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(2e+04f), FAUSTFLOAT(1e+02f));
		ui_interface->addHorizontalSlider("Noise Level", &fHslider3, FAUSTFLOAT(0.02f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fHslider0);
		float fSlow1 = fConst1 * float(fHslider1);
		float fSlow2 = float(fHslider2);
		float fSlow3 = 4.656613e-10f * float(fHslider3);
		float fSlow4 = 1.0f / std::tan(fConst2 * float(fHslider4));
		float fSlow5 = 1.0f - fSlow4;
		float fSlow6 = 1.0f / (fSlow4 + 1.0f);
		float fSlow7 = float(fHslider5);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec1[0] = 1;
			float fTemp0 = ((1 - iVec1[1]) ? 0.0f : fSlow1 + fRec2[1]);
			fRec2[0] = fTemp0 - std::floor(fTemp0);
			iRec3[0] = 1103515245 * iRec3[1] + 12345;
			float fTemp1 = fSlow3 * float(iRec3[0]) + fSlow2 * ftbl0mydspSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec2[0]), 65535))];
			fVec2[0] = fTemp1;
			fRec0[0] = -(fSlow6 * (fSlow5 * fRec0[1] - (fTemp1 + fVec2[1])));
			output0[i0] = FAUSTFLOAT(((std::fabs(fRec0[0]) > fSlow0) ? fSlow7 * fRec0[0] : fRec0[0]));
			iVec1[1] = iVec1[0];
			fRec2[1] = fRec2[0];
			iRec3[1] = iRec3[0];
			fVec2[1] = fVec2[0];
			fRec0[1] = fRec0[0];
		}
	}

};

#endif
