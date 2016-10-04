#include "lif.h"


//Given an LP's GID (global ID)
//return the PE (aka node, MPI Rank)
tw_peid lif_map(tw_lpid gid)
{
     return (tw_peid) gid / g_tw_nlp;
}
