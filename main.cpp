// Author: Ryan Hoffman
// Date: 3/28/14
// Description: file that allows someone to multiply 2 binary numbers,
//              using info.h to do so
/* TODO:
 * 		fix add and shift
 * 		add time data
 * 		gen file option?
 * */
#include <fstream>
#include "info.h"

/* Description: gnerates all multiplication combinations for 4 upto and
 *              including maxBits. does it in binary
 * WARNING: SLOW ON A HARD DRIVE WITH A HIGH AMOUNT OF BITS AND ALSO
 * SLOW WITH A SLOW CPU, AND USED TO FULL CREATES 1GB+ FILES
 * BE CAREFUL WITH THIS FUNC!!!!!!!!!
 * 	I am running it on a 4GHz CPU with a SSD so its fastish for me
 * pre: none
 * post: creates a file called filename
 * param: the filename and the max amount of bits
 * return: none*/
void genFile(string filename, int startBit = MINBITS, 
             int maxBits = MAXBITS)
{
  vector<bool> a,b;
  ofstream fout;
  fout.open(filename.c_str());
  int i = 0;
  for(int currentBits = startBit; currentBits <= maxBits; currentBits++)
  {
    for(; i < pow(2,currentBits); i++)// i keeps its value for speed
    for(int j = i; j < pow(2,currentBits); j++) 
    {
	  intToBin(a, i, currentBits);
  	  intToBin(b, j, currentBits);
      fout<<a<<' '<<b<<'\n';
    }
    
    fout.flush();
  }
  
  fout.close();
  return;
}

/* Description: gets the filename from the user
 * pre: none
 * post: none
 * param: type which is input or output to get an input or output file
 * return: the filename */
string getFilename(const string &type)
{
  string filename;
  cout<<"Please enter the name of the "<<type<<" file: ";
  cin>>filename;
  return filename;
}

/* param order: (all optional) 
 *               I WILL ONLY ASK FOR THE FIRST 3 things
 * inputFile.txt
 * outputFile.txt
 * alg to use<1-3> ( 1 for booth, 2 for add&shift, 3 for both)
 * <b/d/g> b=bin file d= decimal, g means generate a file, 
 * number of bits to start generating
 * max number of bits for the file to generate. defaults to 4*/
int main(int argc, char* argv[])
{
  // sets up the in/out put files and opens them
  string filename, stuff, stuff2, fileType;
  int alg = 0, maxBits = MAXBITS, minBits = MINBITS;
  Info result;
  ifstream fin;
  ofstream fout;
  clock_t t1;
  t1 = clock();
  
  // input file setup
  if(argc > 1)
	filename = argv[1];
  else
    fin.open(getFilename("input").c_str());
  
  // output file setup
  if(argc > 2)
    fout.open(argv[2]);
  else
    fin.open(getFilename("output").c_str());
  
  if(argc > 3)
    alg = atoi(argv[3]);
  else
  {
	  while(alg < 1 || alg > 3)
	  { // selects which alg to use
		cout<<"Which algorithm do you want to use: \n";
		cout<<"\t 1. Booth's algorithm \n";
		cout<<"\t 2. Add and Shift\n";
		cout<<"\t 3. Both algs"<<endl;
		cout<<"selection: ";
		cin>>alg;
	  }
  }
  
  if(argc > 4)
    fileType = argv[4];
  else
    fileType = "b";
  
  if(argc > 5 && atoi(argv[5]) >= MINBITS && atoi(argv[5]) <= MAXBITS )
    minBits = atoi(argv[5]);
    
  if(argc > 6 && atoi(argv[6]) >= minBits && atoi(argv[6]) <= MAXBITS )
    maxBits = atoi(argv[6]);
  
  if(fileType == "g")
    genFile(filename, minBits, maxBits);
  
  fin.open(filename.c_str());
  result.alg = alg;
  
  // sets up a file for output and inport into a csv file. this is the
  // format of the file and this is the header column
 /* string format = "Algorithm, Size, Iterations, ";
  format.append("Number of Additions and Subtractions, Multiplier, ");
  format.append("Multiplier Decimal Value, ");
  format.append(" 2's Complement, 2's Complement Decimal Value");
  format.append("Multiplicand, Multiplicand Decimal Value, Result, ");
  format.append("Result Decimal Value, Expected Decimal Value, ");
  format.append("Execution Time (at bottom row)");
  //cout<<format<<endl;
  fout<<format<<endl;
  format = " ";*/
  fout<<"Alg, Length, Amount, Time, Time Per"<<endl; 
  t1 = clock();
  unsigned int counter = 0;
  int prevSize = 0;
  fin>>stuff>>stuff2;
  do // loops through the file.
  { 
    // file input stuff here. if it is binary
    if(fileType == "b" || fileType == "g")
    { 
      setupBin(result.multiplicand, stuff);
      setupBin(result.multiplier, stuff2);
    }
    else if(fileType == "d")
    {// converts it to binary
      convertDecimalBin(result.multiplicand, stuff);
      convertDecimalBin(result.multiplier, stuff2);
    }
     
    // runs the alg here
    if(alg == ADDSHIFT || alg == BOTH)
    {// add and shift
	  alg = result.alg;// changes inf.alg for output
      result.alg = ADDSHIFT;
      result.addAndShift();
      //cout<<result<<'\n';
      //fout<<result<<'\n';
      result.alg = alg;
    }
    
    if(alg == BOOTHS || alg == BOTH)
    {//booth
	  alg = result.alg;// changes inf.alg for output
      result.alg = BOOTHS;
      result.boothsAlg();
      //cout<<result<<'\n';
      //fout<<result<<'\n';
      result.alg = alg;
      result.iterations = result.numAdd = 0;
    }
    
    if(prevSize > 0 && prevSize < result.multiplicand.size())
    {
	  t1 = clock() - t1;
	  fout<<result.alg<<", "<<result.size<<", "<<counter<<", "<<t1<<", "
	      <<(float)(t1/counter)<<endl;
	  t1 = clock();
	  counter = 0;
	}
    
    prevSize = result.multiplicand.size();
    
    //resets everything
    result.clear();
    counter++;
  }// do while loop
  while(fin>>stuff>>stuff2);
  t1 = clock() - t1;
  //cout<<"Time: "<<t1<<endl;
 
  fout<<"Length:"<<result.multiplicand.size()<<", Time: "<<t1<<", amount:"<<endl;
  
  // closes files
  fin.close();
  fout.close();
  
  return 0;
}

