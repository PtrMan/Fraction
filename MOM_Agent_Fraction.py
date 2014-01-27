from LastSight import *
from MOM import *
from random import random, choice
from copy import deepcopy

Goal="rock18x13 should be open" #example definition which makes sense
Actions=["switch","switch2","switch3"]
OldStates=[list(set(z)) for z in [["light is active"],["lol is active"],["light is active"],["lol is active"],["light is active"],["lol is active"],["stefan takes a sip","this is noise","switch2 is active"],["stefan takes a sip"],
["this is noise","switch2 is active","switch is active","switch3 is active"],["stefan takes a sip","light is active","this is a example"],["switch is active","switch3 is active"],["light is active","watafaq"],["switch is active","thomson is cool","switch3 is active"],["light is active","i like sand"]]]

def Define(AgentActions,AgentGoal):
	global Actions,OldStates,Goal
	Actions=AgentActions
	OldStates=[]
	Goal=AgentGoal


def HandleSituation(Input=[]):
    global OldStates,Extracted
    ResetKnowledge()
    Extracted=Current2=list(set(Extracted+ChainTogether(OldStates,1)+list(set([z for z in Input]))))
    print "cur2",Current2,Input
    OldStates+=[Input] #world step finished
    for g in Current2: PrettyTell(g)  #now we need to find a way to achieve our goal with our actions, since we have to begin with one action.
    for g in Actions: #we need go through our actions and plan
        PrettyTell(g+" is active")
        Sol=PrettyTell("plan "+Goal+"?") 
        if "Path=[]" not in Sol and str(Sol)!="set([])":
            print " my plan is: "+g+" and then "+Sol
            return g
        RetractLast()
    return None

with open("toPy.txt", "r") as text_file: 
    world=text_file.read().split("\n")

OldAssoc=deepcopy(Assoc)
Assoc={}
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
NewView=[z[1] for z in [Assoc[k] for k in Assoc.keys()]]
del Mem[0]
Mem=Mem+[NewView]
Actions=list(set(["switch"+z.split(" ")[0] for z in str(Mem).split("switch") if " " in z]))
print "Actions",Actions
OldStates=deepcopy(Mem)
Ret=HandleSituation() #test

##########OK THE AI HAS GAINED ENOUGH KNOWLDGE TO PLAN!!! ITS TIME TO FIND THE PATH FOR THE FIRST STEP AND EXECUTE IT :)
def Reconstruct_Taken_Path(parent,start,target):
    path=[target]
    current=target
    while current!=start:
        current=parent[current]
        path.append(current)
    return list(reversed(path))

def Shortest_Path(start,target,M,sz):
     besucht=set([])
     parent={}
     queue=[]
     queue.append(start)
     while len(queue)>0:
         active=queue.pop(0)
         besucht.add(active)
         if active == target:
             return Reconstruct_Taken_Path(parent,start,target)
         for (px,py) in [(active[0]-1,active[1]),(active[0]+1,active[1]),(active[0],active[1]+1),(active[0],active[1]-1)]:
             if px<0 or py<0 or px>=sz or py>=sz or (px,py) in besucht:# or M[px][py]!='#':
                 continue
             parent[(px,py)]=active
             if (px,py) not in queue:
                 queue.append((px,py))
action=1
if Ret==None:
    action=choice([0,1,2,3]) #if it has not gained enough knowledge, then select randomly
else:
##########OK THE AI HAS GAINED ENOUGH KNOWLDGE TO PLAN!!! ITS TIME TO FIND THE PATH FOR THE FIRST STEP AND EXECUTE IT :)
    Y=int(Ret.replace("switch","").split("x")[0])
    X=int(Ret.replace("switch","").split("x")[1])
    (Soly,Solx)=Shortest_Path((agenty,agentx),(Y,X),world,sizex)[0]
    if Soly>agenty:
        action=1
    if Soly<agenty:
        action=2
    if Solx>agentx:
        action=3
    if Solx<agentx:
        action=4
######################################################################################################

with open("LastSight.py", "w") as text_file: text_file.write("Mem="+str(Mem)+"\nAssoc="+str(Assoc)+"\nExtracted="+str(Extracted))
with open("fromPy.txt", "w")   as text_file: text_file.write(str(action))




