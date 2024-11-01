import("stdfaust.lib");

//select 3 stereo input
select3stereo(bpc) = ro.cross2, _, _ : _, _, ro.cross2 : _, ro.cross2, _ : ba.selectn(3,bpc), ba.selectn(3,bpc) : _,_;

// Various constant use in computation
dP = environment{
    degToRad = ma.PI/180;
    distToSpl = ma.SR/340;
};

// MS Encode/Decode matrix
msEncode = _,_ <: + , -;
msDecode = _,_ <: (+ : *(0.5)), (- : *(0.5));

allpass(c) = fi.tf22(1,0,-c,0,-1/c)*((1-(1/c))/(1-c));

allpassn(cs) = seq(i, order, allpass(cs : ba.selectn(order,i)))
with
{
    order = ba.count(cs);
};


hilbert(k) = (allpassn(c1s) : mem), allpassn(c2s)
with
{
    c1s = par(i,k,exp(ma.PI/2^(2*i)));
    c2s = par(i,k,exp(ma.PI/2^(2*i-1)));
};

minimalPhaseSplitter(gLow, gHigh) = _ <:(fi.lowpass(N,fc),fi.highpass(N,fc)):(*(gLow),*(gHigh))
with
{
    N = 1;
    fc = 700;
};

// Decorrelation algorithm : it works by applying a delay, then using a hilbert transform. High order allows little spatialization effect (affecting sound comming more from the left or the right). The delay time is computed by searching the maximum delay time when computing the distance between the source and the speakers. So, at pan = 0, the decorrelated signal is delayed (the "dry" signal is not delayed at pan = 0).
decorrelator(pan, spkrWidth, phaseRot) = _,_ :> *(0.5) : de.fdelayltv(4, dP.distToSpl, maxDelay) : fi.allpassnnlt(1, fi.av2sv(0.8)) <: hilbert(14) : *(cos(alpha)), *(sin(alpha)) :> _
with{
    bpc = checkbox("Bypass");

    N=4;
    smoothPan = pan:si.smoo;
    verySmoothPan = pan:si.smooth(0.9999);

    SSpkrL = sin(spkrWidth+verySmoothPan)*dP.distToSpl;
    SSpkrR = sin(spkrWidth-verySmoothPan)*dP.distToSpl;

    maxDelay = SSpkrL, SSpkrR : select2((SSpkrR>SSpkrL)+1);

    alpha = phaseRot+(smoothPan*(ma.PI/200))+ma.PI/4;
    //sign = 1-((pan > 0)*2):si.smooth(0.9999); //help timber
};

//ildPan create a stereo panning with gain differences. It is computed for a constant power.
ildPan(pan, spkrWidth, centerAttenuation, curveStep, centerCompensation) = *(gL), *(gR)
with
{
    csvFactor = centerAttenuation;

    q = tan(pan)/tan(spkrWidth);
    c = csvFactor/curveStep;

    gL = centerCompensation*pow(pow(1-q,c)/(pow(1-q,c)+pow(1+q,c)),1/csvFactor);
    gR = centerCompensation*pow(pow(1+q,c)/(pow(1-q,c)+pow(1+q,c)),1/csvFactor);

};

ildPanDualBand(pan, spkrWidth, centerAttenuation, curveStep, centerCompensation) = (minimalPhaseSplitter(gLAmp, gLInt), minimalPhaseSplitter(gRAmp, gRInt) ) : ro.interleave(2,2) :> _,_ //*(gL), *(gR)
with
{
    csvFactor = centerAttenuation;

    q = tan(pan)/tan(spkrWidth);
    c = csvFactor/curveStep;   

    cAmp = csvFactor;
    cInt = csvFactor/2;

    gLAmp = centerCompensation*pow(pow(1-q,cAmp)/(pow(1-q,cAmp)+pow(1+q,cAmp)),1/csvFactor);
    gRAmp = centerCompensation*pow(pow(1+q,cAmp)/(pow(1-q,cAmp)+pow(1+q,cAmp)),1/csvFactor);

    gLInt = centerCompensation*pow(pow(1-q,cInt)/(pow(1-q,cInt)+pow(1+q,cInt)),1/csvFactor);
    gRInt = centerCompensation*pow(pow(1+q,cInt)/(pow(1-q,cInt)+pow(1+q,cInt)),1/csvFactor);

};

//Perceptually Motivated Amplitude Panning
pmadPan(pan, spkrWidth, centerAttenuation) = *(gL), *(gR)
with
{
    anyPan = 30*pan/spkrWidth;
    twoThirdSpkrWidth = spkrWidth*2/3;
    icld = ba.if(abs(pan)<=twoThirdSpkrWidth, 0.425*anyPan, ba.if(pan>=twoThirdSpkrWidth, (0.85*(anyPan-20)+8.5),ba.if(pan<=(-1)*twoThirdSpkrWidth, 0.85*(anyPan+20)-8.5, 0)));

    gL = 1/pow(1+pow(10,(icld*centerAttenuation/20)),1/centerAttenuation);
    gR = pow(1-pow(gL,centerAttenuation),1/centerAttenuation);

};

//itdPan create a stereo panning with delays. It compute the distance of the sound source to the two left and right speakers, then, compute the difference between both and apply the minimum required delay to only one channel.
itdPan(pan, spkrWidth, preserveSum) = de.fdelayltv(N,dP.distToSpl,dL), de.fdelayltv(N,dP.distToSpl,dR)
with
{    
    N=4;

    SSpkrL = sin(spkrWidth+pan)*dP.distToSpl;
    SSpkrR = sin(spkrWidth-pan)*dP.distToSpl;

    minDelay = SSpkrL-SSpkrR;
    minDelayPos = minDelay >= 0;

    dL = abs(minDelayPos*minDelay), 0 : select2(preserveSum);
    dR = abs((1-minDelayPos)*minDelay), 0 : select2(preserveSum);
};

deltapan = _,_ <: (((ildPan(panPot:si.smoo, spkrWidth, centerAttenuation, curveStep, whichCenterCompensation), ildPanDualBand(panPot:si.smoo, spkrWidth, centerAttenuation, curveStep, whichCenterCompensation), pmadPan(panPot:si.smoo, spkrWidth, 2)):select3stereo(hybridMode)) : (*(invWidth), *(invWidth)) : msEncode) , decorrelator(panPot, spkrWidth, phaseRot)*width : msDecode(_,(_,_:>_)) : itdPan(panPot:si.smooth(0.9999), spkrWidth, preserveSum)
with{
    panPot = hslider("v:[0]Panner/[0]pan [style:knob]", 0, -100, 100 , 0.1)*0.01*30*dP.degToRad;
    widthParam = hslider("v:[0]Panner/[1]width [scale:log]", 0, 0, 100 , 0.1);

    width = sin(widthParam*0.01*45*dP.degToRad):si.smoo;
    invWidth = cos(widthParam*0.01*45*dP.degToRad)/(whichCenterAttenuation,1:select2(centerCompensation)):si.smoo;

    centerAttenuation = hslider("v:[1]Options/[1]centerAttenuation [style:menu{'-6dB':0;'-3dB':1}]", 1, 0, 1, 1)+1;
    curveStep = hslider("v:[1]Options/[2]curve [style:menu{'Rv':0;'Re':1]", 0, 0, 1, 1)+1;
    hybridMode = hslider("v:[1]Options/[2]hybridMode [style:menu{'Mono band':0;'Dual band':1};'PMAP':2}]", 0, 0, 2, 1);
    centerCompensation = hslider("v:[1]Options/[3]centerCompensation [style:menu{'No compensation':0;'Gain compensated':1}]", 0, 0, 1, 1);
    preserveSum = hslider("v:[1]Options/[4]preserveMono [style:menu{'Off':0;'On':1}]", 0, 0, 1, 1);
    phaseRot = hslider("v:[1]Options/[5]widthColoration", 0, ma.PI*(-1), ma.PI, 1):si.smoo;
    whichCenterAttenuation = 1.995262315,1.4125375446:select2(centerAttenuation-1);
    whichCenterCompensation = 1,whichCenterAttenuation:select2(centerCompensation);

    spkrWidth = 30*dP.degToRad;//hslider("[1]Width", 30, 30, 60 , 0.1)*dP.degToRad:si.smoo;
};

process = _ <: hgroup("DeltaPan", deltapan);
