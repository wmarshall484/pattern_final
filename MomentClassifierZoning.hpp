#ifndef __MOMENT_CLASSIFIERZONING
#define __MOMENT_CLASSIFIERZONING

#include <utility>
#include <vector>
#include <math.h>
#include "Classifier.hpp"

using namespace std;

class MomentClassifierZoning:public Classifier{
public:  
  /* The moments to be calculated */
  vector<pair<int, int> > moments;
  vector<vector<double> > class_averages;
  vector<double> rms_averages;
  MomentClassifierZoning(ClassPool _cp_train, vector<pair<int,int> > _moments)
    : Classifier(_cp_train)
  {
    moments=_moments;
  }

  MomentClassifierZoning(ClassPool _cp_train, vector<ClassPool> _cp_test, vector<pair<int,int> > _moments)
    : Classifier(_cp_train, _cp_test)
  {
    moments=_moments;
  }

  
  /* Automatically add the first n pq moments to the list of moments
     to be calculated */
  
  MomentClassifierZoning(ClassPool _cp_train, int n)
    : Classifier(_cp_train)
  {
    addFirstNMoments(n);
  }

  MomentClassifierZoning(ClassPool _cp_train, vector<ClassPool> _cp_test, int n)
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
  virtual vector<double> extractFeatures(CImage *c){
    vector<double> features;
    for(int i = 0; i < moments.size();i++){
      features.push_back(c->getPQCentralMoment(moments[i].first, moments[i].second));
    }
    /* Size of zone edge */
    int zone_edge=2;
    for(int p=0;p<c->height;p+=zone_edge){
      for(int q=0;q<c->width;q+=zone_edge){
	/* Get zone value */
	double val=0;
	for(int i = 0; i < zone_edge; i++){
	  for(int j = 0; j < zone_edge; j++){
	    if(c->buf[c->width*(p+i)+(q+j)]=='x'){
	      val+=1.0;
	    }
	  }
	}
	features.push_back(val);
      }
    }    
    //for(int i = 0; i<1;i++){
    //features.push_back(double(i));
    //}
    
    // Histogram features 
    // Row values first
    /*for(int p=0;p<c->height;p+=1){
      double xval=0;
      for(int q = 0; q<c->width; q++){
	if(c->buf[c->width*(p)+q]=='x'){
	  xval+=1;
	}
      }
      features.push_back(xval);
    }

    //Col values second
    for(int p=0;p<c->width;p+=1){
      double yval=0;
      for(int q = 0; q<c->height; q++){
	if(c->buf[c->width*(q)+p]=='x'){
	  yval+=1;
	}
      }
      features.push_back(yval);
    }*/

    // for(int i = 0; i < features.size();i++)
    //   cout<<features[i]<<" ";
    // cout<<endl;
    return features;
  }

  /* Classify an unknown feature vector to a class */
  virtual int classify(vector<double> features){
    
    if(class_averages.size()==0){
      cout<<"Need to calculate class averages"<<endl;
      exit(0);
    }
    for(int i = 0; i < rms_averages.size();i++){
      if(rms_averages[i]!=0)
	features[i]/=rms_averages[i];
    }
    //for(int i = 0; i < features.size();i++)
    //cout<<features[i]<<" ";
    //cout<<endl;
    // for(int i = 0; i < class_averages.size();i++){
    //   for(int j = 0; j < class_averages[i].size();j++){
    // 	cout<<class_averages[i][j]<<" ";
    //   }
    //   cout<<endl;
    // }
    //exit(0);
    int min_i=0;
    double min_d=getEuclidianDistance(features, class_averages[0]);
    for(int i = 0; i < class_averages.size();i++){
      double tmp=getEuclidianDistance(features, class_averages[i]);
      //cout<<features.size()<<" "<<class_averages[i].size()<<" "<<tmp<<endl;
      if(tmp<min_d){
	min_i=i;
	min_d=tmp;
      }
    }
    return min_i;
  }

  /* applies an RMS transform. assumes the feature vector has already
     been filled. */
  void applyRMSTransform(){

    /* Need a count variable since there might not be the same number
       of instances in each class */
    int count=0;
    vector<double> averages(feature_pool[0][0].size(), 0.0);
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
	  if(averages[k]!=0)
	    feature_pool[i][j][k]/=averages[k];
	  //cout<<feature_pool[i][j][k]<<endl;
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
      vector<double> averages(feature_pool[0][0].size(), 0.0);
      for(int j =0; j < feature_pool[i].size();j++){
	for(int k =0; k < feature_pool[0][0].size();k++){
	  averages[k]+=feature_pool[i][j][k];
	}
	count++;
      }
      for(int k = 0; k < averages.size();k++){
	averages[k]/=(double)count;
	//cout<<averages[k]<<"/"<<(double)count<<" = "<<averages[k]/(double)count<<endl;

      }
      class_averages.push_back(averages);
    }


    
  }

  double getEuclidianDistance(vector<double> v1, vector<double> v2){
    double d=0;
    for(int i =0;i<v1.size();i++){
      //cout<<pow(v1[i]-v2[i], 2)<<endl;
      d+=pow(v1[i]-v2[i], 2);
    }
    //cout<<d<<endl;
    return sqrt(d);
  }

};

#endif
