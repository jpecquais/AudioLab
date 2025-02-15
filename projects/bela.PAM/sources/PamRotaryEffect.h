/* ------------------------------------------------------------
author: "Jean-Loup Pecquais"
copyright: "MyCompany"
name: "PamRotaryEffect"
version: "1.00"
Code generated with Faust 2.75.7 (https://faust.grame.fr)
Compilation options: -a ../../faust/tools/faustMinimalInlined.h -lang cpp -ct 1 -cn PamRotaryEffect -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __PamRotaryEffect_H__
#define  __PamRotaryEffect_H__

/* ------------------------------------------------------------
name: "FaustSynth"
Code generated with Faust 2.23.4 (https://faust.grame.fr)
Compilation options: -lang cpp -scal -ftz 0
------------------------------------------------------------ */

#include <cmath>
#include <cstring>

/************************** BEGIN MapUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <vector>
#include <map>
#include <string>

/************************** BEGIN UI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct UIReal
{
    UIReal() {}
    virtual ~UIReal() {}
    
    // -- widget's layouts
    
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    
    // -- active widgets
    
    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;
    
    // -- metadata declarations
    
    virtual void declare(REAL* zone, const char* key, const char* val) {}
};

struct UI : public UIReal<FAUSTFLOAT>
{
    UI() {}
    virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN PathBuilder.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_PATHBUILDER_H
#define FAUST_PATHBUILDER_H

#include <vector>
#include <string>
#include <algorithm>

/*******************************************************************************
 * PathBuilder : Faust User Interface
 * Helper class to build complete hierarchical path for UI items.
 ******************************************************************************/

class PathBuilder
{

    protected:
    
        std::vector<std::string> fControlsLevel;
       
    public:
    
        PathBuilder() {}
        virtual ~PathBuilder() {}
    
        std::string buildPath(const std::string& label) 
        {
            std::string res = "/";
            for (size_t i = 0; i < fControlsLevel.size(); i++) {
                res += fControlsLevel[i];
                res += "/";
            }
            res += label;
            std::replace(res.begin(), res.end(), ' ', '_');
            return res;
        }
    
        void pushLabel(const std::string& label) { fControlsLevel.push_back(label); }
        void popLabel() { fControlsLevel.pop_back(); }
    
};

#endif  // FAUST_PATHBUILDER_H
/**************************  END  PathBuilder.h **************************/

/*******************************************************************************
 * MapUI : Faust User Interface
 * This class creates a map of complete hierarchical path and zones for each UI items.
 ******************************************************************************/

class MapUI : public UI, public PathBuilder
{
    
    protected:
    
        // Complete path map
        std::map<std::string, FAUSTFLOAT*> fPathZoneMap;
    
        // Label zone map
        std::map<std::string, FAUSTFLOAT*> fLabelZoneMap;
    
    public:
        
        MapUI() {}
        virtual ~MapUI() {}
        
        // -- widget's layouts
        void openTabBox(const char* label)
        {
            pushLabel(label);
        }
        void openHorizontalBox(const char* label)
        {
            pushLabel(label);
        }
        void openVerticalBox(const char* label)
        {
            pushLabel(label);
        }
        void closeBox()
        {
            popLabel();
        }
        
        // -- active widgets
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        
        // -- passive widgets
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}
        
        // -- metadata declarations
        void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {}
        
        // set/get
        void setParamValue(const std::string& path, FAUSTFLOAT value)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                *fPathZoneMap[path] = value;
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                *fLabelZoneMap[path] = value;
            }
        }
        
        FAUSTFLOAT getParamValue(const std::string& path)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                return *fPathZoneMap[path];
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                return *fLabelZoneMap[path];
            } else {
                return FAUSTFLOAT(0);
            }
        }
    
        // map access 
        std::map<std::string, FAUSTFLOAT*>& getMap() { return fPathZoneMap; }
        
        int getParamsCount() { return int(fPathZoneMap.size()); }
        
        std::string getParamAddress(int index)
        { 
            std::map<std::string, FAUSTFLOAT*>::iterator it = fPathZoneMap.begin();
            while (index-- > 0 && it++ != fPathZoneMap.end()) {}
            return (*it).first;
        }
    
        std::string getParamAddress(FAUSTFLOAT* zone)
        {
            std::map<std::string, FAUSTFLOAT*>::iterator it = fPathZoneMap.begin();
            do {
                if ((*it).second == zone) return (*it).first;
            }
            while (it++ != fPathZoneMap.end());
            return "";
        }
    
        static bool endsWith(std::string const& str, std::string const& end)
        {
            size_t l1 = str.length();
            size_t l2 = end.length();
            return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
        }
};


#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
/************************** BEGIN meta.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

struct Meta
{
    virtual ~Meta() {};
    virtual void declare(const char* key, const char* value) = 0;
    
};

#endif
/**************************  END  meta.h **************************/
/************************** BEGIN dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'addBtton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Returns the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceInit(int sample_rate) = 0;

        /**
         * Init instance constant state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual dsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with successive in/out audio buffers.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation: alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         *
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = nullptr):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int sample_rate) { fDSP->init(sample_rate); }
        virtual void instanceInit(int sample_rate) { fDSP->instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) { fDSP->instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class.
 */

class dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() {}
    
    public:
    
        virtual std::string getName() = 0;
        virtual std::string getSHAKey() = 0;
        virtual std::string getDSPCode() = 0;
        virtual std::string getCompileOptions() = 0;
        virtual std::vector<std::string> getLibraryList() = 0;
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
        virtual dsp* createDSPInstance() = 0;
    
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */

#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif
/**************************  END  dsp.h **************************/

// BEGIN-FAUSTDSP



// END-FAUSTDSP

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS PamRotaryEffect
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


class PamRotaryEffect : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int iVec0[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	float fConst4;
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	float fConst10;
	float fRec16[2];
	float fRec17[2];
	int iRec18[2];
	float fRec15[2];
	float fConst11;
	float fRec14[2];
	float fRec13[2];
	float fRec12[2];
	float fRec11[2];
	float fRec10[2];
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	float fRec19[2];
	float fRec9[2];
	int IOTA0;
	float fRec20[32];
	float fRec21[32];
	FAUSTFLOAT fHslider6;
	float fConst12;
	float fConst13;
	float fConst14;
	float fVec1[2];
	float fRec22[2];
	float fVec2[256];
	float fVec3[128];
	float fVec4[2];
	float fRec8[2];
	float fRec4[3];
	float fConst15;
	float fRec24[2];
	float fVec5[2];
	float fRec23[2];
	float fRec5[3];
	float fVec6[2];
	float fVec7[2];
	float fRec3[2];
	float fRec2[2];
	float fVec8[2];
	float fRec1[2];
	float fRec0[2];
	float fConst16;
	float fConst17;
	float fRec25[2];
	float fVec9[2];
	float fRec29[2];
	float fRec28[2];
	float fVec10[2];
	float fRec27[2];
	float fRec26[2];
	float fRec30[2];
	float fRec35[2];
	float fVec11[2];
	float fVec12[2];
	float fRec34[2];
	float fRec33[2];
	float fVec13[2];
	float fRec32[2];
	float fRec31[2];
	float fVec14[2];
	float fVec15[2];
	float fRec39[2];
	float fRec38[2];
	float fVec16[2];
	float fRec37[2];
	float fRec36[2];
	float fRec40[2];
	float fVec17[2];
	float fRec44[2];
	float fRec43[2];
	float fVec18[2];
	float fRec42[2];
	float fRec41[2];
	float fRec45[2];
	
 public:
	PamRotaryEffect() {
	}
	
	void metadata(Meta* m) { 
		m->declare("../../faust/modules/modulations/vibrator.dsp/author", "Jean-Loup Pecquais");
		m->declare("../../faust/modules/modulations/vibrator.dsp/name", "Vibrator");
		m->declare("../../faust/modules/modulations/vibrator.dsp/version", "1.00");
		m->declare("author", "Jean-Loup Pecquais");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.19.1");
		m->declare("compile_options", "-a ../../faust/tools/faustMinimalInlined.h -lang cpp -ct 1 -cn PamRotaryEffect -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("copyright", "MyCompany");
		m->declare("delays.lib/fdelay1:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.1.0");
		m->declare("filename", "PamRotaryEffect.dsp");
		m->declare("filter.lib/author", "Julius O. Smith (jos at ccrma.stanford.edu)");
		m->declare("filter.lib/copyright", "Julius O. Smith III");
		m->declare("filter.lib/deprecated", "This library is deprecated and is not maintained anymore. It will be removed in August 2017.");
		m->declare("filter.lib/license", "STK-4.3");
		m->declare("filter.lib/name", "Faust Filter Library");
		m->declare("filter.lib/reference", "https://ccrma.stanford.edu/~jos/filters/");
		m->declare("filter.lib/version", "1.29");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/nlf2:author", "Julius O. Smith III");
		m->declare("filters.lib/nlf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/nlf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf22t:author", "Julius O. Smith III");
		m->declare("filters.lib/tf22t:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf22t:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.3.0");
		m->declare("math.lib/author", "GRAME");
		m->declare("math.lib/copyright", "GRAME");
		m->declare("math.lib/deprecated", "This library is deprecated and is not maintained anymore. It will be removed in August 2017.");
		m->declare("math.lib/license", "LGPL with exception");
		m->declare("math.lib/name", "Math Library");
		m->declare("math.lib/version", "1.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("music.lib/author", "GRAME");
		m->declare("music.lib/copyright", "GRAME");
		m->declare("music.lib/deprecated", "This library is deprecated and is not maintained anymore. It will be removed in August 2017.");
		m->declare("music.lib/license", "LGPL with exception");
		m->declare("music.lib/name", "Music Library");
		m->declare("music.lib/version", "1.0");
		m->declare("name", "PamRotaryEffect");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.4.1");
		m->declare("oscillators.lib/hsp_phasor:author", "Christophe Lebreton, revised by Stéphane Letz");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.5.1");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.6.0");
		m->declare("version", "1.00");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 1.0f / std::tan(14137.167f / fConst0);
		fConst2 = 1.0f / (fConst1 + 1.0f);
		fConst3 = 1.0f - fConst1;
		fConst4 = 0.0002647059f * fConst0;
		fConst5 = 1.0f / fConst0;
		fConst6 = 1.0f / std::tan(4.712389f / fConst0);
		fConst7 = 1.0f / (fConst6 + 1.0f);
		fConst8 = 9.424778f / fConst0;
		fConst9 = std::sin(fConst8);
		fConst10 = std::cos(fConst8);
		fConst11 = 1.0f - fConst6;
		fConst12 = 1.0f / std::tan(3769.9111f / fConst0);
		fConst13 = 1.0f / (fConst12 + 1.0f);
		fConst14 = 1.0f - fConst12;
		fConst15 = 1.2f / fConst0;
		fConst16 = 44.1f / fConst0;
		fConst17 = 1.0f - fConst16;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(4e+01f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(0.0f);
		fHslider3 = FAUSTFLOAT(0.6f);
		fHslider4 = FAUSTFLOAT(6.0f);
		fHslider5 = FAUSTFLOAT(3.0f);
		fHslider6 = FAUSTFLOAT(5e+01f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec16[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec17[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iRec18[l3] = 0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec15[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec14[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec13[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec12[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec11[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec10[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec19[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec9[l11] = 0.0f;
		}
		IOTA0 = 0;
		for (int l12 = 0; l12 < 32; l12 = l12 + 1) {
			fRec20[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 32; l13 = l13 + 1) {
			fRec21[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fVec1[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec22[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 256; l16 = l16 + 1) {
			fVec2[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 128; l17 = l17 + 1) {
			fVec3[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fVec4[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec8[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 3; l20 = l20 + 1) {
			fRec4[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec24[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fVec5[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec23[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 3; l24 = l24 + 1) {
			fRec5[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fVec6[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fVec7[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec3[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fRec2[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fVec8[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec1[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fRec0[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec25[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fVec9[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			fRec29[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec28[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fVec10[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fRec27[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec26[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec30[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fRec35[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fVec11[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fVec12[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fRec34[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fRec33[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fVec13[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fRec32[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			fRec31[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fVec14[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			fVec15[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fRec39[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fRec38[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fVec16[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec37[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fRec36[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec40[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fVec17[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fRec44[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec43[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fVec18[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fRec42[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec41[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 2; l62 = l62 + 1) {
			fRec45[l62] = 0.0f;
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
	
	virtual PamRotaryEffect* clone() {
		return new PamRotaryEffect();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("PamRotaryEffect");
		ui_interface->addHorizontalSlider("break", &fHslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("fast_rotation_speed", &fHslider4, FAUSTFLOAT(6.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("mic_distance", &fHslider0, FAUSTFLOAT(4e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("mix", &fHslider6, FAUSTFLOAT(5e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("ramp_time", &fHslider5, FAUSTFLOAT(3.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("slow_fast", &fHslider2, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("slow_rotation_speed", &fHslider3, FAUSTFLOAT(0.6f), FAUSTFLOAT(0.1f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = 1.0f - 0.01f * float(fHslider0);
		float fSlow1 = 0.5f * std::min<float>(1.0f, std::max<float>(0.0f, fSlow0));
		float fSlow2 = 1.0f - fSlow1;
		float fSlow3 = float(fHslider5);
		int iSlow4 = std::fabs(fSlow3) < 1.1920929e-07f;
		float fSlow5 = ((iSlow4) ? 0.0f : std::exp(-(fConst5 / ((iSlow4) ? 1.0f : fSlow3))));
		float fSlow6 = (1.0f - float(fHslider1)) * ((int(float(fHslider2))) ? float(fHslider4) : float(fHslider3)) * (1.0f - fSlow5);
		float fSlow7 = 1.4125376f * std::pow(1e+01f, 0.15f * fSlow0);
		float fSlow8 = 0.01f * float(fHslider6);
		float fSlow9 = std::sqrt(fSlow8);
		float fSlow10 = std::sqrt(1.0f - fSlow8);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			iVec0[0] = 1;
			int iTemp0 = 1 - iVec0[1];
			fRec16[0] = fConst9 * fRec17[1] + fConst10 * fRec16[1];
			fRec17[0] = float(iTemp0) + fConst10 * fRec17[1] - fConst9 * fRec16[1];
			int iTemp1 = (fRec16[1] <= 0.0f) & (fRec16[0] > 0.0f);
			iRec18[0] = 1103515245 * iRec18[1] + 12345;
			fRec15[0] = float(1 - iTemp1) * fRec15[1] + 4.656613e-10f * float(iTemp1) * float(iRec18[0]);
			fRec14[0] = fConst7 * (fRec15[0] + fRec15[1] - fConst11 * fRec14[1]);
			fRec13[0] = fConst7 * (fRec14[0] + fRec14[1] - fConst11 * fRec13[1]);
			fRec12[0] = fConst7 * (fRec13[0] + fRec13[1] - fConst11 * fRec12[1]);
			fRec11[0] = fConst7 * (fRec12[0] + fRec12[1] - fConst11 * fRec11[1]);
			fRec10[0] = fConst7 * (fRec11[0] + fRec11[1] - fConst11 * fRec10[1]);
			float fTemp2 = 0.125f * fRec10[0] + 1.0f;
			fRec19[0] = fSlow6 + fSlow5 * fRec19[1];
			float fTemp3 = fTemp2 * fRec19[0];
			float fTemp4 = ((iTemp0) ? 1.5707964f : fRec9[1] - fConst5 * fTemp3);
			fRec9[0] = fTemp4 - std::floor(fTemp4);
			float fTemp5 = std::cos(6.2831855f * fRec9[0] + -3.1415927f);
			float fTemp6 = fConst4 * (fTemp5 + 1.0f);
			float fTemp7 = fTemp6 + 5e-06f;
			float fTemp8 = std::floor(fTemp7);
			fRec20[IOTA0 & 31] = -(fConst2 * (fConst3 * fRec20[(IOTA0 - 1) & 31] + 0.16666667f * (fRec5[1] + fRec5[2])));
			fRec21[IOTA0 & 31] = -(fConst2 * (fConst3 * fRec21[(IOTA0 - 1) & 31] + 0.16666667f * (fRec4[1] + fRec4[2])));
			float fTemp9 = 48.0f * fTemp2;
			float fTemp10 = fTemp9 + 5e-06f;
			float fTemp11 = std::floor(fTemp10);
			float fTemp12 = float(input0[i0]);
			fVec1[0] = fTemp12;
			fRec22[0] = -(fConst13 * (fConst14 * fRec22[1] - fConst12 * (fTemp12 - fVec1[1])));
			float fTemp13 = fSlow9 * fRec22[0];
			fVec2[IOTA0 & 255] = fTemp13;
			int iTemp14 = int(fTemp10);
			float fTemp15 = 0.5f * (fRec20[(IOTA0 - 31) & 31] + fRec21[(IOTA0 - 11) & 31] + fRec21[(IOTA0 - 23) & 31] + fRec20[(IOTA0 - 17) & 31] + fRec21[IOTA0 & 31] + fRec20[IOTA0 & 31]) + fSlow7 * ((fTemp9 - fTemp11) * fVec2[(IOTA0 - std::min<int>(192, std::max<int>(0, iTemp14 + 1))) & 255] - fVec2[(IOTA0 - std::min<int>(192, std::max<int>(0, iTemp14))) & 255] * (fTemp9 + (-1.0f - fTemp11)));
			fVec3[IOTA0 & 127] = fTemp15;
			int iTemp16 = int(fTemp7);
			float fTemp17 = (fTemp6 - fTemp8) * fVec3[(IOTA0 - std::min<int>(192, std::max<int>(0, iTemp16 + 1))) & 127] - fVec3[(IOTA0 - std::min<int>(192, std::max<int>(0, iTemp16))) & 127] * (fTemp6 + (-1.0f - fTemp8));
			fVec4[0] = fTemp17;
			fRec8[0] = -(fConst2 * (fConst3 * fRec8[1] - (fTemp17 + fVec4[1])));
			fRec4[0] = fRec8[0];
			float fTemp18 = ((iTemp0) ? 1.5707964f : fRec24[1] - fConst15 * fTemp3);
			fRec24[0] = fTemp18 - std::floor(fTemp18);
			float fTemp19 = std::cos(6.2831855f * fRec24[0] + -3.1415927f);
			float fTemp20 = fConst4 * (fTemp19 + 1.0f);
			float fTemp21 = fTemp20 + 7.000005f;
			float fTemp22 = std::floor(fTemp21);
			int iTemp23 = int(fTemp21);
			float fTemp24 = (fTemp20 + (7.0f - fTemp22)) * fVec3[(IOTA0 - std::min<int>(192, std::max<int>(0, iTemp23 + 1))) & 127] - fVec3[(IOTA0 - std::min<int>(192, std::max<int>(0, iTemp23))) & 127] * (fTemp20 + (6.0f - fTemp22));
			fVec5[0] = fTemp24;
			fRec23[0] = -(fConst2 * (fConst3 * fRec23[1] - (fTemp24 + fVec5[1])));
			fRec5[0] = fRec23[0];
			float fRec6 = 0.5f * (fTemp15 + fVec3[(IOTA0 - 11) & 127] + fVec3[(IOTA0 - 23) & 127]);
			float fRec7 = 0.5f * (fTemp15 + fVec3[(IOTA0 - 17) & 127] + fVec3[(IOTA0 - 31) & 127]);
			float fTemp25 = fRec5[0] + fRec7;
			fVec6[0] = fTemp25;
			float fTemp26 = 0.70710677f * fVec6[1];
			fVec7[0] = 0.260502f * fRec2[1] + fTemp26;
			fRec3[0] = fVec7[1] + 0.018165573f * fVec6[1] - (0.02569f * fRec3[1] + 0.18420273f * fTemp25);
			fRec2[0] = fRec3[0];
			fVec8[0] = fRec2[1] - 0.870686f * fRec0[1];
			fRec1[0] = 0.870686f * fRec2[0] + fVec8[1] + 1.8685f * (fRec1[1] - fRec2[1]);
			fRec0[0] = fRec1[0];
			float fTemp27 = 3.1415927f * fTemp19;
			fRec25[0] = fConst16 * std::sin(fTemp27) + fConst17 * fRec25[1];
			fVec9[0] = fTemp26 - 0.94657f * fRec28[1];
			fRec29[0] = 1.94632f * fRec29[1] + fVec9[1] + 0.66932607f * fTemp25 - 1.3762561f * fVec6[1];
			fRec28[0] = fRec29[0];
			fVec10[0] = fRec28[1] - 0.06338f * fRec26[1];
			fRec27[0] = 0.06338f * fRec28[0] + fVec10[1] + 0.83774f * (fRec27[1] - fRec28[1]);
			fRec26[0] = fRec27[0];
			fRec30[0] = fConst16 * std::cos(fTemp27) + fConst17 * fRec30[1];
			float fTemp28 = 0.70710677f * (fSlow2 * fRec0[0] + fSlow1 * (fRec25[0] * fRec26[0] + fRec0[0] * fRec30[0]));
			fRec35[0] = -(fConst13 * (fConst14 * fRec35[1] - (fTemp12 + fVec1[1])));
			float fTemp29 = fRec35[0] + fSlow10 * fRec22[0];
			fVec11[0] = fTemp29;
			fVec12[0] = 0.260502f * fRec33[1] + fVec11[1];
			fRec34[0] = fVec12[1] - (0.02569f * (fRec34[1] - fVec11[1]) + 0.260502f * fTemp29);
			fRec33[0] = fRec34[0];
			fVec13[0] = fRec33[1] - 0.870686f * fRec31[1];
			fRec32[0] = 1.8685f * (fRec32[1] - fRec33[1]) + fVec13[1] + 0.870686f * fRec33[0];
			fRec31[0] = fRec32[0];
			output0[i0] = FAUSTFLOAT(fTemp28 + fRec31[0]);
			float fTemp30 = fRec4[0] + fRec6;
			fVec14[0] = fTemp30;
			float fTemp31 = 0.70710677f * fVec14[1];
			fVec15[0] = 0.260502f * fRec38[1] + fTemp31;
			fRec39[0] = fVec15[1] + 0.018165573f * fVec14[1] - (0.02569f * fRec39[1] + 0.18420273f * fTemp30);
			fRec38[0] = fRec39[0];
			fVec16[0] = fRec38[1] - 0.870686f * fRec36[1];
			fRec37[0] = 0.870686f * fRec38[0] + fVec16[1] + 1.8685f * (fRec37[1] - fRec38[1]);
			fRec36[0] = fRec37[0];
			float fTemp32 = 3.1415927f * fTemp5;
			fRec40[0] = fConst16 * std::cos(fTemp32) + fConst17 * fRec40[1];
			fVec17[0] = fTemp31 - 0.94657f * fRec43[1];
			fRec44[0] = 1.94632f * fRec44[1] + fVec17[1] + 0.66932607f * fTemp30 - 1.3762561f * fVec14[1];
			fRec43[0] = fRec44[0];
			fVec18[0] = fRec43[1] - 0.06338f * fRec41[1];
			fRec42[0] = 0.06338f * fRec43[0] + fVec18[1] + 0.83774f * (fRec42[1] - fRec43[1]);
			fRec41[0] = fRec42[0];
			fRec45[0] = fConst16 * std::sin(fTemp32) + fConst17 * fRec45[1];
			output1[i0] = FAUSTFLOAT(fSlow2 * fRec36[0] + fSlow1 * (fRec36[0] * fRec40[0] + fRec41[0] * fRec45[0]) + fTemp28 + fRec31[0]);
			iVec0[1] = iVec0[0];
			fRec16[1] = fRec16[0];
			fRec17[1] = fRec17[0];
			iRec18[1] = iRec18[0];
			fRec15[1] = fRec15[0];
			fRec14[1] = fRec14[0];
			fRec13[1] = fRec13[0];
			fRec12[1] = fRec12[0];
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			fRec19[1] = fRec19[0];
			fRec9[1] = fRec9[0];
			IOTA0 = IOTA0 + 1;
			fVec1[1] = fVec1[0];
			fRec22[1] = fRec22[0];
			fVec4[1] = fVec4[0];
			fRec8[1] = fRec8[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec24[1] = fRec24[0];
			fVec5[1] = fVec5[0];
			fRec23[1] = fRec23[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fVec6[1] = fVec6[0];
			fVec7[1] = fVec7[0];
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			fVec8[1] = fVec8[0];
			fRec1[1] = fRec1[0];
			fRec0[1] = fRec0[0];
			fRec25[1] = fRec25[0];
			fVec9[1] = fVec9[0];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fVec10[1] = fVec10[0];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			fRec30[1] = fRec30[0];
			fRec35[1] = fRec35[0];
			fVec11[1] = fVec11[0];
			fVec12[1] = fVec12[0];
			fRec34[1] = fRec34[0];
			fRec33[1] = fRec33[0];
			fVec13[1] = fVec13[0];
			fRec32[1] = fRec32[0];
			fRec31[1] = fRec31[0];
			fVec14[1] = fVec14[0];
			fVec15[1] = fVec15[0];
			fRec39[1] = fRec39[0];
			fRec38[1] = fRec38[0];
			fVec16[1] = fVec16[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fRec40[1] = fRec40[0];
			fVec17[1] = fVec17[0];
			fRec44[1] = fRec44[0];
			fRec43[1] = fRec43[0];
			fVec18[1] = fVec18[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fRec45[1] = fRec45[0];
		}
	}

};

#endif
