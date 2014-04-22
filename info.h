// Author: Ryan Hoffman
// Date: 3/28/14
// Description: file that allows someone to multiply 2 binary numbers
#ifndef INFO_H
#define INFO_H

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
using namespace std;

enum NAMES{ BOOTHS = 1,
	        ADDSHIFT = 2, 
	        BOTH = 3, 
	        MINBITS = 4, 
	        MAXBITS = 12 };
	        
/* struct Info
 * Description: struct that holds information
 *	 it has the size and how many additions and subtractions that were
 * 	 done
 * Pre: none.
 * Post: none*/

/* function: bool setup()
 * Description: clears everything, but alg
 * Pre: none
 * Post: clears all of the vectors and sets variables to 0
 * param: none
 * return: bool if it returns true then no problem false means that
 *         the multiplicand or multiplier is 0 */
 
/* function: void addAndShift();
 * Description: is the add and shift algrithm
 * Pre: need to have values for multiplier, multiplicand...
 * Post: chages result
 * param: none
 * return: none*/
 
/* function: void boothsAlg(); 
 * Description: does the booths alg. 
 * Pre: need to have multiplier.. set up
 * Post: changes result and numAdd and iterations
 * param:  none
 * return: none */
 
/* function: void clear()
 * Description: clears everything, but alg
 * Pre: none
 * Post: clears all of the vectors and sets variables to 0
 * param: none
 * return: none */
struct Info
{
  // for results
  int size;        // size of the vectors
  int iterations;  // number of iterations that it has to do
  int numAdd;      // number of additions AND subtractions that happened
  int alg;         // which algrithm was used
  float runningTime;
  // for mult
  vector<bool> multiplier;   // the multiplier in bin.
  vector<bool> multiplicand; // the multiplicand in bin.
  vector<bool> result;       // 2x size of others. double register
  vector<bool> twoComp;      // 2's compliment of multiplier
  Info() { clear();}
  bool setup();
  void addAndShift();
  void boothsAlg(); 
  void clear(){ // does not reset which algs it uses
    result.clear();
    multiplicand.clear();
    twoComp.clear();
    multiplier.clear();
    size = iterations = numAdd = 0;
  }
};

ostream &operator<<(ostream &os, const Info &i);
ostream &operator<<(ostream &os, const vector<bool> &a);
/* description: counts the number of 1's in the vector
 * pre: have a vector set up
 * post: none
 * param: a vector
 * return: int that is the number of 1's in the vector*/
int countBits(const vector<bool> &a);

/* description: sets up the vector to be binary.
 * pre: stuff should be a binary number
 * Post: changes a to be a vector of 1,0's 
 * param: vector<bool> and a string. the string should be a bin number
 * return: none */
void setupBin(vector<bool> &a, const string &stuff);

/* description:converts a decimal number to binary
 * pre: stuff should be a decimal number
 * Post: a gets filled with the binary equivilant of stuff
 * param: vector<bool> and a string. the string should be a number
 * return: none  */
void convertDecimalBin(vector<bool> &a, const string &stuff);

/* description: adds 2 binary numbers
 * pre: need to have a be at least the same size as b.
 * Post: a gets changed to be its result + b
 * param: 2 vectors a,b and an OFFSET. OFFSET is a int that allows you
 *        to have it add b to the upper half of a if you set it to > 0
 * return: bool that is the overflow bit*/
bool add(vector<bool> &a,  vector<bool> b, const int &OFFSET,
         const bool &hasSign = true);

/* description: generates 2's compliment of b. stores value in a
 * pre: need to have b be a number
 * Post: a gets changed to 2's compliment of b
 * param: 2 vectors a which is the result, b which is the initial number
 * return: none*/
void gentwoComp(vector<bool> &a, vector<bool> b);

/* description: shift left for vector a
 * pre: a needs to be setup
 * Post: a gets shifted 1 place
 * param: vector a which is the vector that is geting a shift operation
 *        and a bool that is what is shifted in
 * return: the value that was shifted out */
bool shift(vector<bool> &a, const bool &shiftIn);

/* Description: converts a vector of bools into a decimal and returns it
 * Pre: b needs to have a size
 * Post: none
 * Param: vector to convert to decimal, whether there is a sign bit or 
 *        not
 * return: the decimal verion of the vector*/
int toDecimal(const vector<bool> &b, const bool &hasSign = false);

/* Description: converts the integer to a binary number
 * Pre: none
 * Post: a gets changed to be the binary version of origNum
 * Param: a vector a which is the vector that gets a number put in it
 *        origNum = number to be converted
 *        numBits = number of bits the number should be ie if it is 12
 *                  bit numbers then numBits will be 12 and will make
 *                  a have a length of 12 even if it is 1, 0...
 * return: none*/
void intToBin(vector<bool> &a, int origNum, int numBits = MINBITS);

/* Description: inserts 0's in the front of a number
 * Pre: a should have a value but it is not required. a should not be
 *      a signed number
 * Post: a gets amountToPad 0's added in front
 * Param: a vector and the amout it should be padded
 * return: none*/
void pad(vector<bool> &a, int amountToPad);
#endif
