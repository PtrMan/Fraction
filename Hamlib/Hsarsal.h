////////////Hamlib Module/////////////
//> Hsarsal - A Q-learning module with eligibility traces
//> Object orientated and threadsafe
//+ Creator "Patrick Hammer"
//+ License "GPL" Language "C80"
//+ Changedate "16.01.2014", successor of Hql from 2008 - State "90%"
///////////////////////////////////////////////
#ifndef Hsarsal
#define Hsarsal
///////////////////////////////////////////////

//---------------DEFINES---------------------//
#pragma region DEFINES
#pragma endregion
//-------------------------------------------//

//+++++++++++++++DEPENCIES+++++++++++++++++++//
#pragma region DEPENCIES
//STD:
#include <stdlib.h>
#include "Hutil.h"
#pragma endregion
//+++++++++++++++++++++++++++++++++++++++++++//

/////////////////MODULE-DATA///////////////////
struct Hsarsal
{
	int init; //0
}
hsarsal;

typedef struct
{
    int lastStateX;
    int lastStateY;
    int lastAction;
    float Alpha; //=0.1;
    float Gamma; //=0.8;
    float Lambda; //=0.1; //0.1 0.5 0.9
	int nStates;
	int nActions;   
    float ***et;
    float ***Q; //state, action
    int curStateX;
	int curStateY;
}Hsarsal_OBJ;
///////////////////////////////////////////////

//:::::::::::::::MODULE-IO::::::::::::::::::://
void hsarsal_INIT();
void hsarsal_UPDATE();
void hsarsal_CHECK();
//::
Hsarsal_OBJ *hsarsal_OBJ_NEW(int nStates,int nActions,float lambda,float gamma,float alpha);											                                           //(    ..          ..             ..             0.8            0.9          1.0
int hsarsal_OBJ_selectAction(Hsarsal_OBJ *sarsal,int StateX,int StateY,float reward);
//::::::::::::::::::::::::::::::::::::::::::://
#endif
/*................COMMENTS...................//
>>todo: 

>>done:

>>others:

//..........................................*/
