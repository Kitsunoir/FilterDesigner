//--------------------------------- messages.h ---------------------------------

#ifndef MSGS_H
#define MSGS_H

#define R_ADVISE \
"If the source and load differ by a factor of\n"\
"less than 10, performance could be affected\n"\
"You might want to consider an impedance matching\n"\
"network instead."

//           Bandpass Filter Descriptions

#define BPF_PTC_DESC \
"\nParallel Tuned, Capacitor Coupled\n"\
"These passive BPFs are Butterworth derived\n"\
"bandpass filters offering a maximally flat\n"\
"passband response with decent skirt selectivity.\n"\
"\n"\
"The parallel LC topology is\n"\
"more flexible in that the filter can be designed\n"\
"for some image impedance that may be matched to the\n"\
"actual source/load impedances."\

#define BPF_PTLC_DESC \
"\nParallel Tuned LC Coupled\n"\
"These passive BPFs are Butterworth derived\n"\
"bandpass filters offering a maximally flat\n"\
"The LC coupled version offers added skirt\n"\
"selectivity.\n"\
"\n"\
"The parallel LC topology is\n"\
"more flexible in that the filter can be designed\n"\
"for some image impedance that may be matched to the\n"\
"actual source/load impedances."\

#define BPF_STC_DESC \
"\nSeries Tuned, Capacitor Coupled\n"\
"These passive BPFs are Butterworth derived\n"\
"bandpass filters offering a maximally flat\n"\
"passband response with decent skirt selectivity.\n"\
"\n"\
"The series tuned topology is useful for smallish\n"\
"source/load impedances that are equal. (T-line\n"\
"harmonic filters)"

#define BPF_STLC_DESC \
"\nSeries Tuned, LC Coupled\n"\
"These passive BPFs are Butterworth derived\n"\
"bandpass filters offering a maximally flat\n"\
"passband response with decent skirt selectivity.\n"\
"The LC coupled version offers added skirt\n"\
"selectivity.\n"\
"\n"\
"The series tuned topology is useful for smallish\n"\
"source/load impedances that are equal. (T-line\n"\
"harmonic filters)"

#define MFB_DESC \
"\nMulti-feedback Active Bandpass Filter\n"\
"\nGenerally quite good for Q-factors not in"\
"\nexcess of 10 or so. Higher Q-factors become"\
"\nproblematic, due to the gain limits of the"\
"\namplifier."\
"\n"\
"\nThe designer requires values for the cut-off"\
"\nfrequency, and capacitance"

#define STATEVAR_DESC \
"\nState Variable Bandpass Filter\n"\
"\nThis is a useful, high-Q implementation."\
"\nQ-factors up to 100 or more are possible"\
"\nwith this implementation. Excellent for all"\
"\nbut the most demanding low frequency filtering"\
"\nneeds"\
"\n"\
"\nThe designer requires values for the cut-off"\
"\nfrequency, Q-Factor, and voltage gain"

//         Filter characteristic descriptions

#define BUTTERWORTH_DESC \
"\nButterworth Characteristic\n"\
"\nThe Butterworth characteristic is a maximally"\
"\nflat amplitude response across the pass band."\
"\nThe phase characteristic is well behaved."\
"\nThe skirt selectivity is average."\
"\nThis makes the Butterworth a good, general purpose"\
"\nfilter."

#define CHEBYCHEV_DESC \
"\nChebychev Characteristic\n"\
"\nThe Chebychev characteristic maximizes"\
"\nskirt selectivity at the expense of both"\
"\nphase linearity and amplitude response."\
"\nThe passband is characterized by amplitude ripple"\
"\nand wild phase shifts near the cutoff frequency"\
"\nThis is the filter you would use where out of band"\
"\nrejection is most critical."

#define BESSEL_DESC \
"\nBessel Characteristic\n"\
"\nThe Bessel characteristic sacrifices both passband"\
"\namplitude response and skirt selectivity in favour"\
"\n of phase linearity and time delay. The amplitide"\
"\nbegins to roll off well before the cutoff frequency"\
"\nis reached. The skirt selectvity is the poorest of"\
"\nthe three characteristics. This characteristic is"\
"\nuseful where phase linearity, uniform group delay,"\
"\nand/or fast response to pulse type signals is required."

#define LP_CIN_DESC \
"\nThe lowpass, capacitor input topology is desirable as"\
"\nthis is a minimum inductor design, useful for the normal"\
"\nrange of source/load resistance. The parallel capacitor"\
"\nhelps with rejection of high frequency noise and unwanted"\
"\nOOB signals"

#define LP_LIN_DESC \
"\nThe lowpass, series inductor topology is useful for very low"\
"\ninput resistances, including the R_l= \"INF\" cases. It is also"\
"\nuseful with transistors, and as one half of a signal diplexer."

#define HP_CIN_DESC \
"\nThe series input capacitor topology is the usual, minimum inductor"\
"\ndesign choice. Also useful for cases where R_l -> INF. Useful for"\
"\nthe usual case where R_s ~= R_l."

#define HP_LIN_DESC \
"\nThe parallel inductor input topology is useful with transistor"\
"\nbased designs, as BJTs especially, like to see inductive loads to"\
"\nspeed up cutoff. Also useful as half of a diplexer."

//         App Error Messages

#define HELP_ERROR \
"No browser found on this system. Either you don't have"\
"\none of the browsers listed in /etc/browserpaths.cfg"\
"\nor it's in a non-standard location. Manually open"\
"\nDocuments/index.html and/or add your browser path to"\
"\nbrowserpaths.cfg"

#define CHEBYCHEV_ORDER_ERROR \
"It is not possible to design a passive Chebychev"\
"\nfilter with an even number of poles. Reselect an"\
"\nodd order or a different characteristic."

#define PBR_ERROR \
"The passband ripple factor is missing/incorrect"\
"\n(PB ripple can't be greater than 3.01db)"


#endif

//--------------------------------- messages.h ---------------------------------
