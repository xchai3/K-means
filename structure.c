//
//  strcture.c
//  read3
//  this file shows the detail information of every function
//  Created by Xipeng Chai on 9/24/19.
//  Copyright © 2019 Xipeng Chai. All rights reserved.
//
#include "structure.h"
#include <stdio.h>
#include <stdlib.h>
#define MAX 5000
#define round_time 300

/**********initial all data structures****************************/
coord* initial_data2(int numberofpoints){
    coord *data= malloc(sizeof(coord)*(numberofpoints+1));
       if( !data )
        {
          printf("malloc error:data!");
          exit(0);
        }
    return data;
}
ClusterCenter* initial_clustermemory(int K){
    ClusterCenter* cluster_center = (ClusterCenter*)malloc(sizeof(struct ClusterCenter) * (K+ 1));
        if( !cluster_center )
        {
          printf("malloc cluster center new error!\n");
          exit(0);
        }
    return cluster_center;
}
double* initialD(int K){
    double *distanceFromCenter = (double*)malloc(sizeof(double) * (K+1));
         if( !distanceFromCenter )
         {
           printf("malloc error: distanceFromCenter!\n");
           exit(0);
         }
    return distanceFromCenter;
}
SumAll * initialSum(int K){
    SumAll *SUM=malloc(sizeof(SumAll) * (K+1));
        if( !SUM )
        {
          printf("malloc cluster center new error!\n");
          exit(0);
        }
    return SUM;
}
/*****************finish initialing data structures************************/

/************set the first K points as first centroid ************************/
void initial_cluster(int K,int dim,coord*data,ClusterCenter* cluster_center){
    for(int i=1;i<=K;i++){
        for(int j=1;j<=dim;j++){
            cluster_center[i].clus[j]=data[i].test[j];
        }
        cluster_center[i].clusterID=i;
    }
}

/**************caluclate the distance between one point and  a centroid*/
void calulate_distance_to_oneCenter(int dataID,int clusterID,int dim,coord*data,ClusterCenter* cluster_center,double* distanceFromCenter){
    double sum=0;
    for(int j=1;j<=dim;j++){
        sum=sum+pow((double)(data[dataID].test[j]-cluster_center[clusterID].clus[j]),2);
    }
    distanceFromCenter[clusterID]=sqrt(sum);
   // return distanceFromCenter;
}

/********************calculate distance from one point to  every cluster center************/
void calulate_distance_to_allCenter(int dataID,int K,int dim,coord*data,ClusterCenter* cluster_center,double* distanceFromCenter){
    for(int i=1;i<=K;i++)
        calulate_distance_to_oneCenter(dataID,i,dim,data,cluster_center,distanceFromCenter);
}

/**************find the minimum distance and assign the point to that cluster**********/
void allocatecluster_one(int dataID,int K,double* distanceFromCenter,coord*data){
    double min=distanceFromCenter[1];
    int cluster=1;
    for(int i=2;i<=K;i++){
        if(distanceFromCenter[i]<min){
            min=distanceFromCenter[i];
            cluster=i;
        }
    }
    data[dataID].clusterID=cluster;
}
/*********allocate every points to their nearest cluster*********/
void allocatecluster_all(int K,int dim,coord*data,ClusterCenter* cluster_center,double* distanceFromCenter,int numberofpoints){
    for(int i=1;i<=numberofpoints;i++){
        calulate_distance_to_allCenter(i,K,dim,data,cluster_center,distanceFromCenter);
        allocatecluster_one(i,K,distanceFromCenter,data);
    }
}
/**************update the cluster’s centroid to the average of the points assigned to that cluster.*********** */
int re_calculate(int K,int dim,SumAll *SUM,int numberofpoints,coord*data,ClusterCenter* cluster_center){
    int* count; // calculate the number of data in every cluster
    count = (int*)malloc(sizeof(int) * (K + 1));
    if( !count )
     {
       printf("malloc error: count\n");
       exit(0);
     }
    for(int i=1;i<=K;i++){
        count[i]=0;
    
    for(int j=1;j<=dim;j++){
        SUM[i].average[j]=0;
    }
    }
    /****************add every coordinates in every cluster***************/
    for(int i=1;i<=numberofpoints;i++){
        count[data[i].clusterID]++;//get the number of data in every cluster
        
        for(int j=1;j<=dim;j++){
            SUM[data[i].clusterID].average[j]+=data[i].test[j];
        }
    }
/*********collect information of every cluster such as the coordinates of center, the number of points in this cluster********/
    for(int i=1;i<=K;i++){
        printf("No. cluster: %d\n",i);
        printf("number of data: %d\n",count[i]);
        printf("coordinate:");
        for(int j=1;j<=dim;j++){
        printf("%lf ",cluster_center[i].clus[j]);
        }
        printf("\n");
    }
    /********************calculate the average value*********************/
    for(int i=1;i<=K;i++){
        for(int j=1;j<=dim;j++){
            if(count[i]!=0){
            SUM[i].average[j]=(double)SUM[i].average[j]/(double)count[i];
            }
            else // if there is no data point in the cluster, no need to calculate average.
                continue;
        }
    }
    /*************/
    int  isContinue=iscontinue(K,dim,cluster_center,SUM);
    /*********************if the average of points is different from coordinates of centroid**********/
    
    if(isContinue==1){
    for(int i=1;i<=K;i++){
        for(int j=1;j<=dim;j++){
            cluster_center[i].clus[j]=SUM[i].average[j];
        }
        cluster_center[i].clusterID=i;
    }
    free(count);
    }
    return isContinue;
}
/****************this function is used to compare the new centroid with the old centroid********* */
int  iscontinue(int K,int dim,ClusterCenter* cluster_center,SumAll *SUM){
    int isContinue=0; //None of the cluster is changed.
    for(int i=1;i<=K;i++){
        for(int j=1;j<=dim;j++){
        if(cluster_center[i].clus[j]!=SUM[i].average[j]){
            isContinue=1;// means the cluster has been updated, continue to calculate
            break;
        }
        }
    }
    return isContinue;
}

/**********implementate the K means******************/
void K_means(int K,int dim,coord*data,ClusterCenter* cluster_center,double* distanceFromCenter,int numberofpoints,SumAll *SUM,char *filename){
    for(int round=1;round<=round_time;round++){
        printf("round time%d\n",round);
        allocatecluster_all(K,dim,data,cluster_center,distanceFromCenter,numberofpoints);
        int isContinue=re_calculate(K,dim,SUM,numberofpoints,data,cluster_center);
       // printf("iscontinue %d\n",isContinue);
        if(isContinue==0)
            break;
    }
    writetofile(K,dim,numberofpoints,data,filename);
}
/**Explanation:  in my solution ,the K-means recursion will be terminated by two ways: the coordinates of centroids stop changing or the recursion time exceed Max times*****/

/***********After finishing the K-means, write the coordinates and assignes cluster number of every point*****/
void writetofile(int K,int dim,int numberofpoints,coord *data, char *filename)
{

    FILE* file;
    file=malloc(sizeof(FILE*)*(K+1));
    if(!file){
        printf("malloc file error!\n");
        exit(0);
    }
    file= fopen(filename, "w");
 
     fprintf(file,"%d %d\n",numberofpoints,dim+1);
for(int i=1;i<=numberofpoints;i++){
    for(int j=1;j<=dim;j++){
        fprintf(file, "%lf ",data[i].test[j]);
    }
    fprintf(file,"%d\n",data[i].clusterID);
}
    fclose(file);
}
/***********read the first line of input file to get information of the numbers of data and dimension of coordinates*/
int* Readfirstline(char *filename){
    int* index=malloc(sizeof(int));
    
    char *line = NULL;
    int n = 0;
    printf("check2%s\n",filename);
    FILE *coordFile = fopen(filename, "r");
    int i = 1;
    getline(&line, &n, coordFile);
    int items2 = sscanf(line,"%d %d",&index[0],&index[1]);
     //printf("%d %d\n",index[0],index[1]);
    fclose(coordFile);
    return index;
}

/************read all the data********************/
void  Readfile(int numberofpoints,int dim,coord *data,char *filename){
        char *line = NULL;
        int n = 0;
        FILE *coordFile = fopen(filename, "r");
        int i = 1;
        getline(&line, &n, coordFile);
/************check attributes*********************/
        while(getline(&line, &n, coordFile) != -1 && i < MAX)
        {
             char a[20]="%lf";
                    for(int j=1;j<=dim;j++){
                        if(j>1){
                    char b[20]="%*lf";
                    strcat(b,a);
                    strcpy(a,b);
                        }
                     //printf("a: %s\n",a);
                     int items = sscanf(line,a, &data[i].test[j]);
                    }
            data[i].clusterID=0;
            i++;
        }
        fclose(coordFile);
}
