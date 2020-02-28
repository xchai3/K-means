//
//  Kmain.c
//  read3
//This assignment mainly implement K-means. For the detail about function please check the structure files.
//  Created by Xipeng Chai on 9/24/19.
//  Copyright © 2019 Xipeng Chai. All rights reserved.
//

#include <stdio.h>
#include  <stdlib.h>
#include "structure.h"
/************* Variable  and data structure Declarations  ***************/
int isContinue;
int numberofpoints;
int dim;
coord *data;
SumAll *SUM;
ClusterCenter *cluster_center;
double* distanceFromCenter;
char *filename;
char *filename2;

int main(int argc, char* argv[])
{
    /*************check if the user entry right number of parameter*************/
    if( argc != 4 )
     {
       printf("This application need 3 parameter to run:"
           "\n\t\tthe first name of input file,"
           "\n\t\tthe second is the value of K"
           "\n\t\tthe third the name of outputfile"
           "\n");
       exit(0);
     }
    filename=argv[1];
    int K=atoi(argv[2]);
    filename2=argv[3];
    /**********get the total number of data*****************/
    
    numberofpoints=Readfirstline(filename)[0];
    
    /************get the number of attributes*********/
    
    dim=Readfirstline(filename)[1];
    
    /*************initialize *********************************/
    
    data=initial_data2(numberofpoints);
    cluster_center=initial_clustermemory(K);
    distanceFromCenter=initialD(K);
    SUM=initialSum(K);
    
    /**************read every data****************/
    Readfile(numberofpoints,dim,data,filename);
    initial_cluster(K, dim, data, cluster_center);
    
    /**********start K means****************/
    K_means(K, dim, data, cluster_center, distanceFromCenter, numberofpoints, SUM,filename2);
    return(0);
    }
