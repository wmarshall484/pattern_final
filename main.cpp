#include <iostream>
#include <string>
#include <sstream>
#include "ClassPool.hpp"
#include "Classifier.hpp"

using namespace std;

int main(int argc, char **argv){
  /* pool 0 is the training set, pools 1, 2, and 3 are the testing
     sets. */
  vector<ClassPool> pools(4, ClassPool());
  
  /* Each character is a different class. */
  string classes="acemnoruvx";
  /* Each character is a different pool. */
  string poolsID="ABCD";

  /* Filepath to char files */
  string path_prefix="C-I/";

  /* Initialize the different classpools from the files */
  for(int i = 0; i < poolsID.size(); i++){
    for(int j = 0; j < classes.size(); j++){
      stringstream ss;
      ss<<path_prefix<<poolsID[i]<<"-"<<classes[j]<<".txt";
      pools[i].addClass(ss.str(), classes.substr(j,1), classes[j]);
    }
    pools[i].centerAll();
  }

  /* Create classifier with train and test class pools */

  Classifier c(pools[0]);
  c.addTestClassPool(pools[1]);
  c.addTestClassPool(pools[2]);
  c.addTestClassPool(pools[3]);
  
  c.makeFeatureVector();
  c.runClassifierOnTestPools();
  c.printConfusionTable(c.confusion_tables[1]);
  
  return 0;
}
