import("stdfaust.lib");

ms_encode(att_factor,l,r) = (l+r)*att_factor,(l-r)*att_factor;

process = ms_encode(attenuation) with {
    attenuation = hslider("Attenuation",0,-3,0,-3) : ba.db2linear;
};