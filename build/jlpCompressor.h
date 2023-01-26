/* ------------------------------------------------------------
author: "Jean-Loup Pecquais"
license: "GPL3"
name: "Compressor"
version: "1.00"
Code generated with Faust 2.50.6 (https://faust.grame.fr)
Compilation options: -a tools/faustMinimal.h -lang cpp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#include <cmath>
#include <cstring>

#include "faust/gui/MapUI.h"
#include "faust/gui/meta.h"
#include "faust/dsp/dsp.h"

// BEGIN-FAUSTDSP


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

static float mydsp_faustpower2_f(float value) {
	return value * value;
}

class mydsp : public dsp {
	
 private:
	
	int IOTA0;
	float fVec0[32768];
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst0;
	FAUSTFLOAT fHslider1;
	float fConst1;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fHslider3;
	float fConst4;
	float fConst5;
	float fConst7;
	float fConst8;
	float fConst9;
	float fConst10;
	float fRec5[3];
	float fConst11;
	float fRec4[3];
	float fRec3[1048576];
	FAUSTFLOAT fHslider4;
	int iRec2[2];
	FAUSTFLOAT fHslider5;
	int iRec6[2];
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fHbargraph0;
	float fVec1[32768];
	float fRec10[3];
	float fRec9[3];
	float fRec8[1048576];
	int iRec7[2];
	int iRec11[2];
	FAUSTFLOAT fHbargraph1;
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Jean-Loup Pecquais");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/parallelMin:author", "Bart Brouns");
		m->declare("basics.lib/parallelMin:copyright", "Copyright (c) 2020 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/parallelMin:licence", "GPL-3.0");
		m->declare("basics.lib/parallelOp:author", "Bart Brouns");
		m->declare("basics.lib/parallelOp:copyright", "Copyright (c) 2020 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/parallelOp:licence", "GPL-3.0");
		m->declare("basics.lib/version", "0.8");
		m->declare("compile_options", "-a tools/faustMinimal.h -lang cpp -es 1 -mcd 16 -single -ftz 0");
		m->declare("filename", "jlpCompressor.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/integrator:author", "Julius O. Smith III");
		m->declare("filters.lib/integrator:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/integrator:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "0.3");
		m->declare("interpolators.lib/interpolate_linear:author", "Stéphane Letz");
		m->declare("interpolators.lib/interpolate_linear:licence", "MIT");
		m->declare("interpolators.lib/name", "Faust Interpolator Library");
		m->declare("interpolators.lib/version", "0.3");
		m->declare("jlpEnvelop.lib/ahr:author", "Jean-Loup Pecquais");
		m->declare("jlpEnvelop.lib/ahr:licence", "GPL3");
		m->declare("jlpEnvelop.lib/author", "Jean-Loup Pecquais");
		m->declare("jlpEnvelop.lib/license", "GPL3");
		m->declare("jlpEnvelop.lib/name", "JLP Envelop Library");
		m->declare("lib/jlpDynamics.lib/author", "Jean-Loup Pecquais");
		m->declare("lib/jlpDynamics.lib/license", "GPL3");
		m->declare("lib/jlpDynamics.lib/name", "JLP Envelop Library");
		m->declare("license", "GPL3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "Compressor");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.3");
		m->declare("version", "1.00");
	}

	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 0.001f * fConst0;
		float fConst2 = std::tan(31.415926f / fConst0);
		float fConst3 = mydsp_faustpower2_f(fConst2);
		fConst4 = 1.0f / fConst3;
		fConst5 = 2.0f * (1.0f - fConst4);
		float fConst6 = 1.0f / fConst2;
		fConst7 = (fConst6 + -0.76536685f) / fConst2 + 1.0f;
		fConst8 = 1.0f / ((fConst6 + 0.76536685f) / fConst2 + 1.0f);
		fConst9 = (fConst6 + -1.847759f) / fConst2 + 1.0f;
		fConst10 = 1.0f / ((fConst6 + 1.847759f) / fConst2 + 1.0f);
		fConst11 = 0.0f - 2.0f / fConst3;
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(0.0f);
		fHslider3 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(0.0f);
		fHslider5 = FAUSTFLOAT(0.0f);
		fHslider6 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		IOTA0 = 0;
		for (int l0 = 0; l0 < 32768; l0 = l0 + 1) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 3; l1 = l1 + 1) {
			fRec5[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 3; l2 = l2 + 1) {
			fRec4[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 1048576; l3 = l3 + 1) {
			fRec3[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iRec2[l4] = 0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iRec6[l5] = 0;
		}
		for (int l6 = 0; l6 < 32768; l6 = l6 + 1) {
			fVec1[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 3; l7 = l7 + 1) {
			fRec10[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = l8 + 1) {
			fRec9[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 1048576; l9 = l9 + 1) {
			fRec8[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			iRec7[l10] = 0;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			iRec11[l11] = 0;
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
		ui_interface->openVerticalBox("Compressor");
		ui_interface->addHorizontalSlider("Attack", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.2e+02f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHbargraph0, "1", "");
		ui_interface->declare(&fHbargraph0, "tooltip", "gain reduction in dB");
		ui_interface->declare(&fHbargraph0, "unit", "dB");
		ui_interface->addHorizontalBargraph("GainReduction", &fHbargraph0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f));
		ui_interface->declare(&fHbargraph1, "1", "");
		ui_interface->declare(&fHbargraph1, "tooltip", "gain reduction in dB");
		ui_interface->declare(&fHbargraph1, "unit", "dB");
		ui_interface->addHorizontalBargraph("GainReduction", &fHbargraph1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f));
		ui_interface->addHorizontalSlider("Hold", &fHslider5, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(3e+03f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("Knee", &fHslider2, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(24.0f), FAUSTFLOAT(0.1f));
		ui_interface->addCheckButton("Perfect attack", &fCheckbox0);
		ui_interface->addHorizontalSlider("RMS Size", &fHslider4, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(3e+03f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("Release", &fHslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(3e+03f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("Strenght", &fHslider6, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("Threshold", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(-96.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fElse0 = 0.001f * fSlow0;
		int iSlow1 = int(std::max<float>(0.0f, std::floor(fConst0 * ((int(float(fCheckbox0))) ? fElse0 : 0.0f) + 0.5f)));
		float fSlow2 = std::max<float>(1.0f, fConst1 * float(fHslider1));
		float fSlow3 = float(fHslider2);
		float fSlow4 = 0.5f * fSlow3;
		float fSlow5 = float(fHslider3);
		float fSlow6 = fSlow5 - fSlow4;
		int iSlow7 = std::max<int>(1, int(fConst1 * float(fHslider4)));
		int iSlow8 = std::max<int>(0, iSlow7);
		float fSlow9 = 1.0f / float(iSlow7);
		float fSlow10 = 1.0f / std::max<float>(1.0f, fConst1 * float(fHslider5));
		float fSlow11 = 1.0f / std::max<float>(1.0f, fConst1 * fSlow0);
		float fSlow12 = fSlow5 + fSlow4;
		float fSlow13 = 0.5f / fSlow3;
		float fSlow14 = float(fHslider6);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = float(input0[i0]);
			fVec0[IOTA0 & 32767] = fTemp0;
			fRec5[0] = fTemp0 - fConst10 * (fConst9 * fRec5[2] + fConst5 * fRec5[1]);
			fRec4[0] = fConst10 * (fConst11 * fRec5[1] + fConst4 * fRec5[0] + fConst4 * fRec5[2]) - fConst8 * (fConst7 * fRec4[2] + fConst5 * fRec4[1]);
			fRec3[IOTA0 & 1048575] = fRec3[(IOTA0 - 1) & 1048575] + mydsp_faustpower2_f(fConst8 * (fConst4 * fRec4[0] + fConst11 * fRec4[1] + fConst4 * fRec4[2]));
			float fTemp1 = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, std::sqrt(fSlow9 * (fRec3[IOTA0 & 1048575] - fRec3[(IOTA0 - iSlow8) & 1048575]))));
			iRec2[0] = (fTemp1 <= fSlow6) * (iRec2[1] + 1);
			float fTemp2 = float(iRec2[0]);
			float fThen1 = fSlow10 * (fSlow2 + fTemp2);
			iRec6[0] = (fTemp1 >= fSlow6) * (iRec6[1] + 1);
			int iTemp3 = (fTemp1 > fSlow6) + (fTemp1 > fSlow12);
			float fTemp4 = fTemp1 - fSlow5;
			float fElse2 = fSlow13 * mydsp_faustpower2_f(fSlow4 + fTemp4);
			float fThen3 = ((iTemp3 == 1) ? fElse2 : fTemp4);
			float fTemp5 = std::pow(1e+01f, 0.05f * (0.0f - fSlow14 * std::max<float>(0.0f, ((iTemp3 == 0) ? 0.0f : fThen3)))) * std::max<float>(0.0f, std::min<float>(fSlow11 * float(iRec6[0]), 1.0f) * (1.0f - ((fTemp2 <= fSlow2) ? 1.0f : fThen1)));
			fHbargraph0 = FAUSTFLOAT(std::max<float>(0.0f, 2e+01f * std::log10(std::max<float>(1.1754944e-38f, fTemp5))));
			float fRec0 = fTemp5 * fVec0[(IOTA0 - iSlow1) & 32767];
			float fTemp6 = float(input1[i0]);
			fVec1[IOTA0 & 32767] = fTemp6;
			fRec10[0] = fTemp6 - fConst10 * (fConst9 * fRec10[2] + fConst5 * fRec10[1]);
			fRec9[0] = fConst10 * (fConst4 * fRec10[0] + fConst11 * fRec10[1] + fConst4 * fRec10[2]) - fConst8 * (fConst7 * fRec9[2] + fConst5 * fRec9[1]);
			fRec8[IOTA0 & 1048575] = fRec8[(IOTA0 - 1) & 1048575] + mydsp_faustpower2_f(fConst8 * (fConst4 * fRec9[0] + fConst11 * fRec9[1] + fConst4 * fRec9[2]));
			float fTemp7 = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, std::sqrt(fSlow9 * (fRec8[IOTA0 & 1048575] - fRec8[(IOTA0 - iSlow8) & 1048575]))));
			iRec7[0] = (fTemp7 <= fSlow6) * (iRec7[1] + 1);
			float fTemp8 = float(iRec7[0]);
			float fThen4 = fSlow10 * (fSlow2 + fTemp8);
			iRec11[0] = (fTemp7 >= fSlow6) * (iRec11[1] + 1);
			int iTemp9 = (fTemp7 > fSlow6) + (fTemp7 > fSlow12);
			float fTemp10 = fTemp7 - fSlow5;
			float fElse5 = fSlow13 * mydsp_faustpower2_f(fSlow4 + fTemp10);
			float fThen6 = ((iTemp9 == 1) ? fElse5 : fTemp10);
			float fTemp11 = std::pow(1e+01f, 0.05f * (0.0f - fSlow14 * std::max<float>(0.0f, ((iTemp9 == 0) ? 0.0f : fThen6)))) * std::max<float>(0.0f, std::min<float>(fSlow11 * float(iRec11[0]), 1.0f) * (1.0f - ((fTemp8 <= fSlow2) ? 1.0f : fThen4)));
			fHbargraph1 = FAUSTFLOAT(std::max<float>(0.0f, 2e+01f * std::log10(std::max<float>(1.1754944e-38f, fTemp11))));
			float fRec1 = fTemp11 * fVec1[(IOTA0 - iSlow1) & 32767];
			output0[i0] = FAUSTFLOAT(fRec0);
			output1[i0] = FAUSTFLOAT(fRec1);
			IOTA0 = IOTA0 + 1;
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			iRec2[1] = iRec2[0];
			iRec6[1] = iRec6[0];
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			iRec7[1] = iRec7[0];
			iRec11[1] = iRec11[0];
		}
	}

};

// END-FAUSTDSP

#endif
