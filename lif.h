/*
mail.h
Mail System Simulator
7-15-2016
Neil McGlohon
*/


#ifndef _lif_h
#define _lif_h

#include "ross.h"
#include <stdbool.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1


//STRUCTS ------------------------------

// typedef struct
// {
//      tw_lpid reverseNeighborID;
//      double synapseWeight;
// } synapticConnection;

typedef struct
{
     tw_lpid sender;
     tw_lpid recipient;
} neuron_mess;


typedef struct
{
     int refract_length; //Rest time after spiking before membrane voltage is allowed to accrue
     double R_mem; //Resistance of the membrane
     double C_mem; //Capacitance of the membrane
     double Tau; //Time constant. Typically Rmem * Cmem

     double V_thresh; //Threshold by which the voltage spikes if crossed
     double V_spike; //Amount that the voltage is spiked when the threshold is crossed

     double I_bias; //The bias of the presynaptic current

     double V_mem; //The Voltage of the membrane

     bool is_Input_Neuron; //Is the neuron an input neuron?
     double chance_of_firing_each_timestep; //Chance of fireing each Timestep

     tw_lpid* outgoing_adjacency;

     int number_of_incoming_connections;
     double* incoming_weights;

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

int total_neurons;
int simulation_length;
int total_input_neurons;

int* input_neruons;

double** Weight_Matrix;





#endif
