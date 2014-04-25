#ifndef __CLASSIFIER
#define __CLASSIFIER

#include <vector>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "ClassPool.hpp"

using namespace std;

/* The classifir class is meant to be subclassed by actual
   implementations of classifiers.*/

class Classifier{
public:
  /* The training data */
  ClassPool cp_train;
  /* Vector of testing pools */
  vector<ClassPool> cp_test;

  /* This vector stores the features that are extracted from each
     element in each class in the training pool. It assumes that all
     features can be represented as vectors of doubles.The features
     are organized as follows: 
     class -> instance -> feature vector */
  vector<vector<vector<double> > > feature_pool;

  /* Vector of output confusion tables */
  vector<vector<vector<int> > > confusion_tables;

  Classifier(ClassPool _cp_train){
    cp_train=_cp_train;
  }

  Classifier(ClassPool _cp_train, vector<ClassPool> _cp_test){
    cp_train=_cp_train;
    cp_test=_cp_test;
  }

  void runClassifierOnTestPools(){    
    /*for(int i = 0; i < cp_test.size(); i++){
      vector<vector<int> > ct=getEmptyConfusionTable();
      for(int j = 0; j < cp_test[i].ids.size(); j++){
	for(int k = 0; k < cp_test[i].m[cp_test[i].ids[j]].size();k++){
	  string s=classify(extractFeatures(&(cp_test[i].m[cp_test[i].ids[j]][k])));
	  }
	}
      }
      }*/
    int count=0;
    for(vector<ClassPool>::iterator i=cp_test.begin(); i!=cp_test.end();i++){
      vector<vector<int> > ct=getEmptyConfusionTable();
      int actual_class=0;
      for(vector<string>::iterator j = i->ids.begin(); j !=i->ids.end(); j++){
	for(vector<CImage *>::iterator k = (*i).m[*j].begin(); k != (*i).m[*j].end(); k++){
	  int output_class=classify(extractFeatures(*k));
	  //cout<<"("<<output_class<<", "<<*j<<") "<<flush;
	  int classified_as=output_class;
	  ct[actual_class][classified_as]+=1;	  
	}
	actual_class++;
      }
      confusion_tables.push_back(ct);
    }
  }

  virtual int classify(vector<double> features){
    return rand()%cp_train.ids.size();
  }

  /* Populate the feature vector in order of ClassPool ids. Ergo if
     the id string is "asdf", then the feature vector will contain
     vectors of vectors of features in 'a', 's', 'd', 'f' order  */
  void makeFeatureVector(){    
    for(int i = 0; i < cp_train.ids.size(); i++){
      vector<vector<double> > classtmp;      
      int num_images=cp_train.m[cp_train.ids[i]].size();
      for(int j = 0; j < num_images; j++){
	classtmp.push_back(extractFeatures(cp_train.m[cp_train.ids[i]][j]));
      }
      feature_pool.push_back(classtmp);
    }
  }

  /* This function extracts the features from the CImages. It should
     be overridden in subclasses to change which features are
     selected. */

  virtual vector<double>  extractFeatures(CImage *c){
    vector<double> tmp(10, 1.0);
    return tmp;
  }

  /* Add another vector of test instances */
  void addTestClassPool(ClassPool tcp){
    cp_test.push_back(tcp);
  }

  /* Get empty confusion table */
  vector<vector<int> > getEmptyConfusionTable(){
    vector<vector<int> > ret;
    for(int i = 0; i < cp_train.ids.size(); i++){
      vector<int> tmp;
      for(int j = 0; j < cp_train.ids.size(); j++)
        tmp.push_back(0);
      ret.push_back(tmp);
    }
    return ret;
  }

  /* Lazy print confusion table */
  void printCTBasic(vector<vector<int> > &ca){
    for(int i = 0; i < cp_train.ids.size();i++){
      for(int j =0; j < cp_train.ids.size(); j++){
        cout<<setw(2)<<ca[i][j]<<" ";
      }
      cout<<endl;
    }
  }

  /* Prettyprint the confusion table. Finicky. */
  void printConfusionTable(vector<vector<int> > &ca){
    if(ca.size()!=ca[0].size()){
      cout<<"confusion array not square"<<endl;
    }
    vector<int> et1,et2;
    for(int i = 0; i < ca.size();++i){
      et1.push_back(0);
      et2.push_back(0);
    }
    for(int i = 0; i < ca.size();++i){
      for(int j = 0; j < ca.size();++j){
        if(i!=j){
          et1[i]+=ca[i][j];
          et2[j]+=ca[i][j];
        }
      }
    }
    cout<<setw(11)<<0;
    for(int i = 1; i < cp_train.ids.size();i++)
      cout<<setw(10)<<i;
    cout<<setw(10)<<"type 1";
    cout<<endl;
    int diag=0;
    for(int i = 0; i < cp_train.ids.size(); i++){
      cout<<i;
      for(int j = 0; j < cp_train.ids.size();j++){
        cout<<setw(10)<<ca[i][j];
	if(i==j)
	  diag+=ca[i][j];
      }
      cout<<setw(10)<<et1[i]<<endl;
    }
    cout<<"type 2"<<setw(5)<<et2[0];
    int sum=0;
    for(int i = 0; i < cp_train.ids.size(); i++){
      cout<<setw(10)<<et2[i];
      sum+=et2[i];
    }
    cout<<setw(10)<<sum<<" "<<(double)sum/(double(sum+diag))<<endl;  
  }


};

#endif
