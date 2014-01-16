#include "Hsom.h"

void hsom_RenderLayer(float *layer,int from,int to)
{
	int i,s;
	for(i=from,s=0;i<to;i++,s++)
	{
		hrend_SelectColor(layer[i]/2+0.5f,0.3f,0.3f,1.0f);
		hrend_DrawIcon(0.02*s+0.02,1-0.02,0,1.5,0,0);
	}
}
void hsom_OBJ_Output(Hsom_OBJ *som,float *out)
{
	int x=som->minx;
	int y=som->miny;
	int i;
	for(i=0;i<som->numInputs;i++)
	{
		out[i]=som->links[x][y][i]*som->outmul;
	}
}
void hsom_OBJ_Render(Hsom_OBJ *som)
{
#ifdef Hrend
#if OS!=9
	int x=0;
	int y=0;
	int i=0;
	float s=0.6f; //nicht wichtig

	hrend_DisableTex();
	hrend_EnableSmooth();

	if(hsom.bRenderLines)
	{
		for(x=0;x<som->SomSize;x++)
		{
			for(y=0;y<som->SomSize;y++)
			{
				for(i=0;i<som->numInputs;i++)
				{
					hrend_SelectColor(som->links[x][y][i]/2+0.5f,0.2f,0.2f,0.0f);
					hrend_DrawLine((float)0.02f*i+0.02f,(float)1-0.02f,(float)0,(float)0.02f+x*0.02f,(float)0.02f-y*0.02f+0.3f+s,(float)0);
				}
			}
		}
	}
	hsom_RenderLayer(som->inputs,0,som->numInputs);
	for(x=0;x<som->SomSize;x++)
	{
			for(y=0;y<som->SomSize;y++)
			{
				float curval=0.0f;
				for(i=0;i<som->numInputs;i++)
				{
					curval+=som->inputs[i]*som->links[x][y][i];
				}

				curval/=(float)som->numInputs;
				curval*=som->VisActivationMult;

				hrend_SelectColor(curval/2+0.5f,0.3f,0.3f,1.0f);
				hrend_DrawIcon(0.02+x*0.02,0.02-y*0.02+0.3f+s,0,1.8,1,0);
			}
	}
	hrend_SelectColor(1.0f,0.8f,0.8f,1.0f);
	hrend_DrawIcon(0.02+som->minx*0.02,0.02-som->miny*0.02+0.3f+s,0,1.8,-1,0);
	hrend_EnableTex();
#endif
#endif
}
void hsom_OBJ_setWinner(Hsom_OBJ *som,int i) //good for action map input
{
	som->minx=i/som->SomSize;
	som->miny=i%som->SomSize;
}
void hsom_OBJ_getWinner1Dim(Hsom_OBJ *som,float *categorized) //good for sensor map output
{
	int i,x,y;
	for(i=0;i<som->SomSize*som->SomSize;i++)
	{
		categorized[i]=0;
	}
	for(x=0;x<som->SomSize;x++)
	{
		for(y=0;y<som->SomSize;y++)
		{
			if(x==som->minx && y==som->miny)
			{
				categorized[x*som->SomSize+y]=1;
			}
		}
	}
}
void hsom_OBJ_getWinner(Hsom_OBJ *som,float *categorized,float neighbormul) //good for sensor map output
{
	int i;
	if(!neighbormul)
	{
		neighbormul=FLT_MAX;
	}
	for(i=0;i<som->SomSize;i++)
	{
		float abst=fabs(i-som->minx)*neighbormul;
		categorized[i]=max(0,min(1,1/abst));
	}
	for(i=0;i<som->SomSize;i++)
	{
		float abst=fabs(i-som->miny)*neighbormul;
		categorized[som->SomSize+i]=max(0,min(1,1/abst));
	}
}
int hsom_OBJ_getWinnerX(Hsom_OBJ *som)
{
	return som->minx;
}
int hsom_OBJ_getWinnerY(Hsom_OBJ *som)
{
	return som->miny;
}
void hsom_OBJ_Input(Hsom_OBJ *som,float *input)
{
	int i1,i2,j;
	float summe;
	float min=100000.0;

	if(!som->recurrent)
	{
		for(j=0;j<som->numInputs;j++)
		{
			som->inputs[j]=input[j];
			som->inputs[j]=max(min(1,som->inputs[j]),-1);
		}
	}
	else
	{
		for(j=0;j<som->numInputs;j++)
		{
			som->inputs[j]+=input[j]*som->inmul-som->leak;
			som->inputs[j]=max(min(1,som->inputs[j]),-1);
		}
	}
	for(i1=0;i1<som->SomSize;i1++)
	{
		for(i2=0;i2<som->SomSize;i2++)
		{ 
			summe=0.0;
			for(j=0;j<som->numInputs;j++)
			{
				summe=summe+(som->links[i1][i2][j]-som->inputs[j])*(som->links[i1][i2][j]-som->inputs[j]);
			}
			if (summe<=min)	//Erregungszentrumsberechnung
			{
				min=summe;
				som->minx=i1;
				som->miny=i2;
			}
		}
	}
}
void hsom_OBJ_RESET(Hsom_OBJ *som)
{
	int i1,i2,j;
	som->eta=som->starteta;
	som->gamma=som->startgamma;
	som->minx=0;
	som->miny=0;
	for(i1=0;i1<som->SomSize;i1++)
	{ 
		for(i2=0;i2<som->SomSize;i2++)
		{ 
			for(j=0;j<som->numInputs;j++)
			{ 
				som->links[i1][i2][j]=frnd()/100; //0.01*drnd() //0.001* //frnd()/100;
			} //Verbindungsstärken (anfangs schwach)
			som->coords1[i1][i2]=(float) i1*1.0; //Kartenkoords
			som->coords2[i1][i2]=(float) i2*1.0;
		}
	}
	for(j=0;j<som->numInputs;j++)
	{
		som->inputs[j]=0.0f;
	}
}
void hsom_OBJ_COPY(Hsom_OBJ *TO,Hsom_OBJ *FROM)
{
	int i1,i2,j;
	TO->eta=FROM->starteta; //START VALUE!!!!
	TO->etashrink=FROM->etashrink; 
	TO->gamma=FROM->startgamma; //START VALUE!!!
	TO->gammashrink=FROM->gammashrink; 
	TO->minx=FROM->minx; 
	TO->miny=FROM->miny; 
	TO->numInputs=FROM->numInputs; 
	TO->SomSize=FROM->SomSize; 
	TO->starteta=FROM->starteta; 
	TO->startgamma=FROM->startgamma;
	TO->outmul=FROM->outmul;
	TO->VisActivationMult=FROM->VisActivationMult;

	for(i1=0;i1<TO->SomSize;i1++)
	{ 
		for(i2=0;i2<TO->SomSize;i2++)
		{ 
			for(j=0;j<TO->numInputs;j++)
			{ 
				TO->links[i1][i2][j]=FROM->links[i1][i2][j];
			} 
			TO->coords1[i1][i2]=FROM->coords1[i1][i2];
			TO->coords2[i1][i2]=FROM->coords2[i1][i2];
		}
	}
	for(j=0;j<TO->numInputs;j++)
	{
		TO->inputs[j]=FROM->inputs[j];
	}
}
Hsom_OBJ *hsom_OBJ_NEW(int SOMSize,int nInputs,float Gamma,float Eta,float gammashrink,float etashrink,int bRecurrent,float IntegratorLeak,float IntegratorInputMul)
{
	int i1,i2,j,i,k;
	Hsom_OBJ *som=(Hsom_OBJ*)malloc(sizeof(Hsom_OBJ));
	som->inputs=(float*)malloc(nInputs*sizeof(float));
	som->coords1=(float**)malloc(SOMSize*sizeof(float*));
	som->coords2=(float**)malloc(SOMSize*sizeof(float*));
	som->links=(float***)malloc(SOMSize*sizeof(float**));
	som->recurrent=bRecurrent;
	som->outmul=1.0f;
	for(i=0;i<SOMSize;i++)
	{
		som->coords1[i]=(float*)malloc(SOMSize*sizeof(float));
		som->coords2[i]=(float*)malloc(SOMSize*sizeof(float));
		som->links[i]=(float**)malloc(SOMSize*sizeof(float*));
		for(j=0;j<SOMSize;j++)
		{
			som->links[i][j]=(float*)malloc(nInputs*sizeof(float));
		}
	}

	som->leak=IntegratorLeak;
	som->inmul=IntegratorInputMul;

	som->VisActivationMult=1.0f;
	som->SomSize=SOMSize;
	som->etashrink=etashrink;
	som->gammashrink=gammashrink;
	som->numInputs=nInputs;

	som->gamma=Gamma; //Adaptionsreichweite (wie weit d nachban betroffen sind)
	som->eta=Eta; //Lernparameter bzw Adaptionsstärke
	som->starteta=Eta;
	som->startgamma=Gamma;
	som->minx=0;
	som->miny=0;
	
	hsom_OBJ_RESET(som);

	return som;
}
void hsom_OBJ_FREE(Hsom_OBJ *som)
{
	int i,j;
	for(i=0;i<som->SomSize;i++)
	{
		for(j=0;j<som->SomSize;j++)
		{
			free(som->links[i][j]);
		}
		free(som->links[i]);
		free(som->coords2[i]);
		free(som->coords1[i]);
	}

	free(som->links);
	free(som->coords2);
	free(som->coords1);
	free(som->inputs);
	free(som);
}

float hsit(int i1,int i2,Hsom_OBJ *som)
{ // Nachbarschaftsfunktion (Wert abhängig vom abstand zum Erregungszentrum)
	float diff1=(som->coords1[i1][i2]-som->coords1[som->minx][som->miny])*(som->coords1[i1][i2]-som->coords1[som->minx][som->miny]);
	float diff2=(som->coords2[i1][i2]-som->coords2[som->minx][som->miny])*(som->coords2[i1][i2]-som->coords2[som->minx][som->miny]);
	return 1/sqrt(2*PI*som->gamma*som->gamma)*exp((diff1+diff2)/(-2*som->gamma*som->gamma)); //+som->coords2[i1][i2]-som->coords2[som->minx][som->miny]);
}
void hsom_OBJ_Adapt(Hsom_OBJ *som,float *input)
{
	int i1,i2,j;
	hsom_OBJ_Input(som,input);
	for(i1=0;i1<som->SomSize;i1++)
	{ 
		for(i2=0;i2<som->SomSize;i2++)
		{ 
			for(j=0;j<som->numInputs;j++)
			{	//adaptionsregel (änderung der gewichte)
				som->links[i1][i2][j]=som->links[i1][i2][j]+som->eta*hsit(i1,i2,som)*(som->inputs[j]-som->links[i1][i2][j]);
			}
		}
	}
	som->eta=som->eta*som->etashrink; //0.9998
	som->gamma=som->gamma*som->gammashrink;
}

///////////STANDARD-MODULE-FUNCTIONS///////////
void hsom_INIT()
{
	hsom.mode=MODE;
	hsom.init=1;
	//::
	hsom.bRenderLines=0;
}
void hsom_UPDATE(){if(hsom.init){}}
void hsom_CHECK(){}
///////////////////////////////////////////////

