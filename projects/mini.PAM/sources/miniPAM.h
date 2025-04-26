/* ------------------------------------------------------------
name: "miniPAM"
Code generated with Faust 2.77.3 (https://faust.grame.fr)
Compilation options: -a ../../faust/tools/faustMinimalInlined.h -lang cpp -ct 1 -cn MiniPAM -es 1 -mcd 16 -mdd 1024 -mdy 33 -double -ftz 0
------------------------------------------------------------ */

#ifndef  __MiniPAM_H__
#define  __MiniPAM_H__

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
#define FAUSTCLASS MiniPAM
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

static double MiniPAM_faustpower2_f(double value) {
	return value * value;
}

class MiniPAM : public dsp {
	
 private:
	
	int fSampleRate;
	double fConst0;
	double fConst1;
	double fConst2;
	double fConst3;
	double fConst4;
	double fConst5;
	double fConst6;
	double fConst7;
	double fConst8;
	double fConst9;
	double fConst10;
	FAUSTFLOAT fHslider0;
	double fConst11;
	double fRec70[2];
	double fConst12;
	double fConst13;
	double fRec69[3];
	FAUSTFLOAT fHslider1;
	double fRec71[2];
	double fVec0[2];
	FAUSTFLOAT fHslider2;
	double fRec72[2];
	double fConst14;
	double fConst15;
	double fVec1[2];
	double fRec79[2];
	double fRec78[2];
	double fVec2[2];
	double fRec77[2];
	double fRec76[2];
	double fVec3[2];
	double fRec83[2];
	double fRec82[2];
	double fVec4[2];
	double fRec81[2];
	double fRec80[2];
	double fConst16;
	int iRec74[2];
	double fRec75[2];
	double fRec73[2];
	double fVec5[2];
	double fConst17;
	double fRec68[2];
	double fConst18;
	double fConst19;
	double fVec6[2];
	double fRec90[2];
	double fRec89[2];
	double fVec7[2];
	double fRec88[2];
	double fRec87[2];
	double fVec8[2];
	double fRec94[2];
	double fRec93[2];
	double fVec9[2];
	double fRec92[2];
	double fRec91[2];
	double fConst20;
	int iRec85[2];
	double fRec86[2];
	double fRec84[2];
	double fVec10[2];
	double fRec102[2];
	double fRec101[2];
	double fVec11[2];
	double fRec100[2];
	double fRec99[2];
	double fVec12[2];
	double fRec106[2];
	double fRec105[2];
	double fVec13[2];
	double fRec104[2];
	double fRec103[2];
	int iRec97[2];
	double fRec98[2];
	double fRec96[2];
	double fVec14[2];
	double fRec95[2];
	double fVec15[2];
	double fRec113[2];
	double fRec112[2];
	double fVec16[2];
	double fRec111[2];
	double fRec110[2];
	double fVec17[2];
	double fRec117[2];
	double fRec116[2];
	double fVec18[2];
	double fRec115[2];
	double fRec114[2];
	int iRec108[2];
	double fRec109[2];
	double fRec107[2];
	double fVec19[2];
	double fRec67[2];
	double fConst21;
	double fConst22;
	double fRec66[3];
	double fVec20[2];
	double fConst23;
	double fConst24;
	double fConst25;
	double fConst26;
	double fConst27;
	double fConst28;
	double fRec65[2];
	FAUSTFLOAT fHslider3;
	double fRec118[2];
	double fConst29;
	double fRec64[3];
	double fConst30;
	double fConst31;
	double fConst32;
	double fConst33;
	double fConst34;
	double fConst35;
	double fConst36;
	double fConst37;
	double fConst38;
	double fVec21[3];
	double fConst39;
	double fConst40;
	double fVec22[2];
	double fVec23[2];
	double fRec63[2];
	double fRec62[2];
	double fVec24[2];
	double fRec61[2];
	double fRec60[2];
	double fVec25[2];
	double fRec59[2];
	double fRec58[2];
	double fVec26[2];
	double fRec57[2];
	double fRec56[2];
	double fVec27[2];
	double fRec55[2];
	double fRec54[2];
	double fVec28[2];
	double fRec53[2];
	double fRec52[2];
	double fVec29[2];
	double fRec51[2];
	double fRec50[2];
	double fVec30[2];
	double fRec49[2];
	double fRec48[2];
	double fVec31[2];
	double fRec47[2];
	double fRec46[2];
	double fVec32[2];
	double fRec45[2];
	double fRec44[2];
	double fVec33[2];
	double fRec43[2];
	double fRec42[2];
	double fVec34[2];
	double fRec41[2];
	double fRec40[2];
	double fVec35[2];
	double fRec39[2];
	double fRec38[2];
	double fVec36[2];
	double fRec37[2];
	double fRec36[2];
	double fVec37[2];
	double fRec35[2];
	double fRec34[2];
	double fVec38[2];
	double fRec33[2];
	double fRec32[2];
	double fVec39[2];
	double fRec31[2];
	double fRec30[2];
	double fVec40[2];
	double fRec29[2];
	double fRec28[2];
	double fVec41[2];
	double fRec27[2];
	double fRec26[2];
	double fVec42[2];
	double fRec25[2];
	double fRec24[2];
	double fVec43[2];
	double fRec23[2];
	double fRec22[2];
	double fVec44[2];
	double fRec21[2];
	double fRec20[2];
	double fVec45[2];
	double fRec19[2];
	double fRec18[2];
	double fVec46[2];
	double fRec17[2];
	double fRec16[2];
	double fVec47[2];
	double fRec15[2];
	double fRec14[2];
	double fVec48[2];
	double fRec13[2];
	double fRec12[2];
	double fVec49[2];
	double fRec11[2];
	double fRec10[2];
	double fVec50[2];
	double fRec9[2];
	double fRec8[2];
	double fVec51[2];
	double fRec7[2];
	double fRec6[2];
	double fVec52[2];
	double fRec5[2];
	double fRec4[2];
	double fVec53[2];
	double fRec3[2];
	double fRec2[2];
	double fVec54[2];
	double fRec1[2];
	double fRec0[2];
	
 public:
	MiniPAM() {
	}
	
	void metadata(Meta* m) { 
		m->declare("aanl.lib/ADAA1:author", "Dario Sanfilippo");
		m->declare("aanl.lib/ADAA1:copyright", "Copyright (C) 2021 Dario Sanfilippo     <sanfilippo.dario@gmail.com>");
		m->declare("aanl.lib/ADAA1:license", "LGPL v3.0 license");
		m->declare("aanl.lib/hyperbolic:author", "Dario Sanfilippo");
		m->declare("aanl.lib/hyperbolic:copyright", "Copyright (C) 2021 Dario Sanfilippo      <sanfilippo.dario@gmail.com>");
		m->declare("aanl.lib/hyperbolic:license", "LGPL v3.0 license");
		m->declare("aanl.lib/name", "Faust Antialiased Nonlinearities");
		m->declare("aanl.lib/version", "1.4.0");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.21.0");
		m->declare("compile_options", "-a ../../faust/tools/faustMinimalInlined.h -lang cpp -ct 1 -cn MiniPAM -es 1 -mcd 16 -mdd 1024 -mdy 33 -double -ftz 0");
		m->declare("filename", "miniPAM.dsp");
		m->declare("filters.lib/dcblocker:author", "Julius O. Smith III");
		m->declare("filters.lib/dcblocker:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/dcblocker:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/peak_eq:author", "Julius O. Smith III");
		m->declare("filters.lib/peak_eq:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/peak_eq:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/peak_eq_cq:author", "Julius O. Smith III");
		m->declare("filters.lib/peak_eq_cq:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/peak_eq_cq:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/resonlp:author", "Julius O. Smith III");
		m->declare("filters.lib/resonlp:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/resonlp:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf22t:author", "Julius O. Smith III");
		m->declare("filters.lib/tf22t:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf22t:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.6.0");
		m->declare("filters.lib/zero:author", "Julius O. Smith III");
		m->declare("filters.lib/zero:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/zero:license", "MIT-style STK-4.3 license");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.1");
		m->declare("name", "miniPAM");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/onePoleSwitching:author", "Jonatan Liljedahl, revised by Dario Sanfilippo");
		m->declare("signals.lib/onePoleSwitching:licence", "STK-4.3");
		m->declare("signals.lib/version", "1.6.0");
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
		fConst0 = std::min<double>(1.92e+05, std::max<double>(1.0, double(fSampleRate)));
		fConst1 = 200133.46934023377 / fConst0;
		fConst2 = (fConst1 + 699.004365423729) / fConst0 + 1.0;
		fConst3 = 6.800000000000001e-07 * fConst2;
		fConst4 = std::tan(18849.55592153876 / fConst0);
		fConst5 = 1.0 / fConst4;
		fConst6 = (fConst5 + 1.4144271570014144) / fConst4 + 1.0;
		fConst7 = 1.0 / fConst6;
		fConst8 = std::tan(15707.963267948966 / fConst0);
		fConst9 = 2.0 * (1.0 - 1.0 / MiniPAM_faustpower2_f(fConst8));
		fConst10 = 44.1 / fConst0;
		fConst11 = 1.0 - fConst10;
		fConst12 = 1.0 / fConst8;
		fConst13 = 31415.926535897932 / (fConst0 * std::sin(31415.926535897932 / fConst0));
		fConst14 = std::exp(-(1e+02 / fConst0));
		fConst15 = 1.0 - fConst14;
		fConst16 = 0.01 * fConst0;
		fConst17 = 1.0 / fConst0;
		fConst18 = std::exp(-(1e+01 / fConst0));
		fConst19 = 1.0 - fConst18;
		fConst20 = 0.1 * fConst0;
		fConst21 = (fConst5 + -1.4144271570014144) / fConst4 + 1.0;
		fConst22 = 2.0 * (1.0 - 1.0 / MiniPAM_faustpower2_f(fConst4));
		fConst23 = MiniPAM_faustpower2_f(fConst6);
		fConst24 = 0.05555555555555555 / fConst23;
		fConst25 = 1.0 / fConst23;
		fConst26 = 0.5 / fConst6;
		fConst27 = 0.25 / fConst23;
		fConst28 = 2.25 / fConst23;
		fConst29 = -699.004365423729 - 400266.93868046755 / fConst0;
		fConst30 = 1470588.2352941176 / fConst2;
		fConst31 = MiniPAM_faustpower2_f(fConst2);
		fConst32 = 2.312e-13 * fConst31;
		fConst33 = 2.7975200000000005e-15 * fConst31;
		fConst34 = 5.035536000000001e-14 * fConst31;
		fConst35 = 3.4000000000000003e-07 * fConst2;
		fConst36 = 1.1220000000000001e-07 * fConst2;
		fConst37 = 1.2588840000000002e-14 * fConst31;
		fConst38 = 1.1329956000000002e-13 * fConst31;
		fConst39 = fConst29 / fConst0;
		fConst40 = 735784813750.8595 / MiniPAM_faustpower2_f(fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1.0);
		fHslider1 = FAUSTFLOAT(0.0);
		fHslider2 = FAUSTFLOAT(0.2);
		fHslider3 = FAUSTFLOAT(0.0);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec70[l0] = 0.0;
		}
		for (int l1 = 0; l1 < 3; l1 = l1 + 1) {
			fRec69[l1] = 0.0;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec71[l2] = 0.0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fVec0[l3] = 0.0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fRec72[l4] = 0.0;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fVec1[l5] = 0.0;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec79[l6] = 0.0;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			fRec78[l7] = 0.0;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fVec2[l8] = 0.0;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec77[l9] = 0.0;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec76[l10] = 0.0;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fVec3[l11] = 0.0;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec83[l12] = 0.0;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec82[l13] = 0.0;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fVec4[l14] = 0.0;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec81[l15] = 0.0;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec80[l16] = 0.0;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			iRec74[l17] = 0;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec75[l18] = 0.0;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			fRec73[l19] = 0.0;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fVec5[l20] = 0.0;
		}
		for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
			fRec68[l21] = 0.0;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fVec6[l22] = 0.0;
		}
		for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
			fRec90[l23] = 0.0;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec89[l24] = 0.0;
		}
		for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
			fVec7[l25] = 0.0;
		}
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			fRec88[l26] = 0.0;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			fRec87[l27] = 0.0;
		}
		for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
			fVec8[l28] = 0.0;
		}
		for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
			fRec94[l29] = 0.0;
		}
		for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
			fRec93[l30] = 0.0;
		}
		for (int l31 = 0; l31 < 2; l31 = l31 + 1) {
			fVec9[l31] = 0.0;
		}
		for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
			fRec92[l32] = 0.0;
		}
		for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
			fRec91[l33] = 0.0;
		}
		for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
			iRec85[l34] = 0;
		}
		for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
			fRec86[l35] = 0.0;
		}
		for (int l36 = 0; l36 < 2; l36 = l36 + 1) {
			fRec84[l36] = 0.0;
		}
		for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
			fVec10[l37] = 0.0;
		}
		for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
			fRec102[l38] = 0.0;
		}
		for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
			fRec101[l39] = 0.0;
		}
		for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
			fVec11[l40] = 0.0;
		}
		for (int l41 = 0; l41 < 2; l41 = l41 + 1) {
			fRec100[l41] = 0.0;
		}
		for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
			fRec99[l42] = 0.0;
		}
		for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
			fVec12[l43] = 0.0;
		}
		for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
			fRec106[l44] = 0.0;
		}
		for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
			fRec105[l45] = 0.0;
		}
		for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
			fVec13[l46] = 0.0;
		}
		for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
			fRec104[l47] = 0.0;
		}
		for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
			fRec103[l48] = 0.0;
		}
		for (int l49 = 0; l49 < 2; l49 = l49 + 1) {
			iRec97[l49] = 0;
		}
		for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
			fRec98[l50] = 0.0;
		}
		for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
			fRec96[l51] = 0.0;
		}
		for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
			fVec14[l52] = 0.0;
		}
		for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
			fRec95[l53] = 0.0;
		}
		for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
			fVec15[l54] = 0.0;
		}
		for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
			fRec113[l55] = 0.0;
		}
		for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
			fRec112[l56] = 0.0;
		}
		for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
			fVec16[l57] = 0.0;
		}
		for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
			fRec111[l58] = 0.0;
		}
		for (int l59 = 0; l59 < 2; l59 = l59 + 1) {
			fRec110[l59] = 0.0;
		}
		for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
			fVec17[l60] = 0.0;
		}
		for (int l61 = 0; l61 < 2; l61 = l61 + 1) {
			fRec117[l61] = 0.0;
		}
		for (int l62 = 0; l62 < 2; l62 = l62 + 1) {
			fRec116[l62] = 0.0;
		}
		for (int l63 = 0; l63 < 2; l63 = l63 + 1) {
			fVec18[l63] = 0.0;
		}
		for (int l64 = 0; l64 < 2; l64 = l64 + 1) {
			fRec115[l64] = 0.0;
		}
		for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
			fRec114[l65] = 0.0;
		}
		for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
			iRec108[l66] = 0;
		}
		for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
			fRec109[l67] = 0.0;
		}
		for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
			fRec107[l68] = 0.0;
		}
		for (int l69 = 0; l69 < 2; l69 = l69 + 1) {
			fVec19[l69] = 0.0;
		}
		for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
			fRec67[l70] = 0.0;
		}
		for (int l71 = 0; l71 < 3; l71 = l71 + 1) {
			fRec66[l71] = 0.0;
		}
		for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
			fVec20[l72] = 0.0;
		}
		for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
			fRec65[l73] = 0.0;
		}
		for (int l74 = 0; l74 < 2; l74 = l74 + 1) {
			fRec118[l74] = 0.0;
		}
		for (int l75 = 0; l75 < 3; l75 = l75 + 1) {
			fRec64[l75] = 0.0;
		}
		for (int l76 = 0; l76 < 3; l76 = l76 + 1) {
			fVec21[l76] = 0.0;
		}
		for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
			fVec22[l77] = 0.0;
		}
		for (int l78 = 0; l78 < 2; l78 = l78 + 1) {
			fVec23[l78] = 0.0;
		}
		for (int l79 = 0; l79 < 2; l79 = l79 + 1) {
			fRec63[l79] = 0.0;
		}
		for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
			fRec62[l80] = 0.0;
		}
		for (int l81 = 0; l81 < 2; l81 = l81 + 1) {
			fVec24[l81] = 0.0;
		}
		for (int l82 = 0; l82 < 2; l82 = l82 + 1) {
			fRec61[l82] = 0.0;
		}
		for (int l83 = 0; l83 < 2; l83 = l83 + 1) {
			fRec60[l83] = 0.0;
		}
		for (int l84 = 0; l84 < 2; l84 = l84 + 1) {
			fVec25[l84] = 0.0;
		}
		for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
			fRec59[l85] = 0.0;
		}
		for (int l86 = 0; l86 < 2; l86 = l86 + 1) {
			fRec58[l86] = 0.0;
		}
		for (int l87 = 0; l87 < 2; l87 = l87 + 1) {
			fVec26[l87] = 0.0;
		}
		for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
			fRec57[l88] = 0.0;
		}
		for (int l89 = 0; l89 < 2; l89 = l89 + 1) {
			fRec56[l89] = 0.0;
		}
		for (int l90 = 0; l90 < 2; l90 = l90 + 1) {
			fVec27[l90] = 0.0;
		}
		for (int l91 = 0; l91 < 2; l91 = l91 + 1) {
			fRec55[l91] = 0.0;
		}
		for (int l92 = 0; l92 < 2; l92 = l92 + 1) {
			fRec54[l92] = 0.0;
		}
		for (int l93 = 0; l93 < 2; l93 = l93 + 1) {
			fVec28[l93] = 0.0;
		}
		for (int l94 = 0; l94 < 2; l94 = l94 + 1) {
			fRec53[l94] = 0.0;
		}
		for (int l95 = 0; l95 < 2; l95 = l95 + 1) {
			fRec52[l95] = 0.0;
		}
		for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
			fVec29[l96] = 0.0;
		}
		for (int l97 = 0; l97 < 2; l97 = l97 + 1) {
			fRec51[l97] = 0.0;
		}
		for (int l98 = 0; l98 < 2; l98 = l98 + 1) {
			fRec50[l98] = 0.0;
		}
		for (int l99 = 0; l99 < 2; l99 = l99 + 1) {
			fVec30[l99] = 0.0;
		}
		for (int l100 = 0; l100 < 2; l100 = l100 + 1) {
			fRec49[l100] = 0.0;
		}
		for (int l101 = 0; l101 < 2; l101 = l101 + 1) {
			fRec48[l101] = 0.0;
		}
		for (int l102 = 0; l102 < 2; l102 = l102 + 1) {
			fVec31[l102] = 0.0;
		}
		for (int l103 = 0; l103 < 2; l103 = l103 + 1) {
			fRec47[l103] = 0.0;
		}
		for (int l104 = 0; l104 < 2; l104 = l104 + 1) {
			fRec46[l104] = 0.0;
		}
		for (int l105 = 0; l105 < 2; l105 = l105 + 1) {
			fVec32[l105] = 0.0;
		}
		for (int l106 = 0; l106 < 2; l106 = l106 + 1) {
			fRec45[l106] = 0.0;
		}
		for (int l107 = 0; l107 < 2; l107 = l107 + 1) {
			fRec44[l107] = 0.0;
		}
		for (int l108 = 0; l108 < 2; l108 = l108 + 1) {
			fVec33[l108] = 0.0;
		}
		for (int l109 = 0; l109 < 2; l109 = l109 + 1) {
			fRec43[l109] = 0.0;
		}
		for (int l110 = 0; l110 < 2; l110 = l110 + 1) {
			fRec42[l110] = 0.0;
		}
		for (int l111 = 0; l111 < 2; l111 = l111 + 1) {
			fVec34[l111] = 0.0;
		}
		for (int l112 = 0; l112 < 2; l112 = l112 + 1) {
			fRec41[l112] = 0.0;
		}
		for (int l113 = 0; l113 < 2; l113 = l113 + 1) {
			fRec40[l113] = 0.0;
		}
		for (int l114 = 0; l114 < 2; l114 = l114 + 1) {
			fVec35[l114] = 0.0;
		}
		for (int l115 = 0; l115 < 2; l115 = l115 + 1) {
			fRec39[l115] = 0.0;
		}
		for (int l116 = 0; l116 < 2; l116 = l116 + 1) {
			fRec38[l116] = 0.0;
		}
		for (int l117 = 0; l117 < 2; l117 = l117 + 1) {
			fVec36[l117] = 0.0;
		}
		for (int l118 = 0; l118 < 2; l118 = l118 + 1) {
			fRec37[l118] = 0.0;
		}
		for (int l119 = 0; l119 < 2; l119 = l119 + 1) {
			fRec36[l119] = 0.0;
		}
		for (int l120 = 0; l120 < 2; l120 = l120 + 1) {
			fVec37[l120] = 0.0;
		}
		for (int l121 = 0; l121 < 2; l121 = l121 + 1) {
			fRec35[l121] = 0.0;
		}
		for (int l122 = 0; l122 < 2; l122 = l122 + 1) {
			fRec34[l122] = 0.0;
		}
		for (int l123 = 0; l123 < 2; l123 = l123 + 1) {
			fVec38[l123] = 0.0;
		}
		for (int l124 = 0; l124 < 2; l124 = l124 + 1) {
			fRec33[l124] = 0.0;
		}
		for (int l125 = 0; l125 < 2; l125 = l125 + 1) {
			fRec32[l125] = 0.0;
		}
		for (int l126 = 0; l126 < 2; l126 = l126 + 1) {
			fVec39[l126] = 0.0;
		}
		for (int l127 = 0; l127 < 2; l127 = l127 + 1) {
			fRec31[l127] = 0.0;
		}
		for (int l128 = 0; l128 < 2; l128 = l128 + 1) {
			fRec30[l128] = 0.0;
		}
		for (int l129 = 0; l129 < 2; l129 = l129 + 1) {
			fVec40[l129] = 0.0;
		}
		for (int l130 = 0; l130 < 2; l130 = l130 + 1) {
			fRec29[l130] = 0.0;
		}
		for (int l131 = 0; l131 < 2; l131 = l131 + 1) {
			fRec28[l131] = 0.0;
		}
		for (int l132 = 0; l132 < 2; l132 = l132 + 1) {
			fVec41[l132] = 0.0;
		}
		for (int l133 = 0; l133 < 2; l133 = l133 + 1) {
			fRec27[l133] = 0.0;
		}
		for (int l134 = 0; l134 < 2; l134 = l134 + 1) {
			fRec26[l134] = 0.0;
		}
		for (int l135 = 0; l135 < 2; l135 = l135 + 1) {
			fVec42[l135] = 0.0;
		}
		for (int l136 = 0; l136 < 2; l136 = l136 + 1) {
			fRec25[l136] = 0.0;
		}
		for (int l137 = 0; l137 < 2; l137 = l137 + 1) {
			fRec24[l137] = 0.0;
		}
		for (int l138 = 0; l138 < 2; l138 = l138 + 1) {
			fVec43[l138] = 0.0;
		}
		for (int l139 = 0; l139 < 2; l139 = l139 + 1) {
			fRec23[l139] = 0.0;
		}
		for (int l140 = 0; l140 < 2; l140 = l140 + 1) {
			fRec22[l140] = 0.0;
		}
		for (int l141 = 0; l141 < 2; l141 = l141 + 1) {
			fVec44[l141] = 0.0;
		}
		for (int l142 = 0; l142 < 2; l142 = l142 + 1) {
			fRec21[l142] = 0.0;
		}
		for (int l143 = 0; l143 < 2; l143 = l143 + 1) {
			fRec20[l143] = 0.0;
		}
		for (int l144 = 0; l144 < 2; l144 = l144 + 1) {
			fVec45[l144] = 0.0;
		}
		for (int l145 = 0; l145 < 2; l145 = l145 + 1) {
			fRec19[l145] = 0.0;
		}
		for (int l146 = 0; l146 < 2; l146 = l146 + 1) {
			fRec18[l146] = 0.0;
		}
		for (int l147 = 0; l147 < 2; l147 = l147 + 1) {
			fVec46[l147] = 0.0;
		}
		for (int l148 = 0; l148 < 2; l148 = l148 + 1) {
			fRec17[l148] = 0.0;
		}
		for (int l149 = 0; l149 < 2; l149 = l149 + 1) {
			fRec16[l149] = 0.0;
		}
		for (int l150 = 0; l150 < 2; l150 = l150 + 1) {
			fVec47[l150] = 0.0;
		}
		for (int l151 = 0; l151 < 2; l151 = l151 + 1) {
			fRec15[l151] = 0.0;
		}
		for (int l152 = 0; l152 < 2; l152 = l152 + 1) {
			fRec14[l152] = 0.0;
		}
		for (int l153 = 0; l153 < 2; l153 = l153 + 1) {
			fVec48[l153] = 0.0;
		}
		for (int l154 = 0; l154 < 2; l154 = l154 + 1) {
			fRec13[l154] = 0.0;
		}
		for (int l155 = 0; l155 < 2; l155 = l155 + 1) {
			fRec12[l155] = 0.0;
		}
		for (int l156 = 0; l156 < 2; l156 = l156 + 1) {
			fVec49[l156] = 0.0;
		}
		for (int l157 = 0; l157 < 2; l157 = l157 + 1) {
			fRec11[l157] = 0.0;
		}
		for (int l158 = 0; l158 < 2; l158 = l158 + 1) {
			fRec10[l158] = 0.0;
		}
		for (int l159 = 0; l159 < 2; l159 = l159 + 1) {
			fVec50[l159] = 0.0;
		}
		for (int l160 = 0; l160 < 2; l160 = l160 + 1) {
			fRec9[l160] = 0.0;
		}
		for (int l161 = 0; l161 < 2; l161 = l161 + 1) {
			fRec8[l161] = 0.0;
		}
		for (int l162 = 0; l162 < 2; l162 = l162 + 1) {
			fVec51[l162] = 0.0;
		}
		for (int l163 = 0; l163 < 2; l163 = l163 + 1) {
			fRec7[l163] = 0.0;
		}
		for (int l164 = 0; l164 < 2; l164 = l164 + 1) {
			fRec6[l164] = 0.0;
		}
		for (int l165 = 0; l165 < 2; l165 = l165 + 1) {
			fVec52[l165] = 0.0;
		}
		for (int l166 = 0; l166 < 2; l166 = l166 + 1) {
			fRec5[l166] = 0.0;
		}
		for (int l167 = 0; l167 < 2; l167 = l167 + 1) {
			fRec4[l167] = 0.0;
		}
		for (int l168 = 0; l168 < 2; l168 = l168 + 1) {
			fVec53[l168] = 0.0;
		}
		for (int l169 = 0; l169 < 2; l169 = l169 + 1) {
			fRec3[l169] = 0.0;
		}
		for (int l170 = 0; l170 < 2; l170 = l170 + 1) {
			fRec2[l170] = 0.0;
		}
		for (int l171 = 0; l171 < 2; l171 = l171 + 1) {
			fVec54[l171] = 0.0;
		}
		for (int l172 = 0; l172 < 2; l172 = l172 + 1) {
			fRec1[l172] = 0.0;
		}
		for (int l173 = 0; l173 < 2; l173 = l173 + 1) {
			fRec0[l173] = 0.0;
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
	
	virtual MiniPAM* clone() {
		return new MiniPAM();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("miniPAM");
		ui_interface->addHorizontalSlider("bias", &fHslider2, FAUSTFLOAT(0.2), FAUSTFLOAT(0.0), FAUSTFLOAT(1.0), FAUSTFLOAT(0.01));
		ui_interface->addHorizontalSlider("master", &fHslider3, FAUSTFLOAT(0.0), FAUSTFLOAT(-2e+01), FAUSTFLOAT(2e+01), FAUSTFLOAT(0.01));
		ui_interface->addHorizontalSlider("preamp gain", &fHslider1, FAUSTFLOAT(0.0), FAUSTFLOAT(0.0), FAUSTFLOAT(4e+01), FAUSTFLOAT(0.01));
		ui_interface->addHorizontalSlider("presence", &fHslider0, FAUSTFLOAT(1.0), FAUSTFLOAT(0.5), FAUSTFLOAT(2.0), FAUSTFLOAT(0.01));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		double fSlow0 = fConst10 * double(fHslider0);
		double fSlow1 = fConst10 * std::pow(1e+01, 0.05 * double(fHslider1));
		double fSlow2 = fConst10 * double(fHslider2);
		double fSlow3 = fConst10 * std::pow(1e+01, 0.05 * double(fHslider3));
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec70[0] = fSlow0 + fConst11 * fRec70[1];
			int iTemp0 = fRec65[1] > 0.0;
			double fTemp1 = fConst13 * std::pow(1e+01, 0.05 * std::fabs(fRec65[1]));
			double fTemp2 = ((iTemp0) ? fConst13 : fTemp1);
			double fTemp3 = fConst9 * fRec69[1];
			double fTemp4 = fConst12 * (fConst12 + fTemp2) + 1.0;
			fRec69[0] = -(0.1 * fRec70[0] + (fRec69[2] * (fConst12 * (fConst12 - fTemp2) + 1.0) + fTemp3) / fTemp4);
			double fTemp5 = ((iTemp0) ? fTemp1 : fConst13);
			fRec71[0] = fSlow1 + fConst11 * fRec71[1];
			double fTemp6 = (fTemp3 + fRec69[0] * (fConst12 * (fConst12 + fTemp5) + 1.0) + fRec69[2] * (fConst12 * (fConst12 - fTemp5) + 1.0)) / fTemp4 + 0.03794733192202055 * double(input0[i0]) * fRec71[0];
			fVec0[0] = fTemp6;
			fRec72[0] = fSlow2 + fConst11 * fRec72[1];
			fVec1[0] = 0.260502 * fRec78[1] + fVec0[1];
			double fTemp7 = 0.260502 * fTemp6;
			fRec79[0] = fVec1[1] - (0.02569 * (fRec79[1] - fVec0[1]) + fTemp7);
			fRec78[0] = fRec79[0];
			fVec2[0] = fRec78[1] - 0.870686 * fRec76[1];
			fRec77[0] = 1.8685 * (fRec77[1] - fRec78[1]) + fVec2[1] + 0.870686 * fRec78[0];
			fRec76[0] = fRec77[0];
			fVec3[0] = fVec0[1] - 0.94657 * fRec82[1];
			double fTemp8 = 0.94657 * fTemp6;
			fRec83[0] = fVec3[1] + fTemp8 - 1.94632 * (fVec0[1] - fRec83[1]);
			fRec82[0] = fRec83[0];
			fVec4[0] = fRec82[1] - 0.06338 * fRec80[1];
			fRec81[0] = 0.83774 * (fRec81[1] - fRec82[1]) + fVec4[1] + 0.06338 * fRec82[0];
			fRec80[0] = fRec81[0];
			double fTemp9 = std::sqrt(MiniPAM_faustpower2_f(fRec76[0]) + MiniPAM_faustpower2_f(fRec80[0]));
			int iTemp10 = (fTemp9 >= fRec75[1]) | (double(iRec74[1]) >= fConst16);
			iRec74[0] = ((iTemp10) ? 0 : iRec74[1] + 1);
			fRec75[0] = ((iTemp10) ? fTemp9 : fRec75[1]);
			fRec73[0] = fConst15 * fRec75[0] + fConst14 * fRec73[1];
			double fTemp11 = fTemp6 + fRec72[0] * (1.0 - 0.2 * fRec73[0]);
			fVec5[0] = fTemp11;
			double fTemp12 = fTemp11 - fVec5[1];
			double fTemp13 = std::fabs(fTemp11);
			double fTemp14 = std::fabs(fVec5[1]);
			double fTemp15 = fTemp11 + fVec5[1];
			fRec68[0] = fVec5[1] - ((std::fabs(fTemp12) <= fConst17) ? 0.5 * (fTemp15 / (std::fabs(0.5 * fTemp15) + 1.0)) : (fTemp13 + std::log(fTemp14 + 1.0) - (std::log(fTemp13 + 1.0) + fTemp14)) / fTemp12) * (fRec68[1] - fTemp11);
			fVec6[0] = fRec68[1] + 0.260502 * fRec89[1];
			fRec90[0] = fVec6[1] + 0.02569 * (fRec68[1] - fRec90[1]) - 0.260502 * fRec68[0];
			fRec89[0] = fRec90[0];
			fVec7[0] = fRec89[1] - 0.870686 * fRec87[1];
			fRec88[0] = 1.8685 * (fRec88[1] - fRec89[1]) + fVec7[1] + 0.870686 * fRec89[0];
			fRec87[0] = fRec88[0];
			fVec8[0] = fRec68[1] - 0.94657 * fRec93[1];
			fRec94[0] = fVec8[1] + 0.94657 * fRec68[0] - 1.94632 * (fRec68[1] - fRec94[1]);
			fRec93[0] = fRec94[0];
			fVec9[0] = fRec93[1] - 0.06338 * fRec91[1];
			fRec92[0] = 0.83774 * (fRec92[1] - fRec93[1]) + fVec9[1] + 0.06338 * fRec93[0];
			fRec91[0] = fRec92[0];
			double fTemp16 = std::sqrt(MiniPAM_faustpower2_f(fRec87[0]) + MiniPAM_faustpower2_f(fRec91[0]));
			int iTemp17 = (fTemp16 >= fRec86[1]) | (double(iRec85[1]) >= fConst20);
			iRec85[0] = ((iTemp17) ? 0 : iRec85[1] + 1);
			fRec86[0] = ((iTemp17) ? fTemp16 : fRec86[1]);
			fRec84[0] = fConst19 * fRec86[0] + fConst18 * fRec84[1];
			double fTemp18 = fRec68[0] * (1.0 - 0.2 * fRec84[0]);
			fVec10[0] = 0.260502 * fRec101[1] - fVec0[1];
			fRec102[0] = fTemp7 + fVec10[1] - 0.02569 * (fVec0[1] + fRec102[1]);
			fRec101[0] = fRec102[0];
			fVec11[0] = fRec101[1] - 0.870686 * fRec99[1];
			fRec100[0] = 1.8685 * (fRec100[1] - fRec101[1]) + fVec11[1] + 0.870686 * fRec101[0];
			fRec99[0] = fRec100[0];
			fVec12[0] = fVec0[1] + 0.94657 * fRec105[1];
			fRec106[0] = 1.94632 * (fVec0[1] + fRec106[1]) - (fTemp8 + fVec12[1]);
			fRec105[0] = fRec106[0];
			fVec13[0] = fRec105[1] - 0.06338 * fRec103[1];
			fRec104[0] = 0.83774 * (fRec104[1] - fRec105[1]) + fVec13[1] + 0.06338 * fRec105[0];
			fRec103[0] = fRec104[0];
			double fTemp19 = std::sqrt(MiniPAM_faustpower2_f(fRec99[0]) + MiniPAM_faustpower2_f(fRec103[0]));
			int iTemp20 = (fTemp19 >= fRec98[1]) | (double(iRec97[1]) >= fConst16);
			iRec97[0] = ((iTemp20) ? 0 : iRec97[1] + 1);
			fRec98[0] = ((iTemp20) ? fTemp19 : fRec98[1]);
			fRec96[0] = fConst15 * fRec98[0] + fConst14 * fRec96[1];
			double fTemp21 = fRec72[0] * (1.0 - 0.2 * fRec96[0]);
			double fTemp22 = fTemp21 - fTemp6;
			fVec14[0] = fTemp22;
			double fTemp23 = fTemp21 - (fTemp6 + fVec14[1]);
			double fTemp24 = std::fabs(fTemp22);
			double fTemp25 = std::fabs(fVec14[1]);
			double fTemp26 = fTemp21 + fVec14[1] - fTemp6;
			fRec95[0] = fVec14[1] - ((std::fabs(fTemp23) <= fConst17) ? 0.5 * (fTemp26 / (std::fabs(0.5 * fTemp26) + 1.0)) : (fTemp24 + std::log(fTemp25 + 1.0) - (std::log(fTemp24 + 1.0) + fTemp25)) / fTemp23) * (fRec95[1] - fTemp22);
			fVec15[0] = fRec95[1] + 0.260502 * fRec112[1];
			fRec113[0] = fVec15[1] + 0.02569 * (fRec95[1] - fRec113[1]) - 0.260502 * fRec95[0];
			fRec112[0] = fRec113[0];
			fVec16[0] = fRec112[1] - 0.870686 * fRec110[1];
			fRec111[0] = 1.8685 * (fRec111[1] - fRec112[1]) + fVec16[1] + 0.870686 * fRec112[0];
			fRec110[0] = fRec111[0];
			fVec17[0] = fRec95[1] - 0.94657 * fRec116[1];
			fRec117[0] = fVec17[1] + 0.94657 * fRec95[0] - 1.94632 * (fRec95[1] - fRec117[1]);
			fRec116[0] = fRec117[0];
			fVec18[0] = fRec116[1] - 0.06338 * fRec114[1];
			fRec115[0] = 0.83774 * (fRec115[1] - fRec116[1]) + fVec18[1] + 0.06338 * fRec116[0];
			fRec114[0] = fRec115[0];
			double fTemp27 = std::sqrt(MiniPAM_faustpower2_f(fRec110[0]) + MiniPAM_faustpower2_f(fRec114[0]));
			int iTemp28 = (fTemp27 >= fRec109[1]) | (double(iRec108[1]) >= fConst20);
			iRec108[0] = ((iTemp28) ? 0 : iRec108[1] + 1);
			fRec109[0] = ((iTemp28) ? fTemp27 : fRec109[1]);
			fRec107[0] = fConst19 * fRec109[0] + fConst18 * fRec107[1];
			double fTemp29 = fRec95[0] * (1.0 - 0.2 * fRec107[0]);
			fVec19[0] = fTemp18 - fTemp29;
			fRec67[0] = 0.995 * fRec67[1] + fTemp18 - (fTemp29 + fVec19[1]);
			fRec66[0] = fRec67[0] - fConst7 * (fConst21 * fRec66[2] + fConst22 * fRec66[1]);
			double fTemp30 = fRec66[2] + fRec66[0] + 2.0 * fRec66[1];
			fVec20[0] = fTemp30;
			double fTemp31 = fTemp30 - fVec20[1];
			double fTemp32 = MiniPAM_faustpower2_f(fTemp30);
			double fTemp33 = MiniPAM_faustpower2_f(fVec20[1]);
			double fTemp34 = fTemp30 + fVec20[1];
			double fTemp35 = MiniPAM_faustpower2_f(fTemp34);
			fRec65[0] = ((std::fabs(fConst7 * fTemp31) <= fConst17) ? fConst26 * (fTemp34 * (fConst27 * fTemp35 + 27.0) / (fConst28 * fTemp35 + 27.0)) : fConst6 * ((fConst24 * (fTemp32 - fTemp33) + 1.3333333333333333 * (std::log(fConst25 * fTemp32 + 3.0) - std::log(fConst25 * fTemp33 + 3.0))) / fTemp31));
			fRec118[0] = fSlow3 + fConst11 * fRec118[1];
			fRec64[0] = 0.7071067811865476 * fRec65[0] * fRec118[0] - fConst17 * (fConst29 * fRec64[1] + fConst1 * fRec64[2]);
			double fTemp36 = fRec64[0] - fRec64[1];
			double fTemp37 = MiniPAM_faustpower2_f(fRec64[0]);
			double fTemp38 = MiniPAM_faustpower2_f(fRec64[1]);
			double fTemp39 = fRec64[0] + fRec64[1];
			double fTemp40 = fConst35 * fTemp39;
			double fTemp41 = MiniPAM_faustpower2_f(fTemp39);
			double fTemp42 = ((std::fabs(fConst3 * fTemp36) <= fConst17) ? ((fTemp40 < 0.0) ? fConst36 * (fTemp39 * (fConst37 * fTemp41 + 27.0) / (fConst38 * fTemp41 + 27.0)) : fTemp40) : fConst30 * (((((fConst3 * fRec64[0]) < 0.0) ? fConst33 * fTemp37 + 1.3333333333333333 * std::log(fConst34 * fTemp37 + 3.0) : fConst32 * fTemp37) - (((fConst3 * fRec64[1]) < 0.0) ? fConst33 * fTemp38 + 1.3333333333333333 * std::log(fConst34 * fTemp38 + 3.0) : fConst32 * fTemp38)) / fTemp36));
			fVec21[0] = fTemp42;
			double fTemp43 = 3676470.588235294 * (fConst2 * fTemp42 + fConst39 * fVec21[1]) + fConst40 * fVec21[2];
			fVec22[0] = fTemp43;
			fVec23[0] = 0.2806469791704074 * fVec22[1] - 0.5845118592204748 * fRec62[1];
			fRec63[0] = fVec23[1] + 0.7401425791353882 * fVec22[1] + 0.5 * fTemp43 - 1.329712910006021 * fRec63[1];
			fRec62[0] = fRec63[0];
			fVec24[0] = 0.5109642076670752 * fRec62[1] - 0.8549735054220894 * fRec60[1];
			fRec61[0] = fVec24[1] + 1.087818286794911 * fRec62[1] + fRec62[0] + 0.06313815899766854 * fRec61[1];
			fRec60[0] = fRec61[0];
			fVec25[0] = 0.8557230539187307 * fRec60[1] - 0.8642103314292223 * fRec58[1];
			fRec59[0] = fVec25[1] + fRec60[0] + 1.190867161726978 * fRec59[1] - 0.2309310102976318 * fRec60[1];
			fRec58[0] = fRec59[0];
			fVec26[0] = 0.8344671400318606 * fRec58[1] - 0.8773776425918675 * fRec56[1];
			fRec57[0] = fVec26[1] + fRec58[0] + 0.565691796906173 * fRec58[1] - 0.7243277363905875 * fRec57[1];
			fRec56[0] = fRec57[0];
			fVec27[0] = 0.913123538574321 * fRec56[1] - 0.9021891001928158 * fRec54[1];
			fRec55[0] = fVec27[1] + fRec56[0] + 1.805152469827446 * fRec55[1] - 1.233624797754314 * fRec56[1];
			fRec54[0] = fRec55[0];
			fVec28[0] = 0.8870067245856905 * fRec54[1] - 0.9048660134290293 * fRec52[1];
			fRec53[0] = fVec28[1] + fRec54[0] + 1.629354368480196 * fRec53[1] - 1.754960607390966 * fRec54[1];
			fRec52[0] = fRec53[0];
			fVec29[0] = 0.9718420269745968 * fRec52[1] - 0.907647824739398 * fRec50[1];
			fRec51[0] = fVec29[1] + fRec52[0] + 0.5697418581588183 * fRec51[1] - 0.6019425747501961 * fRec52[1];
			fRec50[0] = fRec51[0];
			fVec30[0] = 0.937664182248145 * fRec50[1] - 0.9247986878965434 * fRec48[1];
			fRec49[0] = fVec30[1] + fRec50[0] + 1.520752451778903 * fRec49[1] - 1.566962438216134 * fRec50[1];
			fRec48[0] = fRec49[0];
			fVec31[0] = 0.9561192703046776 * fRec48[1] - 0.9443777901504538 * fRec46[1];
			fRec47[0] = fVec31[1] + fRec48[0] + 1.705858997437662 * fRec47[1] - 1.713553328167775 * fRec48[1];
			fRec46[0] = fRec47[0];
			fVec32[0] = 0.9889504252634936 * fRec46[1] - 0.9508136470443427 * fRec44[1];
			fRec45[0] = fVec32[1] + fRec46[0] + 1.4687770867956 * fRec45[1] - 1.494157095280938 * fRec46[1];
			fRec44[0] = fRec45[0];
			fVec33[0] = 0.9815341454298386 * fRec44[1] - 0.9558053486317664 * fRec42[1];
			fRec43[0] = fVec33[1] + fRec44[0] + 0.9953224449142073 * fRec43[1] - 0.9885667518321769 * fRec44[1];
			fRec42[0] = fRec43[0];
			fVec34[0] = 0.9772722637558471 * fRec42[1] - 0.9570006694359363 * fRec40[1];
			fRec41[0] = fVec34[1] + fRec42[0] + 1.646917434186097 * fRec41[1] - 1.671828560589219 * fRec42[1];
			fRec40[0] = fRec41[0];
			fVec35[0] = 0.9405551174112275 * fRec40[1] - 0.9715751460580375 * fRec38[1];
			fRec39[0] = fVec35[1] + fRec40[0] + 1.960202685508743 * fRec39[1] - 1.925830295520374 * fRec40[1];
			fRec38[0] = fRec39[0];
			fVec36[0] = 0.9366707120277246 * fRec38[1] - 0.9747232344107498 * fRec36[1];
			fRec37[0] = fVec36[1] + fRec38[0] + 1.855537873287801 * fRec37[1] - 1.838923017902955 * fRec38[1];
			fRec36[0] = fRec37[0];
			fVec37[0] = 0.9715721407101856 * fRec36[1] - 0.9772682085594058 * fRec34[1];
			fRec35[0] = fVec37[1] + fRec36[0] + 1.879962914019088 * fRec35[1] - 1.863852125298558 * fRec36[1];
			fRec34[0] = fRec35[0];
			fVec38[0] = 0.9208895578235797 * fRec34[1] - 0.9779111848799823 * fRec32[1];
			fRec33[0] = fVec38[1] + fRec34[0] + 1.977493135962735 * fRec33[1] - 1.920692064616606 * fRec34[1];
			fRec32[0] = fRec33[0];
			fVec39[0] = 0.9794557207120642 * fRec32[1] - 0.9795223224811935 * fRec30[1];
			fRec31[0] = fVec39[1] + fRec32[0] + 1.930741635326338 * fRec31[1] - 1.930336495963867 * fRec32[1];
			fRec30[0] = fRec31[0];
			fVec40[0] = 0.9863130602785309 * fRec30[1] - 0.97984095997443 * fRec28[1];
			fRec29[0] = fVec40[1] + fRec30[0] + 1.79838514163708 * fRec29[1] - 1.805112558301939 * fRec30[1];
			fRec28[0] = fRec29[0];
			fVec41[0] = 0.9596436221385273 * fRec28[1] - 0.9819562537337041 * fRec26[1];
			fRec27[0] = fVec41[1] + fRec28[0] + 1.836494246332822 * fRec27[1] - 1.822430592468539 * fRec28[1];
			fRec26[0] = fRec27[0];
			fVec42[0] = 0.9897667806355799 * fRec26[1] - 0.9830248212232814 * fRec24[1];
			fRec25[0] = fVec42[1] + fRec26[0] + 1.956727295705247 * fRec25[1] - 1.965229527534193 * fRec26[1];
			fRec24[0] = fRec25[0];
			fVec43[0] = 0.9846318647425024 * fRec24[1] - 0.9840886363329268 * fRec22[1];
			fRec23[0] = fVec43[1] + fRec24[0] + 1.952594409962654 * fRec23[1] - 1.952363266320942 * fRec24[1];
			fRec22[0] = fRec23[0];
			fVec44[0] = 0.9784446750553879 * fRec22[1] - 0.9871001087877421 * fRec20[1];
			fRec21[0] = fVec44[1] + fRec22[0] + 1.928851355524846 * fRec21[1] - 1.919793514901131 * fRec22[1];
			fRec20[0] = fRec21[0];
			fVec45[0] = 0.9880194267474456 * fRec20[1] - 0.9879758017308589 * fRec18[1];
			fRec19[0] = fVec45[1] + fRec20[0] + 1.948893426625345 * fRec19[1] - 1.948532931326867 * fRec20[1];
			fRec18[0] = fRec19[0];
			fVec46[0] = 0.9870343694812298 * fRec18[1] - 0.9891807385271297 * fRec16[1];
			fRec17[0] = fVec46[1] + fRec18[0] + 1.986499226863611 * fRec17[1] - 1.984241884763906 * fRec18[1];
			fRec16[0] = fRec17[0];
			fVec47[0] = 0.9942731455136984 * fRec16[1] - 0.9897591602296989 * fRec14[1];
			fRec15[0] = fVec47[1] + fRec16[0] + 1.988617901798895 * fRec15[1] - 1.993186663549856 * fRec16[1];
			fRec14[0] = fRec15[0];
			fVec48[0] = 0.9770791015145096 * fRec14[1] - 0.9903015583822948 * fRec12[1];
			fRec13[0] = fVec48[1] + fRec14[0] + 1.96939254727357 * fRec13[1] - 1.956700830694171 * fRec14[1];
			fRec12[0] = fRec13[0];
			fVec49[0] = 0.9908668699846226 * fRec12[1] - 0.9909371967985479 * fRec10[1];
			fRec11[0] = fVec49[1] + fRec12[0] + 1.910966241885817 * fRec11[1] - 1.911258728982635 * fRec12[1];
			fRec10[0] = fRec11[0];
			fVec50[0] = 0.9934464548045667 * fRec10[1] - 0.9917675190880688 * fRec8[1];
			fRec9[0] = fVec50[1] + fRec10[0] + 1.984644966505127 * fRec9[1] - 1.986182625718655 * fRec10[1];
			fRec8[0] = fRec9[0];
			fVec51[0] = 0.9916647585802274 * fRec8[1] - 0.99300400924389 * fRec6[1];
			fRec7[0] = fVec51[1] + fRec8[0] + 1.989247214871485 * fRec7[1] - 1.987902833642958 * fRec8[1];
			fRec6[0] = fRec7[0];
			fVec52[0] = 0.9985208686478012 * fRec6[1] - 0.9930742526667098 * fRec4[1];
			fRec5[0] = fVec52[1] + fRec6[0] + 1.992973102156906 * fRec5[1] - 1.998487534780157 * fRec6[1];
			fRec4[0] = fRec5[0];
			fVec53[0] = 0.9950226420024363 * fRec4[1] - 0.9946386100309316 * fRec2[1];
			fRec3[0] = fVec53[1] + fRec4[0] + 1.978126926091301 * fRec3[1] - 1.978260033774013 * fRec4[1];
			fRec2[0] = fRec3[0];
			fVec54[0] = 0.997358687719507 * fRec2[1] - 0.997020532636133 * fRec0[1];
			fRec1[0] = fVec54[1] + fRec2[0] + 1.987806823434516 * fRec1[1] - 1.988126101483113 * fRec2[1];
			fRec0[0] = fRec1[0];
			output0[i0] = FAUSTFLOAT(fRec0[0]);
			fRec70[1] = fRec70[0];
			fRec69[2] = fRec69[1];
			fRec69[1] = fRec69[0];
			fRec71[1] = fRec71[0];
			fVec0[1] = fVec0[0];
			fRec72[1] = fRec72[0];
			fVec1[1] = fVec1[0];
			fRec79[1] = fRec79[0];
			fRec78[1] = fRec78[0];
			fVec2[1] = fVec2[0];
			fRec77[1] = fRec77[0];
			fRec76[1] = fRec76[0];
			fVec3[1] = fVec3[0];
			fRec83[1] = fRec83[0];
			fRec82[1] = fRec82[0];
			fVec4[1] = fVec4[0];
			fRec81[1] = fRec81[0];
			fRec80[1] = fRec80[0];
			iRec74[1] = iRec74[0];
			fRec75[1] = fRec75[0];
			fRec73[1] = fRec73[0];
			fVec5[1] = fVec5[0];
			fRec68[1] = fRec68[0];
			fVec6[1] = fVec6[0];
			fRec90[1] = fRec90[0];
			fRec89[1] = fRec89[0];
			fVec7[1] = fVec7[0];
			fRec88[1] = fRec88[0];
			fRec87[1] = fRec87[0];
			fVec8[1] = fVec8[0];
			fRec94[1] = fRec94[0];
			fRec93[1] = fRec93[0];
			fVec9[1] = fVec9[0];
			fRec92[1] = fRec92[0];
			fRec91[1] = fRec91[0];
			iRec85[1] = iRec85[0];
			fRec86[1] = fRec86[0];
			fRec84[1] = fRec84[0];
			fVec10[1] = fVec10[0];
			fRec102[1] = fRec102[0];
			fRec101[1] = fRec101[0];
			fVec11[1] = fVec11[0];
			fRec100[1] = fRec100[0];
			fRec99[1] = fRec99[0];
			fVec12[1] = fVec12[0];
			fRec106[1] = fRec106[0];
			fRec105[1] = fRec105[0];
			fVec13[1] = fVec13[0];
			fRec104[1] = fRec104[0];
			fRec103[1] = fRec103[0];
			iRec97[1] = iRec97[0];
			fRec98[1] = fRec98[0];
			fRec96[1] = fRec96[0];
			fVec14[1] = fVec14[0];
			fRec95[1] = fRec95[0];
			fVec15[1] = fVec15[0];
			fRec113[1] = fRec113[0];
			fRec112[1] = fRec112[0];
			fVec16[1] = fVec16[0];
			fRec111[1] = fRec111[0];
			fRec110[1] = fRec110[0];
			fVec17[1] = fVec17[0];
			fRec117[1] = fRec117[0];
			fRec116[1] = fRec116[0];
			fVec18[1] = fVec18[0];
			fRec115[1] = fRec115[0];
			fRec114[1] = fRec114[0];
			iRec108[1] = iRec108[0];
			fRec109[1] = fRec109[0];
			fRec107[1] = fRec107[0];
			fVec19[1] = fVec19[0];
			fRec67[1] = fRec67[0];
			fRec66[2] = fRec66[1];
			fRec66[1] = fRec66[0];
			fVec20[1] = fVec20[0];
			fRec65[1] = fRec65[0];
			fRec118[1] = fRec118[0];
			fRec64[2] = fRec64[1];
			fRec64[1] = fRec64[0];
			fVec21[2] = fVec21[1];
			fVec21[1] = fVec21[0];
			fVec22[1] = fVec22[0];
			fVec23[1] = fVec23[0];
			fRec63[1] = fRec63[0];
			fRec62[1] = fRec62[0];
			fVec24[1] = fVec24[0];
			fRec61[1] = fRec61[0];
			fRec60[1] = fRec60[0];
			fVec25[1] = fVec25[0];
			fRec59[1] = fRec59[0];
			fRec58[1] = fRec58[0];
			fVec26[1] = fVec26[0];
			fRec57[1] = fRec57[0];
			fRec56[1] = fRec56[0];
			fVec27[1] = fVec27[0];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
			fVec28[1] = fVec28[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fVec29[1] = fVec29[0];
			fRec51[1] = fRec51[0];
			fRec50[1] = fRec50[0];
			fVec30[1] = fVec30[0];
			fRec49[1] = fRec49[0];
			fRec48[1] = fRec48[0];
			fVec31[1] = fVec31[0];
			fRec47[1] = fRec47[0];
			fRec46[1] = fRec46[0];
			fVec32[1] = fVec32[0];
			fRec45[1] = fRec45[0];
			fRec44[1] = fRec44[0];
			fVec33[1] = fVec33[0];
			fRec43[1] = fRec43[0];
			fRec42[1] = fRec42[0];
			fVec34[1] = fVec34[0];
			fRec41[1] = fRec41[0];
			fRec40[1] = fRec40[0];
			fVec35[1] = fVec35[0];
			fRec39[1] = fRec39[0];
			fRec38[1] = fRec38[0];
			fVec36[1] = fVec36[0];
			fRec37[1] = fRec37[0];
			fRec36[1] = fRec36[0];
			fVec37[1] = fVec37[0];
			fRec35[1] = fRec35[0];
			fRec34[1] = fRec34[0];
			fVec38[1] = fVec38[0];
			fRec33[1] = fRec33[0];
			fRec32[1] = fRec32[0];
			fVec39[1] = fVec39[0];
			fRec31[1] = fRec31[0];
			fRec30[1] = fRec30[0];
			fVec40[1] = fVec40[0];
			fRec29[1] = fRec29[0];
			fRec28[1] = fRec28[0];
			fVec41[1] = fVec41[0];
			fRec27[1] = fRec27[0];
			fRec26[1] = fRec26[0];
			fVec42[1] = fVec42[0];
			fRec25[1] = fRec25[0];
			fRec24[1] = fRec24[0];
			fVec43[1] = fVec43[0];
			fRec23[1] = fRec23[0];
			fRec22[1] = fRec22[0];
			fVec44[1] = fVec44[0];
			fRec21[1] = fRec21[0];
			fRec20[1] = fRec20[0];
			fVec45[1] = fVec45[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fVec46[1] = fVec46[0];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			fVec47[1] = fVec47[0];
			fRec15[1] = fRec15[0];
			fRec14[1] = fRec14[0];
			fVec48[1] = fVec48[0];
			fRec13[1] = fRec13[0];
			fRec12[1] = fRec12[0];
			fVec49[1] = fVec49[0];
			fRec11[1] = fRec11[0];
			fRec10[1] = fRec10[0];
			fVec50[1] = fVec50[0];
			fRec9[1] = fRec9[0];
			fRec8[1] = fRec8[0];
			fVec51[1] = fVec51[0];
			fRec7[1] = fRec7[0];
			fRec6[1] = fRec6[0];
			fVec52[1] = fVec52[0];
			fRec5[1] = fRec5[0];
			fRec4[1] = fRec4[0];
			fVec53[1] = fVec53[0];
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			fVec54[1] = fVec54[0];
			fRec1[1] = fRec1[0];
			fRec0[1] = fRec0[0];
		}
	}

};

#endif
