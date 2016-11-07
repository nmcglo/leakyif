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
typedef enum
{
     HEARTBEAT = 1,
     FIRING,
     INPUT
} lif_msg_type;

typedef struct
{
     int refract_length; //Rest time after spiking before membrane voltage is allowed to accrue
     double R_mem; //Resistance of the membrane
     double C_mem; //Capacitance of the membrane
     double Tau; //Time constant. Typically Rmem * Cmem

     double V_thresh; //Threshold by which the voltage spikes if crossed
     double V_spike; //Amount that the voltage is spiked when the threshold is crossed

     double I_bias; //The bias of the presynaptic current
     double I_input_at_big_tick; //The input current, based on the weights of neurons that sent messages to it in this tick

     double V_mem; //The Voltage of the membrane
     double V_last;

     int* rec_firings;
     int total_rec_firings;
     int grand_total_rec_firings;

     bool should_fire_at_next_tick;

     bool is_Input_Neuron; //Is the neuron an input neuron?
     double chance_of_firing_each_timestep; //Chance of fireing each Timestep


     int number_of_outgoing_connections;
     tw_lpid* outgoing_adjacency;

     int number_of_incoming_connections;
     double* incoming_weights;

     int last_firing_time;

     //Things for analysis
     double* V_history;
     int firing_count;
     bool* firing_history;

     int rng_count;

} lif_neuron_state;


typedef struct
{
     tw_lpid sender;
     tw_lpid recipient;
     lif_msg_type mess_type;
     lif_neuron_state encodedState;
} neuron_mess;


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
extern int getNextBigTick(tw_lp *lp);


//IO STUFF --------------------------------

extern void exportArrayToCSV(char* filename, double* myArray, int length);
extern void export2DArrayToCSV(char* filename, double** myArray, int width, int height);
extern void export2DBoolArrayToCSV(char* filename, bool** myArray, int width, int height);

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

double** all_v_history;
bool** all_firing_history;





#endif
