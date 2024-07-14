/*
In this problem, you will be implementing the Knuth Morris Pratt (KMP) algorithm as dicussed in class
Complete the functions 'computeLPSArray' and 'KMPSearch' where the former preprocesses the query
and the latter prints all occurences of the target pattern in the string. 

You can use the print statement already provided to match the output format (You can write your own print statement too)

Running Instructions
Run make to compile your code.
Run make test to test your code on the provided testcase

Input Format
2 line input.
First line contains pattern
Second line contains text.
No whitespace or newline characters withing either string.
(To add support for whitespace, you will have to change main.cpp, you can try doing this on your own OPTIONALLY)

Output Format
Provided in outputs/output0.txt.
Multiple occurences can be possible and will be printed on SEPARATE LINES in the same way.

Please also test the code with your OWN test cases.

- We will test both KMPSearch and computeKMPTable function.
  Please do not change their definitions.

- Please note that we will ONLY copy kmp.cpp to our auto-grader.
  Any modification to any other file will not be part of the
  evaluation of the submission. Please make sure that your
  submission continue to work with the original version of
  the other files.
*/


#include <iostream>
#include <cstring>

// Fills h[] for given pattern pat[0..M-1]
void computeKMPTable(char* pat, int M, int* h)
{
  int i = 1, j = 0;
  h[0] = -1;
  while ( i < M) {
    if( pat[i] != pat[j]){
      h[i] = j;
      while ( j >= 0 && pat[j] != pat[i]) {
        j = h[j];
      }
    } else {
      h[i] = h[j];
    }
    i = i + 1; j = j + 1;
  }
  h[M] = j;
}

// Prints occurrences of txt[] in pat[]
void KMPSearch(char* pat, char* txt)
{
  int M = strlen(pat);
  int T = strlen(txt);
  int h[M + 1];
  computeKMPTable(pat, M, h);
  int i = 0, j = 0;

  while( i < T) {
    if(pat[j] == txt[i]) {
      i = i + 1; j = j + 1;
      if(j == M) {
        printf("Found pattern at index %d \n", i-j);
        j = h[j];
      }
    } else {
      j = h[j];
      if( j < 0) {
        i = i + 1; j = j + 1;
      }
    }
  }

	// printf("Found pattern at index %d \n", index_where_found);
}
