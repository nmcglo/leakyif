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
     s->refract_length = 30;
     s->R_mem = 10;
     s->C_mem = 25;
     s->Tau = s->R_mem * s->C_mem;

     s->V_thresh = 1.6;
     s->V_spike = .25;

     s->I_bias = 1.5;
     s->I_input_at_big_tick = 0;

     s->V_mem = 0;
     s->V_last = 0;

     s->rec_firings = calloc(total_neurons, sizeof(tw_lpid));
     s->total_rec_firings = 0;
     s->grand_total_rec_firings = 0;

     s->should_fire_at_next_tick = false;

     s->last_firing_time = 0;

     s->is_Input_Neuron = FALSE;
     s->chance_of_firing_each_timestep = 0;
     if (self < total_input_neurons)
     {
          s->is_Input_Neuron = TRUE;
          s->chance_of_firing_each_timestep = .5 / total_neurons;
     }

     s->firing_count = 0;

     //----------------------------
     //Outgoing connections stuff
     int outConnections = 0;
     s->outgoing_adjacency = calloc(total_neurons, sizeof(tw_lpid)); //keep track of the ids of all outgoing neighbors

     for(int c = 0; c < total_neurons; c++)
     {
          if(Weight_Matrix[self][c] != 0)
          {
               s->outgoing_adjacency[outConnections] = c; //put the id number in the list
               outConnections +=1;
               // s->outgoing_adjacency[c] = 1;
          }
     }

     s->number_of_outgoing_connections = outConnections;
     // s->outgoing_adjacency = realloc(s->outgoing_adjacency, outConnections * sizeof(tw_lpid)); //Trimming the end and freeing up the previously allocated zeros

     printf("Outgoing Adjacency\n");
     for(int c = 0; c < total_neurons; c++)
     {
          printf("%llu, ",s->outgoing_adjacency[c]);
     }
     printf("\n");

     //----------------------------
     //Incoming connections stuff

     int inConnections = 0;
     s->incoming_weights = calloc(total_neurons, sizeof(double));
     for(int r = 0; r < total_neurons; r++)
     {
          double tempInWeight = Weight_Matrix[r][self]; //Weight of sender r to recipient self
          if(tempInWeight != 0)
          {
               s->incoming_weights[r] = tempInWeight;
               inConnections+= 1;
          }
     }
     s->number_of_incoming_connections = inConnections;

     printf("Incoming Weights\n");
     for(int r = 0; r < total_neurons; r++)
     {
          printf("%.3f, ", s->incoming_weights[r]);
     }
     printf("\n");

     s->V_history = calloc(simulation_length, sizeof(double));
     s->firing_history = calloc(simulation_length, sizeof(bool));
}

void lif_prerun(lif_neuron_state *s, tw_lp *lp)
{
     int self = lp -> gid;

     if(s->is_Input_Neuron)
     {
          // int total_firings = (int) (s->chance_of_firing_each_timestep * simulation_length) + ((tw_rand_unif(lp->rng) - .5) * 2 + 2);
          int total_firings = (int) (s->chance_of_firing_each_timestep * simulation_length);

          printf("%d: I am an input neuron! Total firings: %i\n",self, total_firings);

          int* firing_set = calloc(total_firings, sizeof(int));
          for(int i = 0; i < total_firings; i++)
          {

               double scheduled_firing_big_tick = tw_rand_unif(lp->rng)*simulation_length;
               bool alreadyIn = false;
               for(int j = 0; j < total_firings; j++)
               {
                    if(firing_set[j] == (int) scheduled_firing_big_tick)
                    {
                         alreadyIn = true;
                         break;
                    }
               }
               if(alreadyIn)
               {
                    i--;
                    continue;
               }
               firing_set[i] = (int) scheduled_firing_big_tick;

               double jitter = tw_rand_unif(lp->rng)/(total_neurons * 10000);

               double big_tick_with_jitter = ((int) scheduled_firing_big_tick) + jitter;

               //Send self message at this time
               // printf("%d: Scheudled fire order for time: %i\n",self,nextTime);
               tw_event *e = tw_event_new(self,big_tick_with_jitter,lp);
               neuron_mess *mess = tw_event_data(e);
               mess->mess_type = INPUT_FIRE_ORDER;
               mess->sender = self;
               mess->recipient = self;
               tw_event_send(e);
          }

          // for(int i = 0; i < total_firings; i++)
          // {
          //      printf("%i, ",firing_set[i]);
          // }
          // printf("\n");
     }

     else
     {
          for(int i = 1; i < simulation_length; i++)
          {
               double jitter = tw_rand_unif(lp->rng)/(total_neurons * 100);
               double big_tick_with_jitter = i + jitter;

               tw_event *e = tw_event_new(self,big_tick_with_jitter,lp);
               neuron_mess *mess = tw_event_data(e);
               mess->mess_type = HEARTBEAT_MESS;
               mess->sender = self;
               mess->recipient = self;
               tw_event_send(e);

          }
     }

}

//This method handles the voltage change and scheduling all messages with neighbors
void fire(lif_neuron_state *s, tw_lp *lp)
{
     int self = lp -> gid;
     for(int rec = 0; rec < s->number_of_outgoing_connections; rec++)
     {
          int next = getNextBigTick(lp);
          double halfwayToNext = next - .5;
          tw_stime delay = tw_rand_unif(lp->rng)/(total_neurons*100); //TODO this is suspicious

          tw_lpid recipient = s->outgoing_adjacency[rec];
          tw_event *e = tw_event_new(recipient, halfwayToNext+delay, lp);
          neuron_mess *mess = tw_event_data(e);
          mess->mess_type = FIRING_MESS;
          mess->sender = self;
          mess->recipient = recipient;
          tw_event_send(e);
     }
     s->V_mem = s->V_mem + s->V_spike;
     s->firing_count++;
     s->last_firing_time = (int)tw_now(lp);
     s->firing_history[(int)tw_now(lp)] = true;
}

int getNextBigTick(tw_lp *lp)
{
     int now = (int) tw_now(lp);
     return now+1;
}

double get_external_current_at_time(double theTime, tw_lpid lpid)
{
     //TODO make this a variable function
     return 1.5;
}

int getTimeSinceLastFiring(lif_neuron_state *s, tw_lp *lp)
{
     int now = (int)tw_now(lp);

     return now - s->last_firing_time;

}


void lif_event_handler(lif_neuron_state *s, tw_bf *bf, neuron_mess *in_msg, tw_lp *lp)
{
     if(tw_now(lp) < simulation_length)
     {
          int self = lp -> gid;

          if(in_msg->mess_type == HEARTBEAT_MESS) //Heartbeat message signifies at big ticks to integrate and check if there should be a firing or not
          {
               if(s->is_Input_Neuron)
               {
                    // double chance = s->chance_of_firing_each_timestep;
                    //
                    // double rn = tw_rand_unif(lp->rng);
                    //
                    // if(rn > 1 - chance)
                    // {
                    //      //Fire
                    //      fire(s,lp);
                    // }
               }
               else
               {
                    if(s->total_rec_firings > 0)
                    {
                         printf("Received Firings: ");
                         for(int i =0; i < s->total_rec_firings; i++)
                         {
                              printf("%i, ",s->rec_firings[i]);
                         }
                         printf(" at time: %i\n", ((int) tw_now(lp)));
                         s->rec_firings = calloc(total_neurons, sizeof(tw_lpid));
                         s->total_rec_firings = 0;
                    }


                    // printf("%i: %f I received a heartbeat message\n",self,tw_now(lp));
                    double lastVmem = s->V_last;
                    // if((int) tw_now(lp) < 1)
                    //      lastVmem = 0;
                    // else
                    //      lastVmem = s->V_mem;

                    double I_input = s->I_input_at_big_tick;

                    //Integrate
                    if(getTimeSinceLastFiring(s,lp) > s->refract_length) //Can you integrate? or are you in resting?
                    {
                         s->V_mem = lastVmem + ((-lastVmem + I_input*s->R_mem)/s->Tau);

                         //Do you fire?
                         if(s->V_mem >= s->V_thresh)
                         {
                              //If so then set yourselfs should fire to true
                              fire(s,lp);
                         }
                    }
                    else
                    {
                         s->V_mem = 0;
                    }



                    s->I_input_at_big_tick = 0;

                    s->V_history[(int)tw_now(lp)] = s->V_mem;
                    s->V_last = s->V_mem;
               }

          }
          else if(in_msg->mess_type == FIRING_MESS) //You received a firing message, do stuff
          {
               // printf("%i: I received a firing!\n",self);

               //Get who the message is from
               tw_lpid sender = in_msg->sender;
               s->rec_firings[s->total_rec_firings] = sender;
               s->total_rec_firings +=1;
               s->grand_total_rec_firings +=1;

               //Lookup the strength of the message from your states incoming weights array
               double inWeight = s->incoming_weights[sender];
               s->I_input_at_big_tick += inWeight;
          }
          else if(in_msg->mess_type == INPUT_FIRE_ORDER) //You received an input firing order, fire now. regardless of anything. assume this is big tick.
          {
               tw_stime theTime = tw_now(lp);
               // printf("%i: I received a firing order at %f: firing...\n",self,theTime);
               fire(s, lp);
          }
     }


}

void lif_RC_event_handler(lif_neuron_state *s, tw_bf *bf, neuron_mess *in_msg, tw_lp *lp)
{

}

void lif_final(lif_neuron_state *s, tw_lp *lp)
{
     int self = lp->gid;
     printf("%d: Total Firings Done: %d\n",self,s->firing_count);
     printf("%d: Total Firings Received: %d\n",self,s->grand_total_rec_firings);
     //export the data
     all_firing_history[self] = s->firing_history;
     all_v_history[self] = s->V_history;


     // if(self == total_neurons-1) //Last neuron
     // {
     //      // for(int i = 0; i < simulation_length; i++)
     //      // {
     //      //      printf("%f ",s->V_history[i]);
     //      // }
     //      printf("Exporting VHistory...\n");
     //      exportArrayToCSV("vh.csv", s->V_history, simulation_length);
     //
     //      system("python3 plotHelper.py");
     // }


}
