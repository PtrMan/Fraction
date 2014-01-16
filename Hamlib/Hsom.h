////////////Hamlib Module//////////////////////
//> Hsom - HamModule
//> Hamlib self organizing maps
//+ Creator "Patrick Hammer"
//+ License "GPL" Language "C"
//+ Changedate "06.09.2008" State "50%"
///////////////////////////////////////////////
#ifndef Hsom
#define Hsom
///////////////////////////////////////////////

//---------------DEFINES---------------------//
#pragma region DEFINES
#define MODE 0 //0=speed 1=safeness/quality
#define EXT  1 //not only std
#pragma endregion
//-------------------------------------------//

//+++++++++++++++DEPENCIES+++++++++++++++++++//
#pragma region DEPENCIES
//STD:
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
//OTHER:
//SAME:
#include "Hutil.h"
#include "Hrend.h"
#pragma endregion
//+++++++++++++++++++++++++++++++++++++++++++//

/////////////////MODULE-DATA///////////////////
struct Hsom
{
	int mode; //will be set to MODE on init 
	int init; //0
	//::
	int bRenderLines;
}
hsom;
typedef struct
{
	float ***links; //new float [SomSize][SomSize][numInputs]; //Verbindungen
	float *inputs; //new float[numInputs]; //eingang
	float **coords1; //new float[SomSize][SomSize]; // Koordinaten der Neuronen
	float **coords2; //new float[SomSize][SomSize]; // in der Karte.

	//SomSize*Somsize elems will be in the Somnet
	int recurrent; //recurrent som?
	float leak,inmul; //how much does the leaky integrator leak?
	int SomSize,numInputs;
	int minx, miny; // wo is der winner?
	float eta,gamma,etashrink,gammashrink; // lernparams
	float starteta,startgamma;
	float VisActivationMult;
	float outmul;
}Hsom_OBJ;
///////////////////////////////////////////////

//:::::::::::::::MODULE-IO::::::::::::::::::://
void hsom_INIT();
void hsom_UPDATE();
void hsom_CHECK();
//::
void hsom_RenderLayer(float *layer,int from,int to);
Hsom_OBJ *hsom_OBJ_NEW(int SOMSize,int nInputs,float Gamma,float Eta,float gammashrink,float etashrink,int bRecurrent,float IntegratorLeak,float IntegratorInputMul);
void hsom_OBJ_RESET(Hsom_OBJ *som);
void hsom_OBJ_FREE(Hsom_OBJ *ql);
void hsom_OBJ_COPY(Hsom_OBJ *TO,Hsom_OBJ *FROM);
void hsom_OBJ_Render(Hsom_OBJ *som);

int hsom_OBJ_getWinnerX(Hsom_OBJ *som);
int hsom_OBJ_getWinnerY(Hsom_OBJ *som);
void hsom_OBJ_Input(Hsom_OBJ *som,float *input);	//size=som->numInputs
void hsom_OBJ_Output(Hsom_OBJ *som,float *out);//size=som->numInputs
void hsom_OBJ_Adapt(Hsom_OBJ *som,float *input);	//size=som->numInputs
void hsom_OBJ_setWinner(Hsom_OBJ *som,int i);		//size=2*som->SomSize
void hsom_OBJ_getWinner(Hsom_OBJ *som,float *categorized,float neighbormul); //size=2*som->SomSize
void hsom_OBJ_getWinner1Dim(Hsom_OBJ *som,float *categorized);
//::::::::::::::::::::::::::::::::::::::::::://
#endif
/*................COMMENTS...................//
>>todo:

>>done:

>>others:

//..........................................*/
