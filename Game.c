#include "Hamlib/Hamlib.h"
#include "Game.h"
#include "Automat.h"
#include "Generate.h"
#include "Draw.h"
#include "gui.h"
#include "Statistics.h"

int mom_action=0;
int Get_Action()
{
    return mom_action;
}
void Game_Thread() 
{
	Statistics *s=statistics_new();
	int laststep=-1;
	while(1)
	{
		if(!SINGLEPLAYER && get_step()==laststep)
		{
			laststep=get_step();
			Wait(0.001);
			continue;
		}
		set_finished(0);
		Wait(0.001);
		statistics_next(automat,s);
		Hauto_OBJ_Exec(automat,s);
		set_finished(1);
        //save to file such that python can read it
        {
            int i,j,k;
            char* toWrite=(char*) malloc ((worldsize*worldsize+worldsize)*sizeof(char));
            system("rm toPy.txt");
            k=0;
            for(i=0;i<automat->n;i++)
            {
                for(j=0;j<automat->n;j++)
                {
                    char dummy='#';
                    int state=GetCell(i,j,Cell,state);
                    if(state==ROCK)
                        dummy='R';
                    if(state==OPENROCK)
                        dummy='O';
                    if(state==SWITCH)
                        dummy='1';
                    if(state==OFFSWITCH)
                        dummy='0';
                    if(state==FOOD)
                        dummy='F';
                    if(state==AGENT)
                        dummy='A';
                    if(state==WATER)
                        dummy='W';
                    toWrite[k]=dummy;
                    k++;
                }
                toWrite[k]='\n';
                k++;
            }
            hfio_textFileWrite("toPy.txt",toWrite);
        }
        system("python MOM_Agent_Fraction.py");
        {
            char *toRead=hfio_textFileRead("fromPy.txt");
            if(toRead[0]=='0')
                mom_action=0;
            if(toRead[0]=='1')
                mom_action=1;
            if(toRead[0]=='2')
                mom_action=2;
            if(toRead[0]=='3')
                mom_action=3;
        } 
		if(automat->t%10==0)
			printf("%f people\n",s->amount_of_people);
		laststep=get_step();
		Wait(0.001);
	}					
}
void Game_Init()
{
	////////////////////////// MAKE EVENT HANDLERS KNOWN TO HAMLIB //////////////////////////////////////////////
	hnav_SetRendFunc(draw);
	hrend_2Dmode(0.5,0.6,0.5);
	////////////////////////// LOAD TEXTURES ///////////////////////////////////////////////////////////////
	hfio_LoadTex("textures/command.tga",&COMMAND);
	hfio_LoadTex("textures/forest.tga",&FOREST);
	hfio_LoadTex("textures/street.tga",&STREET);
	hfio_LoadTex("textures/street_men.tga",&STREET_MEN);
	hfio_LoadTex("textures/house.tga",&HOUSE);
	hfio_LoadTex("textures/rock.tga",&ROCK);
	hfio_LoadTex("textures/rock.tga",&GPUTex);
	hfio_LoadTex("textures/house_men.tga",&HOUSE_MEN);
	hfio_LoadTex("textures/grass_men.tga",&GRASS_MEN);
	hfio_LoadTex("textures/forest_men.tga",&FOREST_MEN);
    hfio_LoadTex("textures/and.tga",&AND);
    hfio_LoadTex("textures/neg.tga",&NEG);
    hfio_LoadTex("textures/xor.tga",&XOR);
    hfio_LoadTex("textures/or.tga",&OR);
    hfio_LoadTex("textures/openrock.tga",&OPENROCK);
    hfio_LoadTex("textures/offswitch.tga",&OFFSWITCH);
    hfio_LoadTex("textures/switch.tga",&SWITCH);
    hfio_LoadTex("textures/offcurrent.tga",&OFFCURRENT);
    hfio_LoadTex("textures/current.tga",&CURRENT);
    hfio_LoadTex("textures/rock.tga",&ROCK);
    hfio_LoadTex("textures/water.tga",&WATER);
    hfio_LoadTex("textures/agent.tga",&AGENT);
    hfio_LoadTex("textures/bridge.tga",&BRIDGE);
    hfio_LoadTex("textures/grass.tga",&GRASS);
    hfio_LoadTex("textures/food.tga",&FOOD);
    hfio_LoadTex("textures/spur.tga",&SPUR);
	////////////////////////// OTHER INIT STUFF ///////////////////////////////////////////////////////////////
    gui_Init(); //since now buttons are involved GUI needs to be inited after texture loading
	srand(WORLD);
	landscape=Generate_PerlinNoise(worldsize,worldsize,Generate_WhiteNoise(worldsize,worldsize),8,0);
	automat=Hauto_OBJ_NEW(worldsize,Automat_Simulate,Cell_NEW);
	Generate_World();
	Thread_NEW(Game_Thread,NULL);
}
int main()
{
	Hamlib_CYCLIC(Game_Init,NULL,"1111111111111");
}
