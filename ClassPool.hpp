#ifndef __CPOOL
#define __CPOOL

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <stdlib.h>

#include "CReader.hpp" //contains CReader and CImage

using namespace std;

/* The ClassPool class contains instances from every class. To add a
   class to the classpool, specify its filename and class ID string,
   and it will be read into the ID->CLASS map. */

class ClassPool{
public:
  vector<string> ids;
  map<string, vector<CImage *> > m;

  /* No need to initialize anything */
  ClassPool(){};

  /* adds class vector to pool from filename*/
  void addClass(string filename, string id, char character){
    CReader reader(filename, character);
    vector<CImage *> temp;
    reader.read(temp);
    /* Check that id isn't already in pool */
    for(int i = 0; i < ids.size();i++){
      if(ids[i]==id){
	cout<<"Tried to add id that was already in pool"<<endl;
	exit(0);
      }
    }
    ids.push_back(id);
    m[id]=temp;
  }

  /* Prints each ID in the ids vector */
  void printIds(){
    for(int i = 0; i < ids.size(); i++){
      cout<<"\'"<<ids[i]<<"\" ";
    }
    cout<<endl;
  }

};

#endif
