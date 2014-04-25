#ifndef __MOMENT_CLASSIFIERID
#define __MOMENT_CLASSIFIERID

#include <utility>
#include <vector>
#include <math.h>
#include <armadillo>
#include "Classifier.hpp"

using namespace std;
using namespace arma;

class MomentClassifierId:public Classifier{
public:  
  /* The moments to be calculated */
  vector<pair<int, int> > moments;
  vector<vector<double> > class_averages;
  vector<double> rms_averages;
  
  /* averaged covariance matrix */
  mat covmat;
  MomentClassifierId(ClassPool _cp_train, vector<pair<int,int> > _moments)
    : Classifier(_cp_train)
  {
    moments=_moments;
  }

  MomentClassifierId(ClassPool _cp_train, vector<ClassPool> _cp_test, vector<pair<int,int> > _moments)
    : Classifier(_cp_train, _cp_test)
  {
    moments=_moments;
  }

  
  /* Automatically add the first n pq moments to the list of moments
     to be calculated */
  
  MomentClassifierId(ClassPool _cp_train, int n)
    : Classifier(_cp_train)
  {
    addFirstNMoments(n);
  }

  MomentClassifierId(ClassPool _cp_train, vector<ClassPool> _cp_test, int n)
    : Classifier(_cp_train, _cp_test)
  {
    addFirstNMoments(n);
  }


  /* This functions adds the first N moment features to the moments
     vector */
  void addFirstNMoments(int n){
    int highestPower=1;
    int p=1,q=1;
    int count=0;
    while(count<n){
      moments.push_back(pair<int,int>(q,p));
      if(p==highestPower){
	highestPower++;
	p=1; q=highestPower;
      }
      else{
	p++; q--;
      }
      count++;
    }
  }

  /* Calculate and return the moment features */
  vector<double> extractFeatures(CImage *c){
    vector<double> features;
    for(int i = 0; i < moments.size();i++){
      features.push_back(c->getPQCentralMoment(moments[i].first, moments[i].second));
    }
    return features;
  }

  /* Classify an unknown feature vector to a class */
  int classify(vector<double> features){
    if(class_averages.size()==0){
      cout<<"Need to calculate class averages"<<endl;
      exit(0);
    }
    /* Need to apply rms to each incoming vector */
    for(int i = 0; i < rms_averages.size();i++){
      features[i]/=rms_averages[i];
    }
    int max_i=0;
    double max_d=getMahalanobisDistance(features, class_averages[0]);
    for(int i = 0; i < class_averages.size();i++){
      double tmp=getMahalanobisDistance(features, class_averages[i]);
      if(tmp>max_d){
	max_i=i;
	max_d=tmp;
      }
    }
    return max_i;
  }

  double getMahalanobisDistance(vector<double> _x, vector<double> _mean){
    colvec x = conv_to<colvec>::from(_x);
    colvec mean = conv_to<colvec>::from(_mean);
    colvec tmp = ((inv(covmat)*mean).t())*x+(-0.5*((mean.t())*inv(covmat)*mean))[0];

    return tmp[0];
  }

  void calculateCovMat(){
    covmat=randu<mat>(moments.size(), moments.size());
    covmat.zeros();
    int count=0;
    for(int i = 0; i < feature_pool.size(); i++){
      for(int j = 0; j < feature_pool[i].size(); ++j){
	colvec v = conv_to<colvec>::from(feature_pool[i][j]);
        covmat+=v*v.t();
	count+=1;
      }
    }
    covmat/=count;
  }

  /* applies an RMS transform. assumes the feature vector has already
     been filled. */
  void applyRMSTransform(){

    /* Need a count variable since there might not be the same number
       of instances in each class */
    int count=0;
    vector<double> averages(moments.size(), 0.0);
    for(int i = 0; i < feature_pool.size();i++){
      for(int j = 0; j < feature_pool[i].size();j++){
	for(int k = 0; k < feature_pool[i][j].size();k++){
	  averages[k]+=pow(feature_pool[i][j][k], 2);
	}
	count++;
      }
    }
    for(int i = 0; i < averages.size();i++){
      averages[i]/=(double)count;
      averages[i]=sqrt(averages[i]);
    }
   
    for(int i = 0; i < feature_pool.size();i++){
      for(int j = 0; j < feature_pool[i].size();j++){
	for(int k = 0; k < feature_pool[i][j].size();k++){
	  feature_pool[i][j][k]/=averages[k];
	}
      }
    }
    rms_averages=averages;
  }

  /* Calculates the class averages for each class. It is assumed that
     this is being called after getting the feature values and
     applying RMS transformation. */
  void calculateClassAverages(){
    for(int i =0; i < feature_pool.size();i++){
      int count=0;
      vector<double> averages(moments.size(), 0.0);
      for(int j =0; j < feature_pool[i].size();j++){
	for(int k =0; k < moments.size();k++){
	  averages[k]+=feature_pool[i][j][k];
	}
	count++;
      }
      for(int k = 0; k < averages.size();k++){
	averages[k]/=(double)count;
      }
      class_averages.push_back(averages);
    }
  }

  double getEuclidianDistance(vector<double> v1, vector<double> v2){
    double d=0;
    for(int i =0;i<v1.size();i++){
      d+=pow(v1[i]-v2[i], 2);
    }
    return sqrt(d);
  }

};

#endif
