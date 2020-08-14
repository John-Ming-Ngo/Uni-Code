// skeleton program for find duplicate files using sha256 digests
//
// - it reads filenames form standard input
// - it then computes sha256 digests for first two files
// - compares the digests
//
// the program uses C++ containers and strings, but otherwise
// uses standard C APIs
// ===================================================================

/**
This program checks if the content of input files are identical or not, probably.
Uses a call to sha256 program.
John Ming Ngo, 30020834.
*/

//Note: -type f isolates out the directory! No good.
/**
g++ findDupFiles.cpp -o fdupes 
echo hello > hello.txt 
echo hello > hello2.txt 
echo hellp > hello3.txt 
cp hello2.txt hello4.txt 
cp hello3.txt hello5.txt 
mkdir hello6.txt 
echo world > hello7.txt 
find . -type f -name "hello*.txt" | ./fdupes
find . -name "hello*.txt" | ./fdupes 
Match 1:   
	- hello.txt   
	- hello2.txt   
	- hello4.txt 
Match 2:   
	- hello3.txt   
	- hello5.txt 
Could not compute digests for files:   
	- hello6.txt
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <cassert>
#include <string>
#include <vector>

#include <unordered_map>

// read line from fp
// returns False on EOF
//         True  on success
bool get_line( std::string & line, FILE * fp = stdin)
{
  line.clear();
  while(1) {
    int c = fgetc(fp);
    if( c == '\n') return true;
    if( c == EOF) return ! line.empty();
    line.push_back(c);
  }
}

// returns SHA256 digest of the contents of file fname
// on failure returns empty string
// uses popen() to call sha256sum binary
std::string digest( const std::string & fname)
{
  std::string cmdline = "sha256sum " + fname + " 2> /dev/null";
  FILE * fp = popen( cmdline.c_str(), "r");
  if( fp == NULL) return "";
  std::string output;
  if( ! get_line(output, fp)) return "";
  if( pclose(fp) != 0) return "";
  std::string result;
  for( auto c : output)
    if( isspace(c)) break ; else result.push_back(c);
  return result;
}

int main() {
  // read a list of filenames from stdin
  std::vector<std::string> fnames;
  std::string line;
  while(1) {
    if( ! get_line(line)) break;
    fnames.push_back(line);
  }

  // for debugging purposes print out the filenames
  printf("Read %lu filenames:\n", fnames.size());
  for( const auto & fname : fnames)
    printf("  '%s'\n", fname.c_str());

  if( fnames.size() < 2) {
    printf("I could have worked if you gave me 2+ filenames... :(\n");
    return -1;
  }
  
  //Compute all digests, store them into an array of string digests. - John Ming Ngo
  std::string digests[fnames.size()];
  for(long i = 0; i < fnames.size(); i++) {
	  digests[i] = digest(fnames[i]);
	  
	  //printf("%s\n", digests[i].c_str());
  }

  // OLD CODE
  /**
  // compute the digests for first 2 files
  std::string dig1 = digest( fnames[0]);
  if( dig1.empty()) {
    printf("Could not get digest for file %s\n", fnames[0].c_str());
    return -1;
  }
  printf("Digest 1: %s\n", dig1.c_str());
  std::string dig2 = digest( fnames[1]);
  if( dig2.empty()) {
    printf("Could not get digest for file %s\n", fnames[1].c_str());
    return -1;
  }
  printf("Digest 2: %s\n", dig2.c_str());
  */

  /**
  // compare digests
  if( dig1 == dig2) {
    printf( "First two files are (probably) identical.\n");
  } else {
    printf( "First two files are definitely not identical.\n");
  }
  */
  
  //Digest matches  - John Ming Ngo
  std::unordered_map<std::string, std::vector<std::string>> matchbook = {};
  for (int i = 0; i < fnames.size(); i++) {
	  std::string digest_i = digests[i];
	  if (digest_i.empty()) continue; //Skip no digests.
	  if (matchbook.find(digest_i) != matchbook.end()) continue; //There's already a match case for this digest.
	  
	  std::vector<std::string> matchCase = {fnames[i]}; //Things match with itself.
	  
	  for (int j = (i+1); j < fnames.size(); j++) {
		  if (digests[i] == digests[j]) {
			  matchCase.push_back(fnames[j]); //Add in anything else that matches with it.
		  }
	  }
	  if (matchCase.size() > 1) matchbook.insert({digest_i, matchCase}); //Add it to the matchbook.
  }
  
  //Print match results.  - John Ming Ngo
  int countMatch = 0;
  for (auto & element:matchbook) {
	  countMatch++;
	  std::string digest = element.first;
	  std::vector<std::string> matches = element.second;
	  printf("Match Case %i\n", countMatch);
	  for (std::string & name:matches) {
		  printf("\t\t - %s \n", name.c_str());
	  }
  }
  
  //No Digests  - John Ming Ngo
  printf("Cannot Compute Digests for: \n");
  for (int i = 0; i < fnames.size(); i++) {
	if (digests[i].empty()) {
		printf("%s\n", fnames[i].c_str());
	}
  }

  return 0;
}

