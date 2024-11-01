import("stdfaust.lib");
ws = library("lib/waveshaper.lib");
filt = library("lib/filters.lib");
rev = library("lib/reverbtool.lib");

N_DEL_LINES = 4;
MAX_DELAY = 4; //seconds

diffusor(max_del,del_time,coef,sig) = sig : rev.diffusor.allpass_nested(1000,100,1000,200,0.7,0.6) with {
    time = del_time*(no.lfnoise(ma.SR/100)+1);
};

phase_mod(mod_freq,mod_depth,sig) = sig : filt.phase_rotator(lfo) with {
    lfo = os.osc(mod_freq)*ma.PI*mod_depth;
};

matrix(SIZE,coefs) = si.bus(SIZE) <: si.vecOp((si.bus(SIZE*SIZE),coefs),*) : par(i,SIZE,par(j,SIZE/2,+) : + ); // Work for 4x4, certainly not for other sizes.

input_section(tone,sig) = sig : *(0.25) : fi.svf.hp(120,1) : ws.xlim(1) : fi.lowpass(1,tone) <: hgroup("tap_gains",par(i,N_DEL_LINES,*(vslider("tap_gain_%i",0,-24,0,0.1):ba.db2linear)));
delay_section(i,del_time,tone,mod_freq,mod_depth,mod_rnd,diff,sig) = sig : de.fdelay(ma.SR*MAX_DELAY,ma.SR*del_time*del_ratio) : phase_mod(mod_freq_rnd,mod_depth) : diffusor(0.1,del_time,diff) : ws.xlim(1) : fi.lowpass(1,tone) with {
    hGroup(x) = hgroup("taps_delay_ratio",x);
    del_ratio = hGroup(vslider("ratio_tap_%i",1,0.,1.,0.01));
    mod_freq_rnd = mod_freq*(1+no.lfnoise0(mod_freq)*mod_rnd);
};
feedback_section(matrix_coefs,feedback) = si.bus(N_DEL_LINES) : matrix(N_DEL_LINES,matrix_coefs) : par(i,N_DEL_LINES,*(feedback));

process = input_section(tone) <: (si.vecOp((si.bus(N_DEL_LINES),si.bus(N_DEL_LINES)),+) : par(i,N_DEL_LINES,delay_section(i,del_time,tone,mod_freq,mod_depth,mod_rnd,diff)))~(feedback_section(coefs,feedback)) with {
    hGroup(x)       = hgroup("params",x);
    tone            = hGroup(vslider("tone",50,1,100,0.1)/100 : *(10000)); //scaling
    del_time        = hGroup(vslider("delay_time",1000,0,4000,0.01)/1000);
    mod_freq        = hGroup(vslider("mod_freq",0.3,0,8,0.01));
    mod_depth       = hGroup(vslider("mod_depth",0.,0.,100.,0.01)/100);
    mod_rnd         = hGroup(vslider("mod_rnd",0.,0.,100.,0.01)/50);
    diff            = hGroup(vslider("diffusion",0.,0.,100,0.01)/101);
    feedback        = hGroup(vslider("feedback",0.,0.,100.,0.01)/100);
    density         = hGroup(vslider("density",0.,0.,100.,0.01)/100);
    patch_points    = vgroup("routing_matrix",par(i,N_DEL_LINES,(hgroup("row_%i",par(j,N_DEL_LINES,checkbox("cell_%i%j"))))));
    hadamard        = par(i,N_DEL_LINES*N_DEL_LINES,1) : ro.hadamard(N_DEL_LINES*N_DEL_LINES);
    coefs           = si.vecOp((si.vecOp((hadamard,(density<:si.bus(N_DEL_LINES*N_DEL_LINES))),*),si.vecOp((patch_points,((1-density)<:si.bus(N_DEL_LINES*N_DEL_LINES))),*)),+);
};