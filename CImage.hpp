#ifndef __CIMAGE
#define __CIMAGE

#include <cmath>
#include <iostream>
#include <string.h>
#include <math.h>

using namespace std;

/* The CImage class contains all the information taken from one
   character entry in a character file; e.g., the width, height, and
   baseline. The actual character itself is stored in a
   one-dimensional character buffer. */

/* The CImage also has functions for obtaining features from the
   buffer.
 */
class CImage {
public:
  char character;
  int width, height, baseline;
  char *buf;
  /*constructor */
  CImage(char _character, int _width, int _height, int _baseline, char *_buf):character(_character),width(_width),height(_height),baseline(_baseline),buf(_buf){}
  

  /* Extract the black area feature */
  int getBlackArea(){
    int count=0;
    for(int i = 0; i < width*height;i++){
      if(buf[i]=='x')
	count++;
    }
    return count;
  }
  
  /* Return the top heavy feature; essentially the ratio of the black
     area of the top 12 lines to the bottom 12 lines, multiplied by
     100, and added to 0.5 */

  double getTopHeavy(){
    int top_area=0;
    int bottom_area=0;
    for(int i = 0; i < width*height; i++){
      if(buf[i]=='x' && i <12*width){
	top_area++;
      }
      else if(buf[i]=='x' && i>=12*width){
	bottom_area++;
      }
    }
    //cout<<"top "<<top_area<<" bottom "<<bottom_area<<" black "<<getBlackArea()<<endl;
    //print();
    return floor(((100.0*(double)top_area)/(double)bottom_area)+0.5);
  }


  /* Prints the character buffer */
  void print(){
    for(int i = 0; i < width*height;i++){
      if(i%width==0)
	cout<<endl;
      cout<<buf[i];
    }
  }

  /* Centers the character buffer, meaning that the distance from the
     left and right edges of the character to the left and right edges
     of the image are different by no more than a value of 1. Same for
     the top and bottom edges of the character and image. */

  void center(){
    char *newbuf = (char *)malloc(16*16*sizeof(char));
    memset((void *)newbuf, (int)'.', 16*16);
    int width_offset=(16-width)/2;
    int height_offset=(16-height)/2;
    for(int i = 0; i < height;i++){
      for(int j = 0; j < width;j++){
	if(buf[i*width+j]=='x'){
	  newbuf[((i+height_offset)*16)+j+width_offset]='x';
	}

      }
    }
    free(buf);
    buf=newbuf;
    width=16;
    height=16;
  }

  /* The PQ moment of area feature is calculated as follows:
     p & q are nonzero integers, and I(X,Y) is 1 if Image[x][y] exists
     --
     sum{x=1->width}(sum{y=1->height}(x^p*y^p*I(X,Y)))
     --
     Ergo, centralMoment(1,0)/centralMoment(0,0) = the x moment.
*/
  int getPQMomentOfArea(int p, int q){
    int total=0;
    for(int i = 0; i < height; ++i){
      for(int j = 0; j < width; ++j){
	if(buf[i*width+j]=='x'){
	  total+=pow((double)j,(double)p)*pow((double)i,(double)q);
	}
      }
    }
    return total;
  }

  /* Wrapper for getPQMomentOfArea with p = 1, q = 0 */
  double getXCentroid(){
    return (double)getPQMomentOfArea(1,0)/(double)getPQMomentOfArea(0,0);
  }

  /* Wrapper for getPQMomentOfArea with p = 0, q = 1 */
  double getYCentroid(){
    return (double)getPQMomentOfArea(0,1)/(double)getPQMomentOfArea(0,0);
  }


  /* This is very similar to getPQMomentOfArea, but it takes the
     moment about the centroid of the image, as opposed to the origin
     of the coordinate system (in this case the upper-left hand
     corner */

  double getPQCentralMoment(int p, int q){
    double total=0;
    double Xc=getXCentroid(), Yc=getYCentroid();
    for(int i = 0; i < height; ++i){
      for(int j = 0; j < width; ++j){
	if(buf[i*width+j]=='x'){
	  total+=pow(((double)j-Xc),(double)p)*pow(((double)i-Yc),(double)q);
	}
      }
    }
    return total;
  }

};

#endif
