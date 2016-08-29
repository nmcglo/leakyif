/*
mail.h
Mail System Simulator
7-15-2016
Neil McGlohon
*/


#ifndef _lif_h
#define _lif_h

#include "ross.h"



//STRUCTS ------------------------------

typedef struct
{
     tw_lpid sender;
     tw_lpid recipient;
} neuron_mess;


typedef struct
{

} lif_neuron_state;



//MAPPING -----------------------------

extern tw_lpid lpTypeMapper(tw_lpid gid);
extern tw_peid lif_map(tw_lpid gid);
extern int get_lif_neuron_GID(int lpid);


//DRIVER STUFF -----------------------------

extern void lif_init( lif_neuron_state *s, tw_lp *lp);
extern void lif_prerun( lif_neuron_state *s, tw_lp *lp);
extern void lif_event_handler( lif_neuron_state *s, tw_bf *bf, neuron_mess *in_msg, tw_lp *lp);
extern void lif_RC_event_handler( lif_neuron_state *s, tw_bf *bf, neuron_mess *in_msg, tw_lp *lp);
extern void lif_final( lif_neuron_state *s, tw_lp *lp);
extern void lif_commit( lif_neuron_state *s, tw_bf *bf, neuron_mess *m, tw_lp *lp);


//MAIN STUFF-----------------------------

extern tw_lptype model_lps[];

tw_stime lookahead;
unsigned int nlp_per_pe;
unsigned int custom_LPs_per_pe;


#endif
