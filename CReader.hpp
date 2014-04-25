#ifndef __CREADER
#define __CREADER

using namespace std;

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "CImage.hpp"

/* Pass this class the name of a file that contains a bunch of
   character images and it will automatically read them into a vector
   of pointers toCImage datastructures. */

class CReader {
public:
  string filename;
  char character;
  CReader(string _filename, char _character):filename(_filename),character(_character){    
  }

  /* This function takes in a reference to a vector of pointers to
     CImages and fills it with the characters that were presents in
     the constructor-specified file */

  void read(vector<CImage *> &cimages){
    ifstream myfile(filename.c_str());
    if(!myfile.is_open()){
      cout<<"Error opening "<<myfile<<" for reading."<<endl;
      exit(EXIT_FAILURE);
    }
    while((char)myfile.peek()=='C'){
      string code, h, w, b;
      int count=0;
      int width,height,baseline;
      
      std::getline(myfile, code, ' ');
      std::getline(myfile, h, ' ');
      std::getline(myfile, w, ' ');
      std::getline(myfile, b, '\n');
      //cout<<code<<" "<<h<<" "<<w<<" "<<b<<endl;
      height = atoi(h.c_str()+1);
      width= atoi(w.c_str()+1);
      baseline = atoi(b.c_str()+1);
      char *buf=(char *)malloc(width*height*sizeof(char));
      while(myfile.peek()!='C'&&myfile.peek()!=EOF){
	if(myfile.peek()=='\n')
	  myfile.ignore(1);
	else{
	  //cout<<(char)myfile.peek()<<endl;
	  myfile.read(buf+count++, 1);
	}      
      }
      cimages.push_back(new CImage(character, width,height,baseline,buf));
      }
  }
  
  CReader & operator=(const CReader &in){
    this->filename=in.filename;
    this->character=in.character;
  }

  void setFields(string fn, char c){
    this->filename=fn;
    this->character=c;
  }

};

#endif
