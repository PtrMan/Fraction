def ReplaceRequest(R): exec 'def Request(s,arg=\"N0\"): return R(s,arg)' in locals(),globals()
#Representations (type,transform), as what should words of type get transformed to? to variables, words?
Repres=[('PLAN', ' plan '), ('HOW', ' '), ('WHY', ' '), ('WHEN', ' '), ('IRRELEVANT', ' '), ('WHAT', ' N '), ('DET', ' '), ('NUM', ' N '), ('IS', ' is ')]
# Logic Operators (words,syntax,type)
Ops=[([', '], '', 'ENUMERATION'), (['if'], ':-', 'IF'), (['and'], ',', 'AND'), (['or'], ';', 'OR')]
# Word Types (word_regex,Type)
Types=[('(was|be|gets|is|are)$', 'IS'),
('(why)$', 'WHY'),
('(when)$', 'WHEN'),
('(how)$', 'HOW'),
('(plan)$', 'PLAN'),
('(after)$', 'AFTER'),
('(it|he|she|they)$', 'REF'),
('(should|would|actually|immediately|that|the|a|an|in|on|to|of|inside|onto|under|can)$', 'IRRELEVANT'),
('(not)$', 'NOT'),
('(probable)$', 'PROBABLE'),
('(much|many)$', 'NUM'),
('(more|\\+|plus)$', 'ADD'),
('(what|who|where|which|someone|something|somewhere|some|any|anything|anywhere|things|all|does)$', 'WHAT'),
('.*', 'N')]
#Reasons - which predicates are relevant as reasons to the why question?
Reasons=['do', 'iss', 'mem']
#Sentence Structures [WordTypes,Meaning,Description]
#in sentences the left part of A in Meaning is for assumptions, else its just a &, and the part after $ is a python command(T0 for now(time)):
Rules=[['N', '{0}', 'maybe a prolog question? debugging? be careful!'],
['N IS N', 'mem({0},{2},T0) A iss({0},{2},T0,P)', 'house is old'],
['PLAN N IS N', 'plan({1},{2},{3},T0,1,Path)', 'plan house is old?'],
['N IS N AFTER N IS N', 'iss({0},{2},T0+1,Path):- iss({4},{6},T0,P),append([{4},is,{6}],P,Path)', 'trafficlight is red after it is green'],
['N N N AFTER N N N', 'do({0},{1},{2},T0+1,Path):- do({4},{5},{6},T0,P),append([{4},{5},{6}],P,Path)', 'trafficlight gets red after it gets green'],
['N N', 'do({0},{1},unspecified,T0,P)', 'panther kill'],
['N N N', 'do({0},{1},{2},T0,P)', 'panther kill stefan'],
['N N N N', 'doestimes({0},{1},{3},{2},T0)', 'gimpa burns 4 tires'],
['N IS N N', 'len({3},{2},T0)', 'there are 5 houses'],
['N N ADD N N N N N', 'doestimesmorethan({0},{1},{3},{5},{6},{7},T0)', 'what has more feet than cat has eye?'],
['N N N N ADD N N N N', 'doestimesxmorethan({0},{1},{3},{2},{6},{7},{8},T0)', 'tiger has 2 feet more than cat has eye'],
['N IS N IS PROBABLE', 'probable({0},{2},not{2},nix,T0)', 'humans are red is probable?'],
['N N N IS PROBABLE', 'probable({0},{1},not{1},{2},T0)', 'humans kill pigs is probable?']]
#FOL axioms (Prolog) MOM system doesn't include 
Axioms=""":-use_module(library(clpfd)). :-style_check(-discontiguous). :-style_check(-singleton). 
do(G,turns,C-D,T0,P):-G in C..D.
do(G,ranges,C-D,T0,P):-G ins C..D.
do(we,minimize,D,T0,P):-once(labeling([min(D)],[D])).
do(we,label,D,T0,P):-once(label(D)).
%%%%%%%%%%%%%%%%% CLPFD system integration %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
:-use_module(library(clpfd)). :-style_check(-discontiguous). :-style_check(-singleton). 
do(G,turns,C-D,T0,P):-G in C..D.
do(G,ranges,C-D,T0,P):-G ins C..D.
do(we,minimize,D,T0,P):-once(labeling([min(D)],[D])).
%%%%%%%%%%%%%%%%% PDE Oracle system %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
dx(2).
diffusion(1).
dt(1).
begin([0,0,0,0,0,0,0]). %1D currently
tolerance(10).
simulate(B,[A],[C]):-C#=0.                                     %value=E  lastvalue=S, lastvalueleft=Ss, lastvalueright=Se
simulate(Ss,[S|Start],[E|End]) :- dt(DT), dx(DX), Start=[Se|St],  E#=DT*(2*S+Ss+Se)/(DX*DX), simulate(S,Start,End).
%example: simulate(0,[3000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],E).
simulate_n_times(Ss,End,End ,N):- N#=0, End=Start.
simulate_n_times(Ss,Start,End ,N) :- N#>0, N1#=N-1, simulate(Ss,Start,End2), simulate_n_times(Ss,End2,End, N1).
%example: simulate_n_times(0,[3000,0,0,0,0,0,0,0,0],E,10).
simulate_pde(Ht,Steps,Demand) :- tolerance(Tol), begin(Mat), append(Mat,[Ht],Start1), append(Start1,[0],Start), 
                              simulate_n_times(0,Start,[E|End], Steps), writeln(E),E+10#>Demand, E-10#<Demand.
%example: simulate_pde(300000,100,310).   since it will reach 314, and tolerance is 10, it will succeed
%ok that was simulation, everyone can do that
%now let's find initial condition such that the desired state is reached in 10 steps.
%example: D in 286000..300000, simulate_pde(D,10,310), label([D]).
%%%%%%%%%%%%%%%%%%%% Understanding rational numbers if needed %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
qadd([AZ,AN],[BZ,BN],C) :-  AN#\=0, BN#\=0, CN1#=AN*BN,    CZ1#=AZ*BN+BZ*AN, qequals([CZ1,CN1],C).
qmul([AZ,AN],[BZ,BN],C) :-  AN#\=0, BN#\=0, CN1#=AN*BN,    CZ1#=AZ*BZ,       qequals([CZ1,CN1],C).
qbigger([AZ,AN],[BZ,BN]) :- AN#\=0, BN#\=0, AZ*BN#>=BZ*AN.
qequals([AZ,AN],[BZ,BN]):-  AN#\=0, BN#\=0, AZ*BN#=BZ*AN.
qrats([]).
qrats([LH|LT]):- LH in -100..100, qrats(LT). %bound rationals to finite domain
%example query: http://upload.wikimedia.org/math/8/e/5/8e50b51a3fc429e753d82c099b38f06a.png
%Rats=[X3Z,X3N,LeftZ,LeftN,XZ,XN], qrats(Rats), qmul([3,1],[XZ,XN],[X3Z,X3N]), qmul([LeftZ,LeftN],[X3Z,X3N],[2,1]), qequals([LeftZ,LeftN],[6,1]), label(Rats).
%will result in XZ=-11 and XN=-99 which is 1/9
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
"""
#Procedural Knowledge
CustomCode="""
################## Advanced Command API ###############################################
def RecursiveChildren(inp): #make flexible queries through the ancestor and children hierachy possible for flexible problem specification not restricted to atoms 
    out=Request("what is a "+inp+"?")     #but to entities in the object universe of the AI
    return inp if out==[] else RecursiveChildren(out[0])
def RecursiveAncestor(inp):
    out=Request(inp+" is a what?","N2")
    return inp if out==[] else RecursiveAncestor(out[0])
def RecursiveEnd(inp):
    ancestor=RecursiveAncestor(inp)
    if ancestor==inp:
        return RecursiveChildren(inp)
    else:
        return ancestor
def RecursiveEndL(li):
    return [RecursiveEnd(a) for a in li]
######################################################################################
"""
exec CustomCode
#Language Knowledge
Sentences=["Var1 exceeds Var2 if Var1#>Var2",
"Var1 equals Var2 if Var1#=Var2",
"Var1 unequals Var2 if Var1#\=Var2",
"Var1 has distincts if all_distinct(Var1)",
"Var1 is distinct if all_distinct(Var1)"]
