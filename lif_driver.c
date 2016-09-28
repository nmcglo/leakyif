/*
lif_driver.c
Leaky Integrate and Fire neuromorphic model
7-15-2016
Neil McGlohon
*/


//Includes
#include "lif.h"


//--------------LIF Neuron stuff-------------

void lif_init (lif_neuron_state *s, tw_lp *lp)
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

void lif_prerun(lif_neuron_state *s, tw_lp *lp)
{
     int self = lp -> gid;

     int total_firings = (int) (self->chance_of_firing_each_timestep * simulation_length);

     for(int i = 0; i < total_firings; i++)
     {
          tw_stime scheduled_firing_big_tick = (int) tw_rand_unif(self->rng)*simulation_length;
          //Send self message at this time

          tw_event *e = tw_event_new(self,scheduled_firing_big_tick,lp);
          neuron_mess *mess = tw_event_data(e);
          mess->sender = self;
          mess->recipient = self;
          tw_event_send(e);
     }



}



void lif_event_handler(lif_neuron_state *s, tw_bf *bf, neuron_mess *in_msg, tw_lp *lp)
{




}

void lif_RC_event_handler(lif_neuron_state *s, tw_bf *bf, neuron_mess *in_msg, tw_lp *lp)
{

}

void lif_final(lif_neuron_state *s, tw_lp *lp)
{

}
