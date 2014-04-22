#include "info.h"
bool Info::setup()
{
  size = iterations = numAdd = 0;
  runningTime = 0;
  result.clear();
  // multiply by 0 check
  if(countBits(multiplier) == 0 || countBits(multiplier) == 0)
    return false;
  
  // makes sure the multiplier and multiplicand are the same size
  if(multiplier.size() > multiplicand.size())
    pad(multiplicand, (multiplier.size() - multiplicand.size()));
  else if(multiplier.size() < multiplicand.size())
    pad(multiplicand, (multiplicand.size() - multiplier.size()));
    
  size = multiplicand.size(); // used in outputs
  
  if(alg == ADDSHIFT) // add and shift
  {
    result.resize(2 * multiplier.size());
    add(result, multiplicand, 0, false);
  }
  else if(alg == BOOTHS) // booths
  {
    // puts sign bits in front for 2's compliment to work right
    multiplier.insert(multiplier.begin(), 0);
    multiplicand.insert(multiplicand.begin(), 0);
    gentwoComp(twoComp, multiplier);
    
    result.resize(2 * multiplier.size());
    add(result, multiplicand, 0);
  }
  else
    cout<<alg<<endl;
  
  return true;
}

void Info::addAndShift()
{
  bool E = false;// E bit
  int offset = multiplier.size();
  if(setup() == false)
    return;
  
  for(unsigned int i = multiplier.size(); i > 0; i--)
  {
    if(result[result.size()-1] == 1)
    {
      E = add(result, multiplier, offset, false);
      numAdd++;
    }
    
    iterations++;
    shift(result, E);
    E = 0;// E gets shifted into the result and gets set to 0
  }
  
  return;
}

void Info::boothsAlg()
{
  bool boothBit = 0;
  setup();
  
  int offset = multiplier.size();
  for(unsigned int i = result.size()/2; i > 0; i--)
  {
    if(result[result.size() - 1] == 0 && boothBit == 1)
    {// 0,1 add 
      add(result, multiplier, offset);
      numAdd++;
    }
    else if(result[result.size() - 1] == 1 && boothBit == 0)
    {// 1,0 sub
      add(result, twoComp, offset);
      numAdd++;
    }
    
    boothBit = shift(result, result[0]);// the fist bit gets propigated
    iterations++;
  }
  
  return;
}

ostream &operator<<(ostream &os, const vector<bool> &a)
{
  for(unsigned int i = 0; i < a.size(); i++)
	os<<a[i];  
  return os;
}

ostream &operator<<(ostream &os, const Info &i)
{
  int expected = toDecimal(i.multiplicand, false);
  expected*=toDecimal(i.multiplier, false);
  
  if(i.alg == BOOTHS)
    os<<"Booth's Algorithm";
  else if(i.alg == ADDSHIFT)
    os<<"Add and Shift";
  else
    os<<"Both algorithms";
  
  os<<", "<<i.size<<", "<<i.iterations<<", "<<i.numAdd
    <<", "<<i.multiplier<<", "<<toDecimal(i.multiplier)<<", "
    <<i.twoComp<<", "<<toDecimal(i.twoComp, true)<<", "
    <<i.multiplicand<<", "<<toDecimal(i.multiplicand)<<", "
    <<i.result<<", "<<toDecimal(i.result)<<", "<<expected;
  
  return os;
}

int countBits(const vector<bool> &a)
{
  int counter = 0;
  for(unsigned int i = 0; i < a.size(); i++)
  {
    if(a[i] == 1)
      counter++;
  }
  return counter;
}

void setupBin(vector<bool> &a, const string &stuff)
{
  a.clear();
  for(unsigned int i = 0; i < stuff.size(); i++)
  {
    if(stuff[i] == '0')
      a.push_back(0);
    else
      a.push_back(1);
  }
  return;
}

void convertDecimalBin(vector<bool> &a, const string &stuff)
{
  int origNum = atoi(stuff.c_str());
  intToBin(a, origNum);
  return;
}

void intToBin(vector<bool> &a, int origNum, int numBits)
{
  a.clear();
  int temp = origNum;
  do
  {
    a.insert(a.begin(), (origNum % 2));
    origNum /= 2;
  }while(origNum > 0);
  
  // pad 0's in front to make it a numBits bit number
  for(int i = 2; i < pow(2, numBits); i*=2)
  {
	if( temp < i )
	  a.insert(a.begin(), 0);
  }
  
  return;	
	
}

bool add(vector<bool> &a,  vector<bool> b, const int &OFFSET,
         const bool &hasSign)
{
  int numOnes = 0;
  unsigned int counter = b.size() -1;
  vector<bool> relvantBits(3, 0);// 0= carry in, 1, is a, 2 is b
  for(int i = a.size() - 1 - OFFSET; i > (0 - !hasSign) ; i--, counter--)
  {// sign handled seperatly
    relvantBits[1] = a[i];// a value
    
    if(counter < b.size())
      relvantBits[2] = b[counter]; // b value
    else
      relvantBits[2] = 0;
      
    numOnes = countBits(relvantBits);
    
    // sets a to the proper value
    if(numOnes % 2 == 0)
      a[i] = 0;
    else
      a[i] = 1;
    
    // sets the relvent bits
    if(numOnes > 1)
      relvantBits[0] = 1;
    else
      relvantBits[0] = 0;
  }//for loop
  
  // sets the last bit and checks for overflow
  if(hasSign == true)
    a[0] = b[0];
    
  if(relvantBits[0] == 1)//if there is a carry in to the sign bit
    return true;
  
  return false;
}

void gentwoComp(vector<bool> &a, vector<bool> b)
{
  b.flip();//flips all of the bits
  a = b; 

  b.clear();
  b.push_back(1);// make b = 1
  add(a,b,0);// add 1 to a
}

bool shift(vector<bool> &a, const bool &shiftIn)
{
  bool retVal = a[a.size()-1];
  a.pop_back();
  a.insert(a.begin(), shiftIn);// inserts a 0 at the front
  return retVal; // used for booth bit; it is the value that gets rem
}

int toDecimal(const vector<bool> &b, const bool &hasSign)
{
	int number = 0, counter = 0;
	if(b.size() == 0)
	  return 0;	
	  
	for(int i = b.size()-1; i > 0; i--)
	  number+=b[i]*pow(2, counter++);
	  
	if(hasSign == false)
	  number+=b[0]*pow(2, counter++);
	else if(hasSign == true && b[0] == 1)
	  number = -number;
	
	return number;
}

void pad(vector<bool> &a, int amountToPad)
{
  while(amountToPad > 0)
  {
    a.insert(a.begin(), 0);
  }
  
  return;
}
