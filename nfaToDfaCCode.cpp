#include<stdio.h>
#include<string.h>



// structure pour stocker l'automate
typedef struct trns
{
        char from[100];
        char to[100];
        char alpha[100];
}transition;
transition transitions[50], transition2[50];

int Ntransitions = 0;
// structure pour stocker les états initials ,final,alphabets
typedef struct name
{
        char name[30];
}etat;
etat states[20],initialStates[20],finalStates[20], alphabets[50],noveau_etat[50],n_etat_final[20],etatfinal[20];
int Nstates = 0, NinitialStates = 0, NfinalStates = 0, Nalphabets = 0;
char mot[50],help[100];
int ta=strlen(mot),ss;
char *MOT;
// pour stocker l'ensemble des etats et le nombre des etats
typedef struct table_etat
{
        etat states[100];
        int N;
}table_etat;
table_etat DFAInitialState;

// stocker les transitions de DFA
typedef struct nt
{
      table_etat from;
      table_etat to;
      char alpha[100];
}nouveauTranstion;
 nouveauTranstion DFATranstions[100];
int N_DFATranstions = 0;

int tester_mot();
void verification(	int i);

 void verification_txt(	int i);
 int teste_mot_txt();

FILE *fic=fopen("graphviz.DOT","w");
int isFinal(table_etat s);// foction pour tester si une etat une etat final

void tapper_etat(table_etat s)
{
    
     for(int i=0; i<s.N; i++)
     {
             fprintf(fic,"%s", s.states[i].name);
             if(i!=s.N-1 )
             {
             	fprintf(fic,",");
			 }
     }
      
}


int etatexiste(char noveauetat[])
{
    for(int i=0; i<Nstates; i++)
    {
            if(strcmp(states[i].name, noveauetat) == 0)
            {
               return 0;
            }
    }
    return 1;
}

void stocker_NFA()
{
    char help[256];
    FILE *InputFile;
	InputFile = fopen("input.txt", "r");
	// les etats ini
	fscanf(InputFile, "%s", help);


	char *token = strtok(help, ",");
	while(token != NULL)
	{
                strcpy(initialStates[NinitialStates].name, token);
                NinitialStates++;
                token = strtok(NULL, ",");
    }
    // les etats final
    fscanf(InputFile, "%s", help);
    
    
	token = strtok(help, ",");
	while(token != NULL)
	{
                strcpy(finalStates[NfinalStates].name, token);
                NfinalStates++;
                token = strtok(NULL, ",");
    }
    // les transition.
	while(!feof(InputFile))
	{
        fscanf(InputFile, "%s %s %s",transitions[Ntransitions].from, transitions[Ntransitions].to, transitions[Ntransitions].alpha);
        Ntransitions++;
    }
    fclose(InputFile);
    // les etats.
    for(int i=0; i<Ntransitions; i++)
    {
            if(etatexiste(transitions[i].from))
            {
                strcpy(states[Nstates].name, transitions[i].from);
                Nstates++;
            }
            if(etatexiste(transitions[i].to))
            {
                strcpy(states[Nstates].name, transitions[i].to);
                Nstates++;
            }
    }
    //alphabets.
    for(int i=0; i<Ntransitions; i++)
    {
            int b=1;
            for(int j=0; j<Nalphabets; j++)
            {
                    if(strcmp(transitions[i].alpha,alphabets[j].name) == 0)
                    {
                       b = 0;
                    }
            }
            if(b==1)
            {
                    strcpy(alphabets[Nalphabets].name, transitions[i].alpha);
                    Nalphabets++;
            }
            
    }
    
    
}



int areEquals(table_etat s1, table_etat s2)
{
    if(s1.N != s2.N)
    {
            return 0;
    }
    else
    {
        for(int i1 = 0; i1<s1.N; i1++)
        {
            int b=0;
            for(int i2 = 0; i2<s2.N; i2++)
            {
                    if( strcmp(s1.states[i1].name, s2.states[i2].name) == 0)
                    {
                        b = 1;
                    }
            } 
            if(b==0)
            {
                    return 0;
            }   
        }
    }
    return 1;
}



void toDFA(table_etat fromState)
{
     for(int al=0; al<Nalphabets; al++)
     {
             table_etat toState;
             toState.N=0;
             for(int fs=0; fs<fromState.N; fs++)
             {
                     
                         for(int tr=0; tr<Ntransitions; tr++)
                         {
                                 if( strcmp(transitions[tr].from, fromState.states[fs].name)==0 && strcmp(alphabets[al].name, transitions[tr].alpha)==0)
                                 {
                                     // tester si l'etats deja existe
                                     int boo = 1;
                                     for(int g1=0; g1<toState.N; g1++)
                                     {
                                             if(strcmp(toState.states[g1].name,transitions[tr].to)==0)
                                             {
                                                boo = 0;
                                             }
                                     }
                                     if(boo==1)
                                     {                            
                                                                  strcpy(toState.states[toState.N].name, transitions[tr].to);
                                                                  toState.N++;
                                     }
                                 }
                                 
             
                         }
                     
                     
                     
             }
             if(toState.N != 0)
             {
                          
                         
                          DFATranstions[N_DFATranstions].from.N = fromState.N;
                          DFATranstions[N_DFATranstions].from = fromState;
                          DFATranstions[N_DFATranstions].to.N = toState.N;
                          DFATranstions[N_DFATranstions].to = toState;
                          strcpy(DFATranstions[N_DFATranstions].alpha, alphabets[al].name);
                          N_DFATranstions++;
                    
                          
                          // tester si le nouveau etat existes
                          
                          int b=1;
                          for(int i=0; i<N_DFATranstions; i++)
                          {
                                  if(areEquals(DFATranstions[i].from,toState)==1)
                                  {
                                     b=0;
                                  }
                                  
                          }
                          if(b == 1)
                          {
                               toDFA(toState);
                          }
             }
     }
}

void generer()// fonction pour générer le fichier 
{ 
int i=0,j=0;

fprintf(fic," digraph{ \n node [shape = point ];x;\n");

for(i=0;i<NfinalStates;i++){
fprintf(fic," \n node [shape=doublecircle];%s;\n",finalStates[i]);	
}




fprintf(fic," node [shape=circle];\n");


fprintf(fic," x-> %s [label=\"%s\" ];\n ",transitions[0].from);


for ( i=0; i<N_DFATranstions;i++){
fprintf(fic," %s-> %s [label=\"%s\" ]; \n",transitions[i].from,transitions[i].to,transitions[i].alpha);

}


fprintf(fic,"} ");
	
}
int isFinal(table_etat s)
{
    int b=0;
    for(int i=0; i<s.N; i++)
    {
            for(int j=0; j<NfinalStates; j++)
            {
                    if(strcmp(finalStates[j].name, s.states[i].name)==0)
                    {
                                                     b=1;
                    }
            }
            
    }
    return b;
}
int isfinal(char s[100])
{
    int b=0;
  
    
            for(int j=0; j<NfinalStates; j++)
            {
                    if(strcmp(finalStates[j].name, s)==0)
                    {
                                                     b=1;
                    }
            }
            
    
    return b;
}
void changer(){
	  int i,j,k,c,l,s=0,h,x=1,b,y,z,a;
	  
	  
	 
	  for(i=0;i<50;i++)
	  {
	  	
	  	sprintf(noveau_etat[i].name,"q%d",i);
	  }
	  
	  strcpy(transitions[0].from,noveau_etat[0].name);
	  for(k=0;k<N_DFATranstions;k++){	
				if(areEquals(DFATranstions[0].from,DFATranstions[k].to)==1){ 
			  	              
							 // strcpy(transition2[i].from,noveau_etat[0].name);
			                  strcpy(transitions[k].to,noveau_etat[0].name);
			               }
		                 }
	  
	  
	  
	
	//
	for (j=0;j<N_DFATranstions;j++){
		strcpy(transitions[j].alpha,DFATranstions[j].alpha);
		
		}
		
		
	
     	for(i=1;i<N_DFATranstions;i++){
   
		
			
			for(h=0;h<i;h++){
				 c=0;
				if(areEquals(DFATranstions[h].from,DFATranstions[i].from)==1){
				
					 c=1;
					 l=h;
					 break;
					 
					
				} 
			}
			if(c==1){
				
				strcpy(transitions[i].from,transitions[l].from);
			}
				
					
				else {
					strcpy(transitions[i].from,noveau_etat[x].name);
					x++;
					 for(k=0;k<N_DFATranstions;k++){	
				if(areEquals(DFATranstions[i].from,DFATranstions[k].to)==1){ 
			  	              
							 // strcpy(transition2[i].from,noveau_etat[0].name);
			                  strcpy(transitions[k].to,transitions[i].from);
			               }
		                 }
					
				
					}
				}	
				
					for(i=1;i<N_DFATranstions;i++){
   
		
			
			for(h=0;h<N_DFATranstions;h++){
				 c=0;
				if(areEquals(DFATranstions[h].from,DFATranstions[i].to)==1){
				
					 c=1;
					 l=h;
					 break;
					 
					
				} 
			}
			if(c==0){
					for(a=0;a<i;a++){
				 c=0;
				if(areEquals(DFATranstions[a].to,DFATranstions[i].to)==1){
				
					 c=1;
					 l=a;
					 break;
					 
					
				} 
			}
			if(c==1){
				
				strcpy(transitions[i].to,transitions[l].to);
			}
				
					
				else {
					strcpy(transitions[i].to,noveau_etat[x].name);
					x++;
					 	
		
		                 }
					
				
					}
				
			}
					
		
					
					
				
					
					
	NinitialStates = 1;
	strcpy(initialStates[0].name,transitions[0].from );
	j=0;
	for(i=0;i<N_DFATranstions;i++)
	{
		if(isFinal(DFATranstions[i].from)==1)
		{
				b=0;
		 for(h=0;h<j;h++){
		 
		 	if(strcmp(transitions[i].from,etatfinal[h].name)==0){
		 		b=1;
			 }
		 	
		 }
		 if(b==0){
		 	strcpy(etatfinal[j].name,transitions[i].from );
		j++;
	
		}
	}
}
	for(i=0;i<N_DFATranstions;i++)
	{
		if(isFinal(DFATranstions[i].to)==1)
		{
				b=0;
		 for(h=0;h<j;h++){
		 
		 	if(strcmp(transitions[i].to,etatfinal[h].name)==0){
		 		b=1;
			 }
		 	
		 }
		 if(b==0){
		 	strcpy(etatfinal[j].name,transitions[i].to);
		j++;
	
		}
	}
}
	
	/*for(i=0;i<N_DFATranstions;i++){
		if(isFinal(DFATranstions[i].to)==1)
		{
		strcpy(etatfinal[j].name,transitions[i].to );
		j++;
		}	
		
	}*/

	NfinalStates =j;
	for(i=0;i<NfinalStates;i++){
	strcpy(finalStates[i].name,etatfinal[i].name);	
	}
}
void renverser(){
		int i,j,k;
	char aide[100];
	 Ntransitions=N_DFATranstions;
	for(i=0;i<N_DFATranstions;i++){
		 strcpy(aide, transitions[i].to);
		 strcpy(transitions[i].to, transitions[i].from);
		 strcpy( transitions[i].from,aide);
		 
		
	}
	NinitialStates=NfinalStates;
	NfinalStates=1;
	for(i=0;i<NinitialStates;i++){
		strcpy(initialStates[i].name ,finalStates[i].name);
	}
	strcpy(finalStates[0].name, transitions[0].to);
	
}

/* void tap( table_etat S){
	for( int h=0;h<S.N;h++){
			printf(" %s ",S.states[h].name);
			
		  }
		
	}
*/ int teste_mot_txt(){

 	int i;
 
 	 	

       int c=0;
       for(i=0;i<N_DFATranstions;i++){
       	
       	if(strcmp(transitions[i].from,initialStates[0].name)==0 && transitions[i].alpha[0]==mot[0] )
       	{
       		
       		strcpy(help,transitions[i].to);
       		c=1;
       		break;
       		
		   }
		}
		   
		   if(c==0)
		   {
		   return 11;
		   }
		   else{
		   	
		   	if(ta==1 && isfinal(help)==1){
		   		
		   		return 1;
			   }
			   else{
			   	
			   
			   	return 2;
			   }
		   	
		   }
		   
	
       
       
 }
 void verification_txt(	int i){
 	int j,k=0,c=0,l;
      if(ta==(i+1))
      {
      	if(isfinal(help)==1){
      		printf("%s\n",mot);
      		
		  }
		 
		  
	  }
	  else{
	  
 		for(j=0;j<N_DFATranstions;j++){
 			
 		if(strcmp(transitions[j].from,help)==0 && transitions[j].alpha[0]==mot[i+1]){
 			k=1;
 			strcpy( help,transitions[j].to);
 			
 	
		 
 		verification_txt(i+1);
 	
		  		
		 break;	
		 }
		 
		 
	}
	 	
   
 	}	
 		
	 
 	
 }

 int tester_mot(){

 	int i;
 
 	 	
 	 	printf("\n entrez le mot :  ");
       scanf("%s",mot);
      
       ta=strlen(mot);
       int c=0;
       for(i=0;i<N_DFATranstions;i++){
       	
       	if(strcmp(transitions[i].from,initialStates[0].name)==0 && transitions[i].alpha[0]==mot[0] )
       	{
       		
       		strcpy(help,transitions[i].to);
       		c=1;
       		break;
       		
		   }
		}
		   
		   if(c==0)
		   {
		   return 11;
		   }
		   else{
		   	
		   	if(ta==1 && isfinal(help)==1){
		   		
		   		//return 1;
			   }
			   else{
			   	
			   
			   	return 2;
			   }
		   	
		   }
		   
	
       
       
 }
 
 void verification(	int i){
 	int j,k=0,c=0,l;
      if(ta==(i+1))
      {
      	if(isfinal(help)==1){
      		printf(" mot engendrer par l'automate\n ");
      		
		  }
		  else {
		  	printf(" %s n'est pas un etat d'acceptation\n",help);
		  }
		  
	  }
	  else{
	  
 		for(j=0;j<N_DFATranstions;j++){
 			
 		if(strcmp(transitions[j].from,help)==0 && transitions[j].alpha[0]==mot[i+1]){
 			k=1;
 			strcpy( help,transitions[j].to);
 				printf(" %s,",help);
 		for( j=i+1;j<ta;j++){
 			printf(" %c",mot[j]);	
		 }
		 	printf(" )\n");
 		verification(i+1);
 	
		  		
		 break;	
		 }
		 
		 
	}
	 	
     if(k==0) {
     	printf(" %s,",help);
 		for( j=i+1;j<ta;j++){
 			printf(" %c",mot[j]);	
		 }
		 	printf(" )\n");
     	printf(" l'automate est coincé \n");
     	
	 }
 	}	
 		
	 
 	
 }
 
 mot_existe( ){



  int f=teste_mot_txt();
      
	  if(f==11 ){
	  	
	  }
	  
	   else if  (f==1){
	printf("%s\n",mot);
	  }
	 else { 
	 
	  		verification_txt(0);
	   }
 
	
 	
 	
 }
 
 
 
afficher_mots_engendr(){
 	FILE *InputFil=NULL;
char ligne[50];
char* ll;

//printf(" %d",ta);
	InputFil = fopen("fic.txt", "r");

	
		while(fscanf(InputFil,"%s",mot )!=EOF){
		


		  ta=strlen(mot);  
 
    
           mot_existe();
                
                
  
		
		
		}
 	
 }     


main()
{
	
      stocker_NFA();
      
      // creer l'etat initial de DFA
      DFAInitialState.N=0;
      for(int i=0; i<NinitialStates; i++)
      {
              
           strcpy(DFAInitialState.states[DFAInitialState.N].name , initialStates[i].name);
           DFAInitialState.N++;
      }
      toDFA(DFAInitialState);
      
     
      changer();
      
     
    
     
       

      
      renverser();

       N_DFATranstions=0;
       DFAInitialState.N=0;
       
      for(int i=0; i<NinitialStates; i++)
      {
              
           strcpy(DFAInitialState.states[DFAInitialState.N].name , initialStates[i].name);
           DFAInitialState.N++;
      }
      toDFA(DFAInitialState);
      
      
      changer();
      

       
       for(int i=0;i<N_DFATranstions;i++){
      	printf("\n  %s --> %s --> %s \n",transitions[i].from,transitions[i].to,transitions[i].alpha);
	  }
     
      
      renverser();
       N_DFATranstions=0;
        DFAInitialState.N=0;
      for(int i=0; i<NinitialStates; i++)
      {
              
           strcpy(DFAInitialState.states[DFAInitialState.N].name , initialStates[i].name);
           DFAInitialState.N++;
      }
      toDFA(DFAInitialState);
      changer();
      
      int test=0;
       generer();
      
      
       
       

       int f=tester_mot();
       if (f==11){
       	printf("mot n'appartient pas ");
	   }
	   else if (f==1){
	  	printf(" mot engendre par l'automate");
	  	printf(" (%s,%s)\n",help,mot);
	  }
	 else { 
	 printf(" (%s,%s)\n",help,mot);
	  		verification(0);
	   }
	   printf("les mots engendres par l'automate sont:\n ");
	afficher_mots_engendr();
       
   
        
    
	   }
	  
	  
	 
	  
	  
      
 
       
