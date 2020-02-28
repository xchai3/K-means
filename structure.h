//
//  structure.h
//  read3
//  This file defines all the function will be used in the program.
//  Created by Xipeng Chai on 9/24/19.
//  Copyright © 2019 Xipeng Chai. All rights reserved.
//

#ifndef structure_h
#define structure_h

#include <stdio.h>
/******data structure use to store the coordinates of every point*/
typedef struct coord
{
    double test[100];
    int clusterID;
}coord;
/**********data structure use to store the coordinated of every point***********/
typedef struct ClusterCenter{
  double clus[100];
  int clusterID;
}ClusterCenter;
/************data structure use to sotre  the sum of coordinates for every cluster*******/
typedef struct SumAll{
    double average[100];
}SumAll;

/*******initial cluster , set coordinates for clusters, for the first round, first K point will be used as centroid */
void initial_cluster(int K,int dim,coord*data,ClusterCenter* cluster_center);

/******calculate the distance between one point and a centroid*******************/
void calulate_distance_to_oneCenter(int dataID,int clusterID,int dim,coord*data,ClusterCenter* cluster_center,double* distanceFromCenter);

/*****************calculate the distance of a point to all centroids******************/
void calulate_distance_to_allCenter(int dataID,int K,int dim,coord*data,ClusterCenter* cluster_center,double* distanceFromCenter);

/*******allocate one point to a cluster************************/
void allocatecluster_one(int dataID,int K,double* distanceFromCenter,coord*data);

/**************allocate every point to the closet cluster ******************/
void allocatecluster_all(int K,int dim,coord*data,ClusterCenter* cluster_center,double* distanceFromCenter,int numberofpoints);

/**************calculate the average value of each cluster and update it ********************/
int re_calculate(int K,int dim,SumAll *SUM,int numberofpoints,coord*data,ClusterCenter* cluster_center);
/****************compare the averge value of coordinates with old cluster coordinates*******************/
int  iscontinue(int K,int dim,ClusterCenter* cluster_center,SumAll *SUM);

/*************function to implement K-means****************************************/
void K_means(int K,int dim,coord*data,ClusterCenter* cluster_center,double* distanceFromCenter,int numberofpoints,SumAll *SUM,char *filename);

/***************write the final result to  a file named by users***************************/
void writetofile(int K,int dim,int numberofpoints,coord *data, char *filename);

/*************read the first line to get the number of dataset and the number of attributes of every point*/
int* Readfirstline(char *filename);

/***********read every line in the input file***************/
void Readfile(int numberofpoints,int dim,coord *data,char *filename);

/****************initial the data structure, allocate memory ******************/
coord* initial_data2(int numberofpoints);
ClusterCenter* initial_clustermemory(int K);
double* initialD(int K);
SumAll * initialSum(int K);
#endif /* strcture_h */
