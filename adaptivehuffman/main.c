#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bitio.h"
#include "errhand.h"
#include <stdlib.h>

typedef struct tree
{  int value;
   unsigned int weight;
   int order;
  int child_0;
  int child_1;
  int parent;
}NODE;

typedef struct code
{unsigned int code;
 int bits;
}CODE;
void createcode(NODE *nodes,CODE *codes, unsigned alreadybits, int bitcounts, int root)
{if(nodes[root].value==-2)
  return;
 if(nodes[root].value>=0)
 {codes[nodes[root].value].code=alreadybits;
  codes[nodes[root].value] .bits=bitcounts;
  //printf("\ncode[%d].code is %d with the length %d",root,alreadybits,bitcounts);
  return;
 }
   bitcounts++;
  alreadybits<<=1;
 createcode(nodes,codes,alreadybits,bitcounts,nodes[root].child_0);
 //alreadybits++;
  createcode(nodes,codes,alreadybits|1,bitcounts,nodes[root].child_1);
  //printf("\nthe bitcounts\n%d",bitcounts);
 }





 Updatetree(NODE *nodes,int firstorder)
 {
     int i;

  i=firstorder;
     while(i<512)
    {(nodes[i].weight)++;
     i=nodes[i].parent;}
 }

  Switchnode(NODE*nodes,int order1,int order2)
  {int temp_value,temp_child_0,temp_child_1;
    temp_value=nodes[order1].value;
   temp_child_0=nodes[order1].child_0;
   temp_child_1=nodes[order1].child_1;

   nodes[nodes[order1].child_0].parent=nodes[order2].order;
    nodes[nodes[order1].child_1].parent=nodes[order2].order;

    nodes[nodes[order2].child_0].parent=nodes[order1].order;
    nodes[nodes[order2].child_1].parent=nodes[order1].order;


   nodes[order1].value=nodes[order2].value;
   nodes[order1].child_0=nodes[order2].child_0;
   nodes[order1].child_1=nodes[order2].child_1;

   nodes[order2].value=temp_value;
   nodes[order2].child_0=temp_child_0;
   nodes[order2].child_1=temp_child_1;



  }


 int testfirst(NODE *nodes,int order)
 {int testorder;

     if(order==512)  {return(512);}
     for(testorder=order;testorder<512;testorder++)
  {if(nodes[testorder].weight>nodes[order].weight)
   {break;}

  }
  if(nodes[order].parent==testorder-1){return order;}
  else return(testorder-1);

 }


 IfSwitch(NODE *nodes,int order )
{ int firstorder,parentfirstorder,parentorder;

  firstorder=testfirst(nodes,order);
  if(firstorder!=order)
  Switchnode(nodes,firstorder,order);
  parentorder=nodes[firstorder].parent;
  while(parentorder<512)
  {parentfirstorder=testfirst(nodes,parentorder);
   if(parentfirstorder!=parentorder)
   Switchnode(nodes,parentfirstorder,parentorder);
   parentorder=nodes[parentfirstorder].parent;
  }
  Updatetree(nodes,firstorder);

}


Testvalue(NODE*nodes,int c,int noworder)
{int i;

//printf("the noworder is %d\n",noworder);
 for(i=0;i<=512;i++)
   if (c==nodes[i].value)
  {IfSwitch(nodes,nodes[i].order);
   return(noworder);}

  nodes[noworder].value=-1;
  nodes[noworder].order=noworder;//can delete
  nodes[noworder].weight=0;
  nodes[noworder].child_0=noworder-2;
  nodes[noworder].child_1=noworder-1;


  nodes[noworder-1].value=c;
  nodes[noworder-1].order=noworder-1;//can delete??
  nodes[noworder-1].weight=1;
  nodes[noworder-1].child_0=0;//can delete
  nodes[noworder-1].child_1=0;//can delete
  nodes[noworder-1].parent=noworder;


  nodes[noworder-2].value=-2;//the new NYT
  nodes[noworder-2].order=noworder-2;//can delete
  nodes[noworder-2].weight=0;
  nodes[noworder-2].child_0=0;
  nodes[noworder-2].child_1=0;
  nodes[noworder-2].parent=noworder;

  noworder=noworder-2;


  IfSwitch(nodes,noworder+2);

   return(noworder);

 }















int initiatenode(nodes)
NODE *nodes;
{int i,max,j,temp;
 int type=0;
 for(i=0;i<=512;i++)
 {
  nodes[i].value=-1;//at first,-1 menas intermediate node
  nodes[i].order=0;
  nodes[i].weight=0;
  nodes[i].child_0=0;
  nodes[i].child_1=0;
  nodes[i].parent=0;
 }
 nodes[512].value=-2; //nodes[512] is the root, at first it is the NYT
}




















int CaculateEntropy(input,counts)
FILE* input;
unsigned int*counts;
{ int c;
 int size=0;
 int kindofsymbol=0;
 double Entropy=0;
 long record;
 record=ftell(input);
 while((c=getc(input))!=EOF)
 {size++;
 counts[c]++;
  //printf("\n%chello\n",c);
  }
   fseek(input,record,0);
   for(c=0;c<256;c++)
 {if(counts[c]!=0)
   {Entropy+=(double)(counts[c])*(log10((double)(size)/counts[c]))/log10(2.0)/(double)(size);
  kindofsymbol++;}
 }
  printf("\nenropy of the file is %lf\n",Entropy);
  printf("\nkind of the character is %d\n",kindofsymbol);

 return(size);
}

int main (int argc, char *argv[])
  {
    FILE *input;
    FILE *inputcopy;
    BIT_FILE *output;
      int size=0;
      int noworder=512;
     unsigned long int bitsize=0;
    int c;
    long record;
    unsigned int *counts;
    NODE *nodes;
    CODE *codes;
    int rootnode;
    int type;
    int *allorder;
    counts =(unsigned int *)calloc(256,sizeof(unsigned int));
    if(argc!=3)
    printf("not correct arguments setting");
    *allorder=256;


    if((input=fopen(argv[1],"rb"))==NULL) {printf("cannot open file for input\n");return 0; }

    output=OpenOutputBitFile(argv[2]);
    if(output==NULL) {  printf("cannot open file for output\n");  return(0); }
    size=CaculateEntropy(input,counts);
    printf("the size is\n%d",size);

    nodes=(NODE*)calloc(514,sizeof(NODE));

    codes=(CODE*)calloc(514,sizeof(CODE));

    initiatenode(nodes);



    record=ftell(input);
    type=0;
    while((c=getc(input))!=EOF)
    {

     noworder=Testvalue(nodes,c,noworder);
     createcode(nodes,codes,0,0,512);
     //if(type==18)
     //{printf("\input=%X\n",c);break;}
     //type++;
    //printf("\n her is the starting point\n");
     //printf("\input=%c,code=%X,codelength=%d\n",c,codes[c].code,codes[c].bits);
     //printf("\input=%c,code=%X,codelength=%d\n",c,codes[c].code,codes[c].bits);
     OutputBits(output,codes[c].code,codes[c].bits);
     bitsize=bitsize+codes[c].bits;
    }

     fseek(input,record,0);







    //printf("\nthe bits number of the output is %ld\n",bitsize/8);
    printf("\nthe bits number of the output is %lf\n",(double)(bitsize)/8);
printf("the compression ratio is %lf%%\n",100*(size*8-(double)(bitsize))/((double)(size)*8));
printf("the average bitsize is %lf\n",(double)(bitsize)/size);
CloseOutputBitFile(output);

    fclose(input);
  return 0;
}
