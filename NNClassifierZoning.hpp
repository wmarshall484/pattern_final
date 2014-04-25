#ifndef __NNZONING
#define __NNZONING

#include "Classifier.hpp"
#include "MomentClassifierZoning.hpp"

using namespace std;

/* Subclasses the Moment classifier to provide K-NN functionality in
   moment-space */

class NNClassifierZoning:public MomentClassifierZoning{
public:
  int k;
  vector<pair<int, int> > moments;

  NNClassifierZoning(ClassPool _cp_train,vector<pair<int,int> > _moments, int _k)
    :MomentClassifierZoning(_cp_train, _moments)
  {
    k=_k;
  }

  NNClassifierZoning(ClassPool _cp_train, vector<ClassPool> _cp_test,vector<pair<int,int> > _moments, int _k)
    :MomentClassifierZoning(_cp_train, _cp_test, _moments)
  {
    k=_k;
  }

  NNClassifierZoning(ClassPool _cp_train,int n, int _k)
    :MomentClassifierZoning(_cp_train, n)
  {
    k=_k;
  }

  NNClassifierZoning(ClassPool _cp_train, vector<ClassPool> _cp_test, int n, int _k)
    :MomentClassifierZoning(_cp_train, _cp_test, n)
  {
    k=_k;
  }
  
  int classify(vector<double> features){

    for(int i = 0; i < rms_averages.size();i++){
      features[i]/=rms_averages[i];
    }

    vector<pair<int, double> > mins;
    mins.push_back(pair<int, double>(0, getEuclidianDistance(features, feature_pool[0][0])));
    int maxIndex=0;
    for(int i = 0; i < feature_pool.size();i++){
      for(int j = 0; j < feature_pool[i].size();j++){
	pair<int, double> tmp(i, getEuclidianDistance(features, feature_pool[i][j]));
	if(mins.size()<k)
	  mins.push_back(tmp);
	else{
	  if(mins[maxIndex].second>tmp.second){
	    mins[maxIndex]=tmp;
	    /*Find new max */
	    double maxd=mins[0].second;
	    double maxi=0;
	    for(int i = 0; i < mins.size();i++){
	      if(mins[i].second>maxd){
		maxi=i;
		maxd=mins[i].second;
	      }
	    }
	    maxIndex=maxi;
	  }
	  
	}
      }
    }
    map<int,int> freq;
    for(int i =0; i < mins.size(); i++){
      if(freq.find(mins[i].first)==freq.end()){
	freq[mins[i].first]=1;
      }
      else
	freq[mins[i].first]++;
    }
    
    map<int,int>::iterator greatest=freq.begin();
    int max=greatest->second;
    for(map<int,int>::iterator i=freq.begin();i!=freq.end(); i++){
      if(i->second>max){
	greatest=i;
	max=i->second;
      }
    }
    return greatest->first;
  }

};

#endif
