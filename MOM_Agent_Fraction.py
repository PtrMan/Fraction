from LastSight import *
from MOM import *
from random import random
from copy import deepcopy

Goal="something should be open" #example definition which makes sense
Actions=["switch","switch2","switch3"]
OldStates=[list(set(z)) for z in [["light is active"],["lol is active"],["light is active"],["lol is active"],["light is active"],["lol is active"],["stefan takes a sip","this is noise","switch2 is active"],["stefan takes a sip"],
["this is noise","switch2 is active","switch is active","switch3 is active"],["stefan takes a sip","light is active","this is a example"],["switch is active","switch3 is active"],["light is active","watafaq"],["switch is active","thomson is cool","switch3 is active"],["light is active","i like sand"]]]

def Define(AgentActions,AgentGoal):
	global Actions,OldStates,Goal
	Actions=AgentActions
	OldStates=[]
	Goal=AgentGoal

def HandleSituation(Input=[]):
    global OldStates
    ResetKnowledge()
    Current2=ChainTogether(OldStates,2)+list(set([z for z in Input]))
    print Current2
    OldStates+=[Input] #world step finished
    for g in Current2: PrettyTell(g)  #now we need to find a way to achieve our goal with our actions, since we have to begin with one action.
    for g in Actions: #we need go through our actions and plan
		PrettyTell(g+" is active")
		Sol=PrettyTell("plan "+Goal+"?") 
		if "Path=[]" not in Sol:
			return " my plan is: "+Sol
		RetractLast()

with open("toPy.txt", "r") as text_file: 
    world=text_file.read().split("\n")

OldAssoc=deepcopy(Assoc)
agenty=0; agentx=0;
sizey=len(world)-1
sizex=len(world[0])
for y in range(sizey):
    for x in range(sizex):
        NoName=lambda y,x: ((y,x) not in Assoc.keys() or Assoc[(y,x)][0]!=world[y][x])
        if world[y][x]=="A":
            agenty,agentx=y,x
        if world[y][x]=="1" and NoName(y,x):
            Assoc[(y,x)]=(world[y][x],"switch"+str(y)+"x"+str(x)+" is active")
        if world[y][x]=="0" and NoName(y,x):
            Assoc[(y,x)]=(world[y][x],"switch"+str(y)+"x"+str(x)+" is not active")
        if world[y][x]=="R" and NoName(y,x):
            Assoc[(y,x)]=(world[y][x],"rock"+str(y)+"x"+str(x)+" is not open")
        if world[y][x]=="O" and NoName(y,x):
            Assoc[(y,x)]=(world[y][x],"rock"+str(y)+"x"+str(x)+" is open")
print agenty,agentx
OldStates=NewView=[z[1] for z in [Assoc[k] for k in Assoc.keys()]]
del Mem[0]
Mem=Mem+[NewView]
#####print HandleSituation() #test
action=1;
with open("LastSight.py", "w") as text_file: text_file.write("Mem="+str(Mem)+"\nAssoc="+str(Assoc)+"\nFinished=False")
with open("fromPy.txt", "w")   as text_file: text_file.write(str(action))




