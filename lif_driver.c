/*
lif_driver.c
Leaky Integrate and Fire neuromorphic model
7-15-2016
Neil McGlohon
*/


//Includes
#include "lif.h"


//--------------LIF Neuron stuff-------------

void lif_init (mailbox_state *s, tw_lp *lp)
{
     int self = lp -> gid;

     //init state data //TODO make this part of the input file and not hardcoded
     self->refract_length = 30;
     self->R_mem = 10;
     self->C_mem = 25;
     self->Tau = self->R_mem * self->C_mem;

     self->V_thresh = 1.6;
     self->V_spike = .25;

     self->I_bias = 1.5;

     self->V_mem = 0;

     self->is_Input_Neuron = FALSE;
     self->chance_of_firing_each_timestep = 0;
     if self < total_input_neurons
     {
          self->is_Input_Neuron = TRUE;
          self->chance_of_firing_each_timestep = .5 / total_neurons;
     }



}

void lif_prerun(mailbox_state *s, tw_lp *lp)
{
     int self = lp -> gid;

     int total_firings = (int) (self->chance_of_firing_each_timestep * simulation_length);

     for(int i = 0; i < total_firings; i++)
     {
          tw_stime scheduled_firing_big_tick = (int) tw_rand_unif(self->rng)*simulation_length;

     }
}



void lif_event_handler(mailbox_state *s, tw_bf *bf, letter *in_msg, tw_lp *lp)
{

}

void lif_RC_event_handler(mailbox_state *s, tw_bf *bf, letter *in_msg, tw_lp *lp)
{

}

void lif_final(mailbox_state *s, tw_lp *lp)
{

}
