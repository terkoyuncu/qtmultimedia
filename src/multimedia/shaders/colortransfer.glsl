#ifndef PERCEPTUAL_QUANTIZER
#define PERCEPTUAL_QUANTIZER

// Convert Rec.709 to linear
vec4 convertRec709ToLinear(vec4 rgba)
{
    const vec4 alphaMinusOne = vec4(0.099, 0.099, 0.099, 0);
    const vec4 alpha = alphaMinusOne + 1.;
    bvec4 cutoff = lessThan(rgba, vec4(0.081));
    vec4 high = pow((rgba + alphaMinusOne)/alpha, vec4(1./0.45));
    vec4 low = rgba/4.5;
    return mix(high, low, cutoff);
}


// This uses the PQ transfer function, see also https://en.wikipedia.org/wiki/Perceptual_quantizer
// or https://ieeexplore.ieee.org/document/7291452
const float SDR_LEVEL = 100.;

vec4 convertPQToLinear(vec4 rgba)
{
    const vec4 one_over_m1 = vec4(8192./1305.);
    const vec4 one_over_m2 = vec4(32./2523.);
    const float c1 = 107./128.;
    const float c2 = 2413./128;
    const float c3 = 2392./128.;

    vec4 e = pow(rgba, one_over_m2);
    vec4 num = max(e - c1, 0);
    vec4 den = c2 - c3*e;
    return pow(num/den, one_over_m1)*10000./SDR_LEVEL;
}

float convertPQToLinear(float sig)
{
    const float one_over_m1 = float(8192./1305.);
    const float one_over_m2 = float(32./2523.);
    const float c1 = 107./128.;
    const float c2 = 2413./128;
    const float c3 = 2392./128.;

    float e = pow(sig, one_over_m2);
    float num = max(e - c1, 0);
    float den = c2 - c3*e;
    return pow(num/den, one_over_m1)*10000./SDR_LEVEL;
}

float convertPQFromLinear(float sig)
{
    const float m1 = float(1305./8192.);
    const float m2 = float(2523./32.);
    const float c1 = 107./128.;
    const float c2 = 2413./128;
    const float c3 = 2392./128.;

    sig *= SDR_LEVEL/10000.;
    float p = pow(sig, m1);
    float num = c1 + c2*p;
    float den = 1. + c3*p;
    return pow(num/den, m2);
}

#endif
