/*
lif_main.c
Leaky Integrate and Fire neuromorphic model
7-15-2016
Neil McGlohon
*/

//includes
#include "lif.h"


tw_lptype model_lps[] =
{
     {
          (init_f) lif_init,
          (pre_run_f) lif_prerun,
          (event_f) lif_event_handler,
          (revent_f) lif_RC_event_handler,
          (commit_f) NULL,
          (final_f) lif_final,
          (map_f) lif_map,
          sizeof(lif_neuron_state)
     },
     { 0 },
};


// // COMMAND OPS EXAMPLE
//
// //Define command line arguments default values
// int total_mailboxes= 0;
// int total_post_offices = 0;
//
// //Command line opts
// const tw_optdef model_opts[] = {
//      TWOPT_GROUP("Mail Model"),
//      TWOPT_UINT("mailboxes", total_mailboxes, "Number of mailboxes in simulation"),
//      TWOPT_UINT("postoffices", total_post_offices, "Number of post offices in simulation"),
//      TWOPT_END()
// };


//Displays simple settings of the simulation
void displayModelSettings()
{
     if(g_tw_mynode ==0)
     {
          for (int i = 0; i < 30; i++)
          {
               printf("*");
          }

          //Put your settings to print here

          for (int i = 0; i < 30; i++)
          {
               printf("*");
          }
          printf("\n");

     }
}


//for doxygen
#define lif_main main
int lif_main(int argc, char** argv, char **env)
{

     return 0;
}
