from MOM import *
if os.path.exists("MOM_pl.pl"): os.remove("MOM_pl.pl")
ResetKnowledge()
while 1!=0:
	txt=raw_input("you: ")
	if txt=="KNOWLEDGE" or txt=="SAVE":
		toWrite=("def ReplaceRequest(R): exec 'def Request(s,arg=\"N0\"): return R(s,arg)' in locals(),globals()\n#Representations (type,transform), as what should words of type get transformed to? to variables, words?\nRepres="+str(Repres)+
		"\n# Logic Operators (words,syntax,type)\nOps="+str(Ops))+("\n# Word Types (word_regex,Type)\nTypes="+str(Types)+
		"\n#Reasons - which predicates are relevant as reasons to the why question?\nReasons="+str(Reasons)+"\n#Sentence Structures [WordTypes,"+
		"Meaning,Description]\n"+"#in sentences the left part of A in Meaning is for assumptions, else its just a &, and the part after $ is a python cmd(T0 for now(time)):\nRules="+str(Rules)+
		"\n#FOL axioms MOM system doesn't include \nAxioms=\"\"\""+Axioms+"\"\"\"\n#Procedural Knowledge\nCustomCode=\"\"\""+CustomCode+"\"\"\"\nexec CustomCode\n#Language Knowledge\nSentences="+str(SInput).replace(", '",",\n'")).replace(", (",",\n(").replace(", [",",\n[")
		if txt=="SAVE":
			with open("MOM_def.py", "w") as def_file: def_file.write(toWrite)
		else: print toWrite
	else:
		print str(PrettyTell(txt))
