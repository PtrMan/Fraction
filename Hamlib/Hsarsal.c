#include "Hsarsal.h"

///////////STANDARD-MODULE-FUNCTIONS///////////
void hsarsal_INIT()
{
	hsarsal.init=1;
}
void hql_UPDATE(){if(hsarsal.init){}}
void hql_CHECK(){}
///////////////////////////////////////////////

Hsarsal_OBJ *hsarsal_OBJ_NEW(int nStates,int nActions,float lambda,float gamma,float alpha)
{
	Hsarsal_OBJ *sarsal=(Hsarsal_OBJ*)malloc(sizeof(Hsarsal_OBJ));
	int x,y,i;
    sarsal->lastStateX=0;
    sarsal->lastStateY=0;
    sarsal->lastAction=0;
    sarsal->Alpha=alpha;
	sarsal->Gamma=gamma;
	sarsal->Lambda=lambda; 
	sarsal->nStates=nStates;
	sarsal->nActions=nActions;
	sarsal->curStateX=0;
    sarsal->curStateY=0;
	sarsal->Q=(float***)malloc(sarsal->nStates*sizeof(float**));
    sarsal->et=(float***)malloc(sarsal->nStates*sizeof(float**));
	for(x=0;x<sarsal->nStates;x++)
	{
		sarsal->Q[x]=(float**)malloc(sarsal->nStates*sizeof(float*));
        sarsal->et[x]=(float**)malloc(sarsal->nStates*sizeof(float*));
		for(y=0;y<sarsal->nStates;y++)
		{
			sarsal->Q[x][y]=(float*)malloc(sarsal->nActions*sizeof(float));
            sarsal->et[x][y]=(float*)malloc(sarsal->nActions*sizeof(float));
		}
	}
	return sarsal;
}
int hsarsal_OBJ_selectAction(Hsarsal_OBJ *sarsal,int StateX,int StateY,float reward)
{
    int maxk=0,i,j,k;
    float maxval=-999999;
    for(k=0;k<sarsal->nActions;k++)
    {
        if(sarsal->Q[StateX][StateY][k]>maxval)
        {
            maxk=k;
            maxval=sarsal->Q[StateX][StateY][k];
        }
    }
    int Action=0;
    if(drnd(1.0)<sarsal->Alpha)
        Action=(int)(drnd()*(((double)(sarsal->nActions+1))-0.001));
    else
        Action=maxk;
    float DeltaQ=reward+sarsal->Gamma*sarsal->Q[StateX][StateY][Action]-sarsal->Q[sarsal->lastStateX][sarsal->lastStateY][sarsal->lastAction];
    sarsal->et[sarsal->lastStateX][sarsal->lastStateY][sarsal->lastAction]=sarsal->et[sarsal->lastStateX][sarsal->lastStateY][sarsal->lastAction]+1;
    for(i=0;i<sarsal->nStates;i++)
    {
        for(j=0;j<sarsal->nStates;j++)
        {
            for(k=0;k<sarsal->nActions;k++)
            {
                sarsal->Q[i][j][k]=sarsal->Q[i][j][k]+sarsal->Alpha*DeltaQ*sarsal->et[i][j][k];
                sarsal->et[i][j][k]=sarsal->Gamma*sarsal->Lambda*sarsal->et[i][j][k];
            }
        }
    }
    sarsal->lastStateX=StateX;
    sarsal->lastStateY=StateY;
    sarsal->lastAction=Action;
    return sarsal->lastAction;
}
