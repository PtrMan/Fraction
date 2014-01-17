from MOM import *
from itertools	 import chain, combinations, product
from collections import defaultdict

Goal="lol should be active" #example definition which makes sense
Actions=["switch","switch2","switch3"]
OldStates=[list(set(z)) for z in [["light is active"],["lol is active"],["light is active"],["lol is active"],["light is active"],["lol is active"],["stefan takes a sip","this is noise","switch2 is active"],["stefan takes a sip"],
["this is noise","switch2 is active","switch is active","switch3 is active"],["stefan takes a sip","light is active","this is a example"],["switch is active","switch3 is active"],["light is active","watafaq"],["switch is active","thomson is cool","switch3 is active"],["light is active","i like sand"]]]

def powerset(iterable):
    s = list(iterable)
    return chain.from_iterable(combinations(s, r) for r in range(len(s)+1))

def tpriory(States,minQual): #a sort of special apriory algorithm, designed to work one-directional in time and just having one quality tune parameter
    Rules={}
    for l in range(1,len(States)):
        for k in range(len(States)-1):
            superM=list(product(powerset(States[k]),powerset(States[k+l])))
            for i in range(len(superM)):
                ELEM=superM[i]
                if [] not in ELEM:
                    try:
                        Rules[str(ELEM)]+=1              #supply increases quality of rule
                    except:
                        Rules[str(ELEM)]=1
                    for j in range(len(States)-1):
                        if k!=j:
                            superM2=list(product(powerset(States[j]),powerset(States[j+l])))
                            for h in range(len(superM2)):
                                ELEM2=superM2[h]
                                if [] not in ELEM2:
                                    if ELEM!=[] and ELEM[0] in [E[0] for E in ELEM2 if len(E)>0] and not all(RES in [E[1] for E in ELEM2] for RES in ELEM[1]):
                                        Rules[str(ELEM)]-=1  #lack of confidence decreases the quality of rule
        return [z for z in sorted(Rules, key=Rules.get) if Rules[z]>minQual]

def ChainTogether(States,minQual):
    Chain=[]
    for y in tpriory(States,minQual):
        x=eval(y)
        Supli=list(reversed([a for a in x[0]]+[b for b in x[1]]))
        for i in range(len(Supli)-1):
            Chain+=[Supli[i]+" after "+Supli[i+1]]
    return list(set(Chain))

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
    
print HandleSituation() #test
