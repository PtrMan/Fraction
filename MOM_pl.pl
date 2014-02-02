:-op(700,xfx,<-). :-op(450,xfx,..). :-op(1100,yfx,&).
Vs<-[Var | Dec] :- findall(Var,maplist(call,[Dec]),Vs).
len(A,B,T) :- Li<-[W| mem(W,A,T)], length(Li,Z), B=Z, B\==0.
len(A,B,T) :- Li<-A, len(Li,B,T).
do(A,B,C,T,P) :- current_predicate(mem/3), B\=turns, B\=ranges, B\=exceeds, B\=equals, B\=unequals, B\=minimize, B\=proves, mem(A,Z,T), do(Z,B,C,T,P).
iss(A,B,T,P) :- current_predicate(mem/3), mem(A,B,T).
iss(A,B,T,P) :- current_predicate(mem/3), mem(A,Z,T), mem(Z,B,T).
doestimes(X,Action,B,Times,T) :- current_predicate(mem/3), S<-[TimesW|(mem(W,X,T),doestimes(W,Action,B,TimesW,T))],sumlist(S,Sum),Times is Sum,Times\==0.
doestimes(X,Action,B,Times,T) :- current_predicate(mem/3), W<-[K|(mem(K,B,T),do(X,Action,K,T,P))],length(W,Times),Times\==0.
doestimes(X1,Action1,B1,Z,T) :- current_predicate(doestimesxmorethan/8), doestimesxmorethan(X1,Action1,B1,More,X2,Action2,B2,T), doestimes(X2,Action2,B2,Times2,T),Z is Times2+More.
doestimesmorethan(A,Action,B,X,Bction2,Y,T) :- doestimes(A,Action,B,N,T), doestimes(X,Bction2,Y,M,T), N>M. %lets define when something is probable true in a set of elements:
probable(M,H,NH,S,T):-current_predicate(mem/3), G<-[E|mem(E,M,T)],L<-[F|(mem(F,M,T),(iss(F,H,T,P);do(F,H,S,T,P2)))],D<-[V|(mem(V,M,T),(iss(V,NH,T,P3);do(V,NH,S,T,P4)))],length(D,U),length(L,Z),length(G,N),U==0,2*Z>N,N\==0.
plan(A,V,B,T,N,Path):-current_predicate(mem/3), call_with_depth_limit((V=is,iss(A,B,T,P);do(A,V,B,T,P2)),N,R),(number(R),reverse(P,Path);(R=depth_limit_exceeded,K is N+1,plan(A,V,B,T,K,Path))). %planning

:-use_module(library(clpfd)). :-style_check(-discontiguous). :-style_check(-singleton). 
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

do(Var1,exceeds,Var2,T0,P) :- Var1#>Var2.
do(Var1,equals,Var2,T0,P) :- Var1#=Var2.
do(Var1,unequals,Var2,T0,P) :- Var1#\=Var2.
do(Var1,has,distincts,T0,P) :- all_distinct(Var1).
 iss(Var1,distinct,T0,P) :- all_distinct(Var1).
iss(switch18x15,active,T0+1,Path):- iss(rock17x12,notopen,T0,P),append([rock17x12,is,notopen],P,Path).
iss(rock17x12,open,T0+1,Path):- iss(rock17x12,open,T0,P),append([rock17x12,is,open],P,Path).
iss(rock17x12,open,T0+1,Path):- iss(switch18x15,notactive,T0,P),append([switch18x15,is,notactive],P,Path).
iss(rock17x12,open,T0+1,Path):- iss(switch10x16,notactive,T0,P),append([switch10x16,is,notactive],P,Path).
iss(switch10x16,notactive,T0+1,Path):- iss(rock17x12,notopen,T0,P),append([rock17x12,is,notopen],P,Path).
iss(rock17x12,notopen,T0+1,Path):- iss(rock17x12,notopen,T0,P),append([rock17x12,is,notopen],P,Path).
iss(switch10x16,notactive,T0+1,Path):- iss(switch10x16,notactive,T0,P),append([switch10x16,is,notactive],P,Path).
iss(switch10x16,active,T0+1,Path):- iss(switch18x15,notactive,T0,P),append([switch18x15,is,notactive],P,Path).
iss(switch18x15,active,T0+1,Path):- iss(switch10x16,active,T0,P),append([switch10x16,is,active],P,Path).
iss(switch10x16,active,T0+1,Path):- iss(rock17x12,notopen,T0,P),append([rock17x12,is,notopen],P,Path).
iss(rock17x12,notopen,T0+1,Path):- iss(switch10x16,notactive,T0,P),append([switch10x16,is,notactive],P,Path).
iss(rock17x12,notopen,T0+1,Path):- iss(switch18x15,notactive,T0,P),append([switch18x15,is,notactive],P,Path).
iss(switch18x15,notactive,T0+1,Path):- iss(switch18x15,notactive,T0,P),append([switch18x15,is,notactive],P,Path).
iss(switch10x16,active,T0+1,Path):- iss(switch18x15,active,T0,P),append([switch18x15,is,active],P,Path).
iss(switch10x16,notactive,T0+1,Path):- iss(switch18x15,notactive,T0,P),append([switch18x15,is,notactive],P,Path).
iss(switch18x15,notactive,T0+1,Path):- iss(rock17x12,notopen,T0,P),append([rock17x12,is,notopen],P,Path).
iss(switch10x16,notactive,T0+1,Path):- iss(rock17x12,open,T0,P),append([rock17x12,is,open],P,Path).
iss(rock17x12,notopen,T0+1,Path):- iss(switch18x15,active,T0,P),append([switch18x15,is,active],P,Path).
iss(switch10x16,active,T0+1,Path):- iss(switch10x16,active,T0,P),append([switch10x16,is,active],P,Path).
iss(switch18x15,active,T0+1,Path):- iss(rock17x12,open,T0,P),append([rock17x12,is,open],P,Path).
iss(switch18x15,active,T0+1,Path):- iss(switch18x15,active,T0,P),append([switch18x15,is,active],P,Path).
iss(switch18x15,notactive,T0+1,Path):- iss(switch10x16,active,T0,P),append([switch10x16,is,active],P,Path).
iss(switch18x15,notactive,T0+1,Path):- iss(rock17x12,open,T0,P),append([rock17x12,is,open],P,Path).
iss(rock17x12,open,T0+1,Path):- iss(switch18x15,active,T0,P),append([switch18x15,is,active],P,Path).
iss(switch10x16,active,T0+1,Path):- iss(rock17x12,open,T0,P),append([rock17x12,is,open],P,Path).
iss(rock17x12,open,T0+1,Path):- iss(switch10x16,active,T0,P),append([switch10x16,is,active],P,Path).
iss(rock17x12,notopen,T0+1,Path):- iss(switch10x16,active,T0,P),append([switch10x16,is,active],P,Path).
iss(switch18x15,notactive,T0+1,Path):- iss(switch10x16,notactive,T0,P),append([switch10x16,is,notactive],P,Path).
mem(switch18x15,active,0) .
iss(switch18x15,active,0,P).