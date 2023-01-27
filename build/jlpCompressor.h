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
	float fConst9;
	float fConst10;
	float fConst11;
	float fRec8[3];
	float fConst12;
	float fRec7[3];
	float fVec1[2];
	float fConst15;
	float fConst16;
	float fConst18;
	float fConst20;
	float fConst21;
	float fConst22;
	float fVec2[2];
	float fRec6[2];
	float fRec5[2];
	float fConst25;
	float fConst27;
	float fVec3[2];
	float fConst29;
	float fConst30;
	float fRec4[2];
	float fRec3[2];
	float fRec2[1048576];
	FAUSTFLOAT fHslider4;
	int iRec1[2];
	FAUSTFLOAT fHslider5;
	int iRec9[2];
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fHbargraph0;
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Jean-Loup Pecquais");
		m->declare("basics.lib/name", "Faust Basic Element Library");
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
		m->declare("filters.lib/itu_r_bs_1770_4_kfilter:author", "Jakob Dübel");
		m->declare("filters.lib/itu_r_bs_1770_4_kfilter:copyright", "Copyright (C) 2022 Jakob Dübel");
		m->declare("filters.lib/itu_r_bs_1770_4_kfilter:license", "ISC license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf22t:author", "Julius O. Smith III");
		m->declare("filters.lib/tf22t:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf22t:license", "MIT-style STK-4.3 license");
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
		return 1;
	}
	virtual int getNumOutputs() {
		return 1;
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
		float fConst8 = (fConst6 + 0.76536685f) / fConst2 + 1.0f;
		fConst9 = 1.0f / fConst8;
		fConst10 = (fConst6 + -1.847759f) / fConst2 + 1.0f;
		fConst11 = 1.0f / ((fConst6 + 1.847759f) / fConst2 + 1.0f);
		fConst12 = 0.0f - 2.0f / fConst3;
		float fConst13 = std::tan(5283.415f / fConst0);
		float fConst14 = mydsp_faustpower2_f(fConst13);
		fConst15 = (fConst14 + -1.5848527f) / fConst8;
		fConst16 = fConst14 + -1.0f;
		float fConst17 = 1.7803667f * fConst13;
		fConst18 = (fConst14 + fConst17 + 1.5848527f) / fConst8;
		float fConst19 = 1.4142135f * fConst13;
		fConst20 = 1.0f / (fConst14 + fConst19 + 1.0f);
		fConst21 = (fConst14 + (1.5848527f - fConst17)) / fConst8;
		fConst22 = fConst14 + (1.0f - fConst19);
		float fConst23 = std::tan(119.806114f / fConst0);
		float fConst24 = mydsp_faustpower2_f(fConst23);
		fConst25 = 1.0006541f * (fConst24 + -1.0f);
		float fConst26 = 0.50032705f * (fConst24 + 1.0f);
		fConst27 = fConst26 - fConst23;
		float fConst28 = fConst23 + fConst26;
		fConst29 = 1.0f / fConst28;
		fConst30 = 0.0f - 1.0006541f / fConst28;
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
			fRec8[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 3; l2 = l2 + 1) {
			fRec7[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fVec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fVec2[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec6[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec5[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fVec3[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec4[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec3[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 1048576; l10 = l10 + 1) {
			fRec2[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			iRec1[l11] = 0;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			iRec9[l12] = 0;
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
		FAUSTFLOAT* output0 = outputs[0];
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
			fRec8[0] = fTemp0 - fConst11 * (fConst10 * fRec8[2] + fConst5 * fRec8[1]);
			fRec7[0] = fConst11 * (fConst12 * fRec8[1] + fConst4 * fRec8[0] + fConst4 * fRec8[2]) - fConst9 * (fConst7 * fRec7[2] + fConst5 * fRec7[1]);
			float fTemp1 = fConst12 * fRec7[1] + fConst4 * fRec7[0] + fConst4 * fRec7[2];
			fVec1[0] = fTemp1;
			fVec2[0] = 0.0f - fConst20 * (fConst22 * fRec5[1] - fConst21 * fVec1[1]);
			fRec6[0] = fVec2[1] + fConst20 * (fConst18 * fTemp1 - 2.0f * (fConst16 * fRec6[1] - fConst15 * fVec1[1]));
			fRec5[0] = fRec6[0];
			fVec3[0] = 0.50032705f * fRec5[1] - fConst27 * fRec3[1];
			fRec4[0] = fConst30 * fRec5[1] + fConst29 * (fVec3[1] - fConst25 * fRec4[1] + 0.50032705f * fRec5[0]);
			fRec3[0] = fRec4[0];
			fRec2[IOTA0 & 1048575] = fRec2[(IOTA0 - 1) & 1048575] + mydsp_faustpower2_f(0.92736715f * fRec3[0]);
			float fTemp2 = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, std::sqrt(fSlow9 * (fRec2[IOTA0 & 1048575] - fRec2[(IOTA0 - iSlow8) & 1048575]))));
			iRec1[0] = (fTemp2 <= fSlow6) * (iRec1[1] + 1);
			float fTemp3 = float(iRec1[0]);
			float fThen1 = fSlow10 * (fSlow2 + fTemp3);
			iRec9[0] = (fTemp2 >= fSlow6) * (iRec9[1] + 1);
			int iTemp4 = (fTemp2 > fSlow6) + (fTemp2 > fSlow12);
			float fTemp5 = fTemp2 - fSlow5;
			float fElse2 = fSlow13 * mydsp_faustpower2_f(fSlow4 + fTemp5);
			float fThen3 = ((iTemp4 == 1) ? fElse2 : fTemp5);
			float fTemp6 = std::pow(1e+01f, 0.05f * (0.0f - fSlow14 * std::max<float>(0.0f, ((iTemp4 == 0) ? 0.0f : fThen3)))) * std::max<float>(0.0f, std::min<float>(fSlow11 * float(iRec9[0]), 1.0f) * (1.0f - ((fTemp3 <= fSlow2) ? 1.0f : fThen1)));
			fHbargraph0 = FAUSTFLOAT(std::max<float>(0.0f, 2e+01f * std::log10(std::max<float>(1.1754944e-38f, fTemp6))));
			float fRec0 = fTemp6 * fVec0[(IOTA0 - iSlow1) & 32767];
			output0[i0] = FAUSTFLOAT(fRec0);
			IOTA0 = IOTA0 + 1;
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fVec1[1] = fVec1[0];
			fVec2[1] = fVec2[0];
			fRec6[1] = fRec6[0];
			fRec5[1] = fRec5[0];
			fVec3[1] = fVec3[0];
			fRec4[1] = fRec4[0];
			fRec3[1] = fRec3[0];
			iRec1[1] = iRec1[0];
			iRec9[1] = iRec9[0];
		}
	}

};

// END-FAUSTDSP

#endif
