#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#define SPEC 2


typedef struct vectordist {

    long dist;
    char color;

}vectordist_t;
typedef struct result {

    long sum;
    int graphindex;

} result_t;
typedef struct max{
    long max;
    int indexmax;


}max_t;
void maxheapify(result_t* res,long* spec,int i);
void maxHeapvector(result_t* res,long* spec);
void stampa(int index);
long fasteratol(char* num);
void aggiungigrafo(long** graph,result_t* res,long* spec,vectordist_t* vect,int graphindex,char* inbuff,max_t* maxmin);
void sumshortpath(long** graph,result_t* res,long* spec,vectordist_t* vect,int graphindex,max_t* maxmin);



int main(){
    int i,graphind,j,flag,m;
    char specbuffer[32],temp[11];

    long spec[SPEC];
    vectordist_t* vect;
    long** G;
    char* inbuffer;
    result_t* res;
    max_t puntmaxmin;
    j=0;
    m=0;
    puntmaxmin.max=0;
/*
    do{

        specbuffer[j]=getchar_unlocked();
        j++;
    }while(specbuffer[j-1]!='\n');
    specbuffer[j-1]='\0';
    j=0;*/
    if(fgets(specbuffer,32,stdin)!=NULL) {
        while (specbuffer[m] != '\n') {
            if (specbuffer[m] == ' ') {
                temp[j] = '\0';
                m++;
                spec[0] = fasteratol(temp);
                j = 0;

            } else {
                temp[j] = specbuffer[m];
                j++;
                m++;
            }
        }
        temp[j] = '\0';
        spec[1] = fasteratol(temp);
    }

    inbuffer=(char*)malloc(10*spec[0]*sizeof(char)+(spec[0]-1)*sizeof(char)+2);
    res=(result_t*) malloc(spec[1]*sizeof(result_t));
    G = (long**) malloc(spec[0]*sizeof(long*));
    vect=(vectordist_t*) malloc(spec[0]*sizeof(vectordist_t));
    graphind=0;
    flag=0;



    for(i=0;i<spec[0]; i++){
        G[i]= (long*) malloc(spec[0]*sizeof(long));
    }


    while(flag==0 && fgets(inbuffer,10*spec[0]*spec[0],stdin)!=NULL){

        if ((inbuffer[0] == 'a') || (inbuffer[0] == 'A')) {
            aggiungigrafo(G, res, spec, vect,graphind,inbuffer,&puntmaxmin);
            graphind++;

        }
        if ((inbuffer[0] == 't') || (inbuffer[0] == 'T')) {

            if(graphind==0){
                printf("\n");
            }
            else{
                for (j = 0; j < graphind && j < spec[1]; j++) {

                    stampa(res[j].graphindex);
                    if(j==graphind-1 || j==spec[1]-1){
                        putchar('\n');
                    }
                    else{
                        putchar(' ');
                    }

                    //printf("%d%s", res[j].graphindex,j==graphind-1 || j==spec[1]-1?"\n":" ");
                }
            }

        }
        if(inbuffer[0]=='\n'){
            flag=1;
        }
    }

    for(i=0;i<spec[0];i++){

        free(G[i]);
    }
    free(G);
    free(res);
    free(inbuffer);
    free(vect);
    return 0;

}

void maxheapify(result_t* res,long* spec,int i){
    int largest,leftnode,rightnode;
    result_t temp;
    largest=i;
    leftnode=2*i;
    rightnode=2*i+1;

    if (leftnode<spec[1] && res[leftnode].sum>res[largest].sum)
        largest=leftnode;

    if (rightnode<spec[1] && res[rightnode].sum>res[largest].sum)
        largest = rightnode;

    if (largest!=i){
        temp.sum=res[i].sum;
        temp.graphindex=res[i].graphindex;
        res[i].sum=res[largest].sum;
        res[i].graphindex=res[largest].graphindex;
        res[largest].sum=temp.sum;
        res[largest].graphindex=temp.graphindex;
        maxheapify(res,spec,largest);
    }
}


void maxHeapvector(result_t* res,long* spec){
    int start,i;

    start=(spec[1]/2)-1;
    for (i=start;i>=0;i--) {
        maxheapify(res,spec,i);
    }
}
void stampa(int index){

    if (index/10 != 0) {
        stampa(index/ 10);
    }

    putchar(index%10 + '0');
}
long fasteratol(char* num){
    long conv=0;
    int i=0;
    while(num[i]!='\0'){
        conv=conv*10+(num[i]-'0');
        i++;
    }

    return conv;

}
void aggiungigrafo(long** graph,result_t* res,long* spec,vectordist_t* vect,int graphindex,char* inbuff,max_t* maxmin){
    int i,k,j,m;
    char temp[11];
    i=0;
    m=0;


    while(i<spec[0] && fgets(inbuff,10*spec[0]*sizeof(char)+(spec[0]-1)*sizeof(char)+2,stdin)!=NULL) {
        k=0;
        j=0;

        while(inbuff[m]!='\0'){
            if(inbuff[m]==','){
                temp[j]='\0';
                m++;
                graph[i][k]= fasteratol(temp);
                j=0;
                k++;
            }
            else{
                if(inbuff[m]=='\n'){
                    temp[j]='\0';
                    m++;
                }
                else{
                    temp[j]=inbuff[m];
                    j++;
                    m++;
                }
            }
        }
        temp[j]='\0';
        graph[i][k]=fasteratol(temp);
        m=0;
        i++;
    }


    sumshortpath(graph,res,spec,vect,graphindex,maxmin);
}

void sumshortpath(long** graph,result_t* res,long* spec,vectordist_t* vect,int graphindex,max_t* maxmin) {

    int curr, i, j, flag;
    long tempdist;
    result_t min;
    result_t compare;
    min.sum = 0;
    min.graphindex=0;
    curr = 0;
    vect[0].dist = 0;
    vect[0].color = 'w';
    compare.graphindex = graphindex;
    compare.sum = 0;


    for (i = 1; i < spec[0]; i++) {
        vect[i].dist = -1;
        vect[i].color = 'w';
    }                   //set distanza=inf e col=white

    for (i = 1; i < spec[0]; i++) {
        vect[curr].color = 'b';
        for (j = 0; j < spec[0]; j++) {
            if (j != curr) {
                tempdist = vect[curr].dist + graph[curr][j];
                if (graph[curr][j] != 0 && (tempdist < vect[j].dist || vect[j].dist == -1)) {
                    vect[j].dist = vect[curr].dist + graph[curr][j];

                }
                if (vect[j].dist != -1 && vect[j].color != 'b') {
                    if (min.sum == 0 || vect[j].dist < min.sum) {
                        min.sum = vect[j].dist;
                        min.graphindex = j;
                    }
                }           //ric minimo
            }

        }
        curr = min.graphindex;
        min.sum = 0;
    }                   //ricerca percorsi minimi

    for (i = 0; i < spec[0]; i++) {
        if(vect[i].dist!=-1){
        compare.sum = compare.sum + vect[i].dist;
        }
    }
    flag = 0;
    if(graphindex==0){
        maxmin->max=compare.sum;
        maxmin->indexmax=0;
        flag=1;
    }

    if(compare.graphindex<spec[1]){
        res[compare.graphindex].sum=compare.sum;
        res[compare.graphindex].graphindex=compare.graphindex;
        flag=1;
        /*
        if(compare.sum>maxmin->max){
            maxmin->max=res[compare.graphindex].sum;
            maxmin->indexmax=compare.graphindex;
        }*/
    }
    if(compare.graphindex==spec[1]){
        maxHeapvector(res,spec);
    }
    if(compare.sum<res[0].sum && flag==0){
        res[0].sum=compare.sum;
        res[0].graphindex=compare.graphindex;
        maxheapify(res,spec,0);
    }
    /*if(compare.sum<maxmin->max && flag==0 ){
        res[maxmin->indexmax].sum=compare.sum;
        res[maxmin->indexmax].graphindex=compare.graphindex;
        maxmin->max=0;
        for(i=0;i<spec[1];i++){
            if(res[i].sum>maxmin->max){
                maxmin->max=res[i].sum;
                maxmin->indexmax=i;
            }
        }
    }*/

}