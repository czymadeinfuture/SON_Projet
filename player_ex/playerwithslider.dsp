declare filename "playerwithslider.dsp";
declare name "playerwithslider";
import("stdfaust.lib");

// the sliders for controlling the parameters
noiseLevel = hslider("Noise Level", 0.02, 0, 0.1, 0.001);
lfoFreq = hslider("LFO Frequency[Hz]", 0.5, 0.1, 10, 0.01);
lfoDepth = hslider("LFO Depth", 0.001, 0, 5, 0.0001);
lpfFreq = hslider("LPF Cutoff Frequency[Hz]", 5000, 1000, 20000, 100);
compThreshold = hslider("Compressor Threshold", 0.5, 0.1, 1, 0.01);
compRatio = hslider("Compressor Ratio", 0.5, 0.1, 1, 0.01);

// define a low pass filter
basicLPF(freq) = fi.lowpass(1, freq);

// define lfo
lfo = os.osc(lfoFreq) * lfoDepth;

// make a white noise
noise = no.noise * noiseLevel;

// application of the low pass filter
filteredNoise = basicLPF(lpfFreq)(noise + lfo);

// make a compressor
compressor(signal, threshold, ratio) = ba.if(abs(signal) > threshold, signal * ratio, signal);

compressedSignal = compressor(filteredNoise, compThreshold, compRatio);

process = compressedSignal;