/*
TODO-List
- [ ] - Fix fast rotary sound
- [ ] - Ramp should be linear and not exponential
*/

declare name "Rototor";
declare author "Jean-Loup Pecquais";
declare copyright "MyCompany";
declare version "1.00";

import("../../lib/filters.lib");
import("../../lib/effects.lib");
import("../../lib/math.lib");
import("../../lib/structure.lib");
import("stdfaust.lib");

/////////////////////////////////////////////
/*
Constant declaration
*/
MAX_DELAY = 96; //samples
HEAD_SPEAKERS_SHAFT_LEN = 0.18; //m
NUM_OF_HEAD_SPEAKERS = 2;
NUM_PROPERTY_HEAD_SPEAKER = 7; // Number of property that defines a head speaker
GLOBAL_ALLPASS_GAIN = 0.5;//hslider("allpasses_gain",50,0,100,0.1)/100;

SPEAKER_HEAD1 = environment {
    ALLPASS_GAIN = GLOBAL_ALLPASS_GAIN;
    SPEED_OFFSET_PERCENT = 0.;
    PHASE_OFFSET = 0.5*ma.PI;
    DELAY_OFFSET = 0;
    DOPPLER_DEL1 = 11;
    DOPPLER_DEL2 = 23;
    ROTATION_DIRECTION = 1;
    get = ALLPASS_GAIN,SPEED_OFFSET_PERCENT,PHASE_OFFSET,DELAY_OFFSET,DOPPLER_DEL1,DOPPLER_DEL2,ROTATION_DIRECTION;
};

SPEAKER_HEAD2 = environment {
    ALLPASS_GAIN = GLOBAL_ALLPASS_GAIN;
    SPEED_OFFSET_PERCENT = 0.2;
    PHASE_OFFSET = 0.5*ma.PI;
    DELAY_OFFSET = 7;
    DOPPLER_DEL1 = 17;
    DOPPLER_DEL2 = 31;
    ROTATION_DIRECTION = -1;
    get = ALLPASS_GAIN,SPEED_OFFSET_PERCENT,PHASE_OFFSET,DELAY_OFFSET,DOPPLER_DEL1,DOPPLER_DEL2,ROTATION_DIRECTION;
};

SPEAKER_HEAD3 = environment {
    ALLPASS_GAIN = GLOBAL_ALLPASS_GAIN;
    SPEED_OFFSET_PERCENT = -0.2;
    PHASE_OFFSET = 0.5*ma.PI;
    DELAY_OFFSET = 13;
    DOPPLER_DEL1 = 29;
    DOPPLER_DEL2 = 37;
    ROTATION_DIRECTION = 1;
    get = ALLPASS_GAIN,SPEED_OFFSET_PERCENT,PHASE_OFFSET,DELAY_OFFSET,DOPPLER_DEL1,DOPPLER_DEL2,ROTATION_DIRECTION;
};

speakers = SPEAKER_HEAD1.get,SPEAKER_HEAD2.get;

get_property(property_idx,speaker_idx) = ba.selector(property_idx+speaker_idx*NUM_PROPERTY_HEAD_SPEAKER,NUM_OF_HEAD_SPEAKERS*NUM_PROPERTY_HEAD_SPEAKER,speakers);
IDX_ALLPASS_GAIN = 0;
IDX_SPEED_OFFSET_PERCENT = 1;
IDX_PHASE_OFFSET = 2;
IDX_DELAY_OFFSET = 3;
IDX_DOPPLER_DEL1 = 4;
IDX_DOPPLER_DEL2 = 5;
IDX_ROTATION_DIRECTION = 6;

/////////////////////////////////////////////
/*
Rotation engine
Engine:
Output a phasor signal going from -pi to pi.
*/
engine(rotation_speed,direction,phase_offset) = os.hsp_phasor(2*ma.PI,rotation_speed*pow(-1,direction),0,phase_offset)-ma.PI;
get_distance_from_engine(shaft_len,angle) = (cos(angle)*0.5+0.5)*shaft_len*ma.SR/SPEED_OF_SOUND;

/////////////////////////////////////////
/*
"Head" speakers definition
rotation_speed: modulation frequency in Hz.
am_depth: 0 is no tremolo effect. 1 is maximum tremolo effect.
PHASE_DEPTH: 0 is no phase modulation. 1 is maximum phase modulation. Note that depth of amplitude modulation depends on the phase depth
*/
head_rotors(rotation_speed,am_depth,phase_depth,sig) = sig*(1.414) <: doppler : gain_compensation : hilbert_modulators with {
    N = NUM_OF_HEAD_SPEAKERS;

    _am_depth = clip(0,1,am_depth)*0.5;

    engines = par(i,N,engine(rotation_speed*(1+get_property(IDX_SPEED_OFFSET_PERCENT,i)),get_property(IDX_ROTATION_DIRECTION,i),get_property(IDX_PHASE_OFFSET,i)));
    lfos = engines : par(i,N,cos);
    dels = engines : par(i,N,get_distance_from_engine(HEAD_SPEAKERS_SHAFT_LEN)+get_property(IDX_DELAY_OFFSET,i));

    doppler = back_and_forth.feedback_summed(N,main,direct,loop) with {
        F_ABSORPTION = 3000;
        main = par(i,N,(de.fdelay1(MAX_DELAY,(ba.selector(i,N,dels))):fi.lowpass(1,F_ABSORPTION)));
        direct = par(i,N,(*(get_property(IDX_ALLPASS_GAIN,i)) <: (_,@(get_property(IDX_DOPPLER_DEL1,i)),@(get_property(IDX_DOPPLER_DEL2,i)))) :> _);
        loop = par(i,N,*((-1)*get_property(IDX_ALLPASS_GAIN,i)/3) : fi.lowpass(1,F_ABSORPTION) <: (_,@(get_property(4,i)),@(get_property(5,i))) :> _);
    };

    hilbert_modulators = par(i,N,hilbert_modulator(ba.selector(i,N,lfos)*phase_depth,_am_depth));

    gain_compensation = par(i,N,*(sqrt(1-get_property(IDX_ALLPASS_GAIN,i))));

};

woofer(rotation_speed,sig) = sig : hilbert_modulator(lfo,0) with {
    N = NUM_OF_HEAD_SPEAKERS;
    SPEED_OFFSET = sum(i,N,get_property(IDX_SPEED_OFFSET_PERCENT,i)) : /(N);
    phase_offset2 = hslider("mic_angle",0.5,-1,1,0.01)*ma.PI; //FIXME: Does not appear ?!?
    woofer_engine = engine(rotation_speed*(1+SPEED_OFFSET),1,phase_offset2);
    lfo = cos(woofer_engine);
};

////////////////////////////////////////////
POSITION_LIST = 0,1;
stereo_pan(position,sig) = sig <: (*(sqrt(1-position)),*(sqrt(position)));
mixer = si.bus(N_TOTAL) : mix with{
    N = NUM_OF_HEAD_SPEAKERS;
    N_TOTAL = N+1; //because of woofer input.
    mix_heads = par(i,N,stereo_pan(ba.selector(i,N,POSITION_LIST))) : routing.stereo_rotate(2*N) : (sum(i,N,_),sum(i,N,_));
    mix = ((_<:_,_),(si.bus(N) : mix_heads)) : routing.interleave(4) : (+,+); 
};

////////////////////////////////////////////
/*
Processsing function
Params description :
rotation_speed: modulation frequency in Hz.
am_depth: 0 is no tremolo effect. 100 is maximum tremolo effect.
PHASE_DEPTH: 0 is no phase modulation. 100 is maximum phase modulation. Note that depth of amplitude modulation depends on the phase depth
treble_mix: adjust ce quantity of head speakers mix. 100% always sounds like dry signal. (reversed behavior...)
bass_phase_offset: change the lower band phase from -180° to +180° continuously. Alter the tonal balance of the effect.
*/

process = crossover1(FC_XOVER) : head_dry_wet : ((woofer(rotation_speed)),(@(48):*(gain_compensation):head_rotors(rotation_speed,am_depth,PHASE_DEPTH))) : mixer with {
    //Constant
    FC_XOVER = 1200;
    BASS_DELAY = dist2samples(HEAD_SPEAKERS_SHAFT_LEN)/2;
    PHASE_DEPTH = 1;

    //Routing functions
    head_dry_wet = (_,(_<:_,_)) : (+(_,*(sqrt(1-treble_mix))),*(sqrt(treble_mix)));
    stereo_reduction = ((_<:(_,_)),_,_) : ro.cross2 : (+,+);

    //Parameters
    slow_speed = hslider("slow_rotation_speed",1,0.1,4,0.01);
    fast_speed = hslider("fast_rotation_speed",7,4,10,0.1);
    slow_fast = hslider("slow_fast",0,0,1,1);
    is_spinning = 1-hslider("break",0,0,1,1);
    ramp_time = hslider("ramp_time",6,0,20,0.1);
    treble_mix = hslider("mix",50,0,100,0.1)/100;
    am_depth = 1-hslider("mic_distance",50,0,100,0.1)/100;

    speed_factor = fast_speed/slow_speed;
    rotation_speed = is_spinning*slow_speed*pow(speed_factor,slow_fast) : si.smooth(ba.tau2pole(ramp_time));
    gain_compensation = ba.db2linear(3*am_depth);
};