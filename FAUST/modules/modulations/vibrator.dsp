//#####################################Vibrator#####################################
// 
// ## Global explanation
//
// Vibrator is a rotary speaker emulation, roughly based on Doppolas speaker & Yamaha RA200 amp.
// 
// The Vibrator signal chain goes as follow :
// input signal -> first order crossover filter -> phase matching for low-end               -> mixing stage
//                                              -> speaker rotation algorithm for high end  ->
//
// The speaker rotation algorithm can be split in two part : a doppler effect and a tremolo effect.
//      The doppler effect is created by using an allpass block with some parallel delay lines inside
//      The tremolo effect is done by using an hilbert transform and then by creating on frequency linear modulated phase shift.
//          This constantly modulated signal (modulated signal) can be sum with one of the hilbert transform output (comparative signal) to create a tremolo effect.
//          Adding a first order allpass filter on the comparative signal creates a more interesting effect.
//
// The low-end of the signal is also processed by an Hilbert transform to reproduce the frequency linear modulated phase shift but without adding a comparative signal.
// It allows to preserve a good phase matching when summing back the low and high part of the signal.
// 
// The vibrator function can have N heads specified. The default process function has two of the mixed in stereo using a constant energy panning law.
//
// ## Todo-List:
// 
// ### Backlog:
// - [ ] - Refactor code to extract the following functions:
//       - [ ] - SpinningSpeaker
//       - [ ] - DummySpinningSpeaker
//
//
// ### Working On:
//
// - [ ] - Code cleaning
//       - [ ] - Document code
//       - [x] - Extract mixer from vibrator 
//       - [x] - Refactor the code to a vibrator(N,Speaker_1,Speaker_2,...,Speaker_N,sig) function that is called inside the process function.
// 
// ### Done:
//
// - [x] - Adding randomness is too computanial heavy for the current state of Bela.PAM
//       - [x] Find a mecanism to bypass randomness -> Use explicit substitution
// - [x] - Fix fast rotary sound
// - [!] - Ramp should be linear and not exponential
//

declare name "Vibrator";
declare author "Jean-Loup Pecquais";
declare version "1.00";

import("../../lib/filters.lib");
import("../../lib/effects.lib");
import("../../lib/math.lib");
import("../../lib/structure.lib");
import("../../lib/mixer.lib");
import("stdfaust.lib");

create_speaker(speed_offset_percent,phase_offset,delay_offset,doppler_del1,doppler_del2,rotation_direction) = speed_offset_percent,phase_offset,delay_offset,doppler_del1,doppler_del2,rotation_direction;

vibrator(f_cross,num_head_speakers,speakers,sig) = sig : crossover1(f_cross) : head_dry_wet : ((woofer(rotation_speed)),(global_head_delay:*(gain_compensation):head_rotors(rotation_speed,am_depth,PHASE_DEPTH))) with {
    /*
    Constant declaration
    */
    MAX_DELAY = 192; //samples
    NUM_OF_SPEAKERS = num_head_speakers+1;
    HEAD_SPEAKERS_SHAFT_LEN = 0.18; //m
    GLOBAL_ALLPASS_GAIN = 0.5;              
    RANDOM_SPEED = 1.5; //Hz
    RANDOM_DEPTH = 0.25; // %
    PHASE_DEPTH = 1;
    HEADS_DELAY = 48; //ms
    SPEED_OF_SOUND = 340; //m.s-1

    NUM_PROPERTY_HEAD_SPEAKER = 6;          // Number of property that defines a head speaker
    IDX_SPEED_OFFSET_PERCENT = 0;
    IDX_PHASE_OFFSET = 1;
    IDX_DELAY_OFFSET = 2;
    IDX_DOPPLER_DEL1 = 3;
    IDX_DOPPLER_DEL2 = 4;
    IDX_ROTATION_DIRECTION = 5;
    get_property(property_idx,speaker_idx) = ba.selector(property_idx+speaker_idx*NUM_PROPERTY_HEAD_SPEAKER,num_head_speakers*NUM_PROPERTY_HEAD_SPEAKER,speakers);

    /////////////////////////////////////////////
    /*
    Rotation engine
    Engine:
    Output a phasor signal going from -pi to pi.
    */
    engine(rotation_speed,direction,phase_offset) = os.hsp_phasor(2*ma.PI,rotation_speed*pow(-1,direction),0,phase_offset)-ma.PI;
    get_distance_from_engine(shaft_len,angle) = (cos(angle)*0.5+0.5)*shaft_len*ma.SR/SPEED_OF_SOUND;
    randomize(i) = *(1+RANDOM_DEPTH*0.5*((par(i,NUM_OF_SPEAKERS+1,no.lfnoise(RANDOM_SPEED)):ba.selector(i,NUM_OF_SPEAKERS+1))));

    /////////////////////////////////////////
    /*
    "Head" speakers definition
    rotation_speed: modulation frequency in Hz.
    am_depth: 0 is no tremolo effect. 1 is maximum tremolo effect.
    phase_depth: 0 is no phase modulation. 1 is maximum phase modulation. Note that depth of amplitude modulation depends on the phase depth
    */
    head_rotors(rotation_speed,am_depth,phase_depth,sig) = sig*(ba.db2linear(3*(3-N))) <: doppler : gain_compensation : hilbert_modulators with {
        N = num_head_speakers;

        _am_depth = clip(0,1,am_depth)*0.5;

        engines = par(i,N,engine((rotation_speed : randomize(i))*(1+get_property(IDX_SPEED_OFFSET_PERCENT,i)),get_property(IDX_ROTATION_DIRECTION,i),get_property(IDX_PHASE_OFFSET,i)));
        lfos = engines : par(i,N,cos);
        dels = engines : par(i,N,get_distance_from_engine(HEAD_SPEAKERS_SHAFT_LEN)+get_property(IDX_DELAY_OFFSET,i));

        doppler = back_and_forth.feedback_summed(N,main,direct,loop) with {
            F_ABSORPTION = 4500;
            main = par(i,N,(de.fdelay1(MAX_DELAY,ba.selector(i,N,dels))));
            direct = par(i,N,(*(GLOBAL_ALLPASS_GAIN) <: (_,@(get_property(IDX_DOPPLER_DEL1,i)),@(get_property(IDX_DOPPLER_DEL2,i)))) :> _);
            loop = par(i,N,*((-1)*(GLOBAL_ALLPASS_GAIN/3)) : fi.lowpass(1,F_ABSORPTION) <: (_,@(get_property(IDX_DOPPLER_DEL1,i)),@(get_property(IDX_DOPPLER_DEL2,i))) :> _);
        };

        allpass_f = 4000;
        Q = (tan(ma.PI*allpass_f/ma.SR)-1)/(tan(ma.PI*allpass_f/ma.SR)+1);
        hib_dry_path = (allpass(Q):*(-0.5));
        hilbert_modulators = par(i,N,hilbert_modulator_dry_path(ba.selector(i,N,lfos)*phase_depth,_am_depth,hib_dry_path));

        gain_compensation = par(i,N,*(sqrt(1-GLOBAL_ALLPASS_GAIN)));

    };

    woofer(rotation_speed,sig) = sig : hilbert_modulator(lfo,0) with {
        N = num_head_speakers;
        SPEED_OFFSET = sum(i,N,get_property(IDX_SPEED_OFFSET_PERCENT,i)) : /(N);
        offset = 0.5*ma.PI;
        woofer_engine = engine(rotation_speed*(1+SPEED_OFFSET),1,offset);
        lfo = cos(woofer_engine) : randomize(2);
    };

    //Routing functions
    head_dry_wet = (_,(_<:_,_)) : (+(_,*(sqrt(1-treble_mix))),*(sqrt(treble_mix)));
    stereo_reduction = ((_<:(_,_)),_,_) : ro.cross2 : (+,+);

    //Delay
    global_head_delay = de.fdelay1(MAX_DELAY,(HEADS_DELAY:randomize(3)));

    //Parameters
    slow_speed  = hslider("slow_rotation_speed",0.6,0.1,4,0.01);
    fast_speed  = hslider("fast_rotation_speed",6,4,10,0.1);
    slow_fast   = hslider("slow_fast",0,0,1,1);
    is_spinning = 1-hslider("break",0,0,1,1);
    ramp_time   = hslider("ramp_time",3,0,20,0.1);
    treble_mix  = hslider("mix",50,0,100,0.1)/100;
    am_depth    = 1-hslider("mic_distance",40,0,100,0.1)/100;

    rotation_speed = is_spinning*ba.if(slow_fast,fast_speed,slow_speed) : si.smooth(ba.tau2pole(ramp_time));
    gain_compensation = ba.db2linear(3*am_depth);
};

process = vibrator(FC_XOVER,NUM_OF_HEAD_SPEAKERS,(speaker_1,speaker_2)) : mix with {
    /*
    Constant declaration
    */
    FC_XOVER = 1200;
    NUM_OF_HEAD_SPEAKERS = 2;

    speaker_1 = create_speaker(0.,0.5*ma.PI,0,11,23,1);
    speaker_2 = create_speaker(0.2,0.5*ma.PI,7,17,31,-1);
    width = hslider("stereo_width",50,0,100,0.1);

    mix = ((_<:_,_),(si.bus(NUM_OF_HEAD_SPEAKERS) : stereo_mixer(NUM_OF_HEAD_SPEAKERS))) : routing.interleave(4) : (+,+); 

};