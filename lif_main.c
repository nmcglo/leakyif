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
          printf("\n");

          printf("Leaky Integrate and Fire Model Configuration:\n");
          printf("\tnnodes: %i\n", tw_nnodes());
          printf("\tg_tw_nlp: %llu\n", g_tw_nlp);

          printf("\tNLPs per PE: %i\n", nlp_per_pe);

          printf("\tTotal Neurons: %i\n", total_neurons);

          //Put your settings to print here

          for (int i = 0; i < 30; i++)
          {
               printf("*");
          }
          printf("\n");

     }
}

void parse_and_init()
{
     system("python3 parseInput.py");
     FILE* fp;
     fp = fopen("generatedInput.dat", "rb");

     printf("Input File Loading...\n");

     //Get Total Neurons
     fread(&total_neurons,sizeof(int),1,fp);
     // printf("Total Neurons: %i\n",total_neurons);

     //Get Total Input Neurons
     fread(&total_input_neurons,sizeof(int),1,fp);
     // printf("Total Input Neurons: %i\n",total_input_neurons);

     //Get Input Neuron IDs
     double input_neurons_temp[total_input_neurons]; //TODO python ctypes is requiring doubles, find out how to write binary ints
     fread(input_neurons_temp,sizeof(input_neurons_temp),1,fp);

     input_neruons = malloc(total_input_neurons * sizeof(int));
     for(int i = 0; i < total_input_neurons; i++)
     {
          input_neruons[i] = (int)input_neurons_temp[i];
          // printf("%i ",input_neruons[i]);
     }
     // printf("\n");

     //Get Weight Matrix
     double* Weight_Matrix_temp;
     Weight_Matrix_temp = calloc(total_neurons*total_neurons, sizeof(*Weight_Matrix_temp));

     fread(Weight_Matrix_temp,sizeof(double)*total_neurons*total_neurons,1,fp);
     printf("Input File Loaded.\n");

     Weight_Matrix = malloc(total_neurons * sizeof(double *));
     for(int i = 0; i < total_neurons; i++)
     {
          Weight_Matrix[i] = malloc(total_neurons * sizeof(double));
     }

     for( int i = 0; i < total_neurons; i++)
     {
          for(int j = 0; j < total_neurons; j++)
          {
                    Weight_Matrix[i][j] = Weight_Matrix_temp[j + total_neurons*i];
                    // printf("%.2f ",Weight_Matrix[i][j]);
          }
          // printf("\n");
     }

     //exported data info to be collected at end
     all_v_history = calloc(total_neurons,sizeof(double*));
     for(int i = 0; i < total_neurons; i++)
     {
          all_v_history[i] = calloc(simulation_length,sizeof(double));
     }

     all_firing_history = calloc(total_neurons, sizeof(bool*));
     for(int i = 0; i < total_neurons; i++)
     {
          all_firing_history[i] = calloc(simulation_length,sizeof(bool));
     }

}

void exportFinalData()
{
          printf("Exporting Firing History...\n");
          export2DBoolArrayToCSV("firings.csv", all_firing_history, simulation_length, total_neurons);

          printf("Exporting V History...\n");
          export2DArrayToCSV("vh.csv", all_v_history, simulation_length, total_neurons);
          system("python3 plotHelper.py");
}


//for doxygen
#define lif_main main
int lif_main(int argc, char** argv, char **env)
{
     parse_and_init();

     // tw_opt_add(model_opts);
     tw_init(&argc, &argv);

     nlp_per_pe = 1;
     custom_LPs_per_pe = 1;

     simulation_length = 1000;

     g_tw_events_per_pe = 1000000;

     g_tw_nlp = (total_neurons);
     g_tw_lookahead = .0001;
     custom_LPs_per_pe = (g_tw_nlp / g_tw_npe)/tw_nnodes();

     displayModelSettings();

     g_tw_lp_types = model_lps;
     // g_tw_lp_typemap = lpTypeMapper;

     tw_define_lps(custom_LPs_per_pe, sizeof(neuron_mess));

     tw_lp_setup_types();

     tw_run();
     tw_end();

     // exportFinalData();


     return 0;
}
