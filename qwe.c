#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "Graph.h"
#include "Quack.h"


#define MAX 1024
void dfsQuack(Graph g, Vertex v, int numV);

struct graphRep { 
    int nV;       // #vertices 
    int nE;       // #edges 
    int **edges;  // matrix of Booleans ... THIS IS THE ADJACENCY MATRIX
};

bool differByOne(char *p, char *q){
	int sim=0;  //sim is to judge True or False
	int count = 0; //the number of diffdrent alphabet
	if (strlen(p) == strlen(q)){
		
		for(int u = 0; u < strlen(p); u++){	
			if (p[u] != q[u]) {
				count +=1;			
			}	
		}
		
		if (count ==1){
			sim = 0;
		}
		else{
			sim = 1;
		}		
	}
	else if (strlen(q) - strlen(p)==1){
		
		for(int i=0; i<strlen(q); i++){	
			if(q[i]!=p[i]){
				for(int d=i+1;d<strlen(q);d++){	
					if(q[d]!=p[d-1]){
						sim = 1;
					}
				}	
			}
		}
	}
	else if (strlen(p) - strlen(q)==1){
		
		for(int j=0; j<strlen(p); j++){	
			if(p[j]!=q[j]){
				for(j=j+1;j<strlen(q);j++){	
					if(p[j]!=q[j-1]){
						sim = 1;
					}
				}			
			}
		}
	}
	else if (strlen(q) - strlen(p)>=2){
		sim = 1;
	}
	else if (strlen(p) - strlen(q)>=2){	
		sim = 1;
	}	
	return sim;
}
struct word_attribution{
	int current[1000]; //current list like[-1,-1,-1,...,-1,-1]
	int max_len; 
	int solutions[100][1000];//to store each situation of langest current
	int count_s; 
	int index_c;
};

typedef struct word_attribution *wl;

wl creste_wl(void){
	wl word_list;
	word_list=malloc(sizeof(struct word_attribution));
	for(int i=0;i<1000;i++){
		word_list->current[i]=-1;
	}
	memset(word_list->solutions,-1,100*1000);
	word_list->max_len=0;
	word_list->count_s=0;
	word_list->index_c=0;
	return word_list;
}

bool if_edge(wl w,Graph g,int vertex){// a judge function to judge whether there is an edge
	for(int i=vertex;i<g->nV;i++){
		if (g->edges[vertex][i]==1){
			return true;
		}
	}
	return false;
}

void find_longest(int start,Graph g, wl w){// the function of finding the longest road and store the words
	if(w->current[0]==-1){
		exit(0);
	}
	if(if_edge(w,g,start)){
		
		for (int i=start;i<g->nV;i++){
			if(g->edges[start][i]==1){
				w->current[w->index_c]=i;
				w->index_c++;
				find_longest(i,g,w);
			}
		}
		w->index_c--;
		w->current[w->index_c]=-1;		
			
	}else{
		if(w->max_len<w->index_c){
		
		w->max_len=w->index_c;
		memset(w->solutions,-1,100*1000);
		w->count_s=0;
		for(int i=0;i<w->index_c;i++){
			
			w->solutions[0][i]=w->current[i];	
		}
		w->count_s++;
		w->index_c--;
		w->current[w->index_c]=-1;
		}else if(w->max_len==w->index_c){
			
			for(int i=0;i<w->index_c;i++){
				w->solutions[w->count_s][i]=w->current[i];
			}			
			w->count_s++;
			w->index_c--;
			w->current[w->index_c]=-1;
		}else{
			
			w->index_c--;
			w->current[w->index_c]=-1;
		}
		
	
	}
}

int main(int argc, char * argv[]){
	char word[MAX][24];// a two-dimensional array to store words
	char word_final[MAX][24];// do some change about words
	int index=0; // transition variable
	int num = 0; // the row number of word_final[][]
	while(scanf("%s",word[num])==1){
		num++;	
	}
	printf("Dictionary\n");	
	for(int a =1; a<num;a++){
		for (int b=0; b<a ;b++){
			if(strcmp(word[a],word[b])==0){
				strcpy(word[a]," ");	
			}			
		}
	}	
	index=num;
	int count=0;
	for (int i=0;i<index;i++){
		if(!strcmp(word[i]," ")){
			num--;		
		}else{
			strcpy(word_final[count],word[i]);
			count++;
		}	
	}	
	for (int i=0; i<num; i++){// print the rusult of step 1
		
		printf("%d: %s\n", i, word_final[i]);		
	}
	printf("Ordered Word Ladder Graph\n");
	Graph graph = newGraph(num);
	for(int t=0; t < num; t++){
		for(int u=t+1; u < num; u++){
			int s = differByOne(word_final[t],word_final[u]);
			if(s==0){
				Edge edge = newEdge(t, u);
				insertEdge(edge, graph);				
			}
		}
	}

	showGraph(graph);//print the result of step 2
	wl word_l=creste_wl();
	for(int i=0;i<num;i++){// find the longest road
		word_l->current[0]=i;
		word_l->index_c=1;
		
		find_longest(i,graph,word_l);
		
	}
	
	
	printf("Longest ladder length: %d\n", word_l->max_len);
	printf("Longest ladders:\n");
	for(int i=0;i<word_l->count_s;i++){//print the result of step 3
		printf("%2d: ", i+1);
		for(int j=0;j<word_l->max_len;j++){
			
			printf("%s ", word_final[word_l->solutions[i][j]]);
			if(j==word_l->max_len-1){
				break;
			}
			printf("-> ");
			
		}
		printf("\n");
	}
	
	freeGraph(graph);
	
    	
	
}





















	
