#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include "time_diff.h"
/***********************************************************************
*******
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2
uppercase
  letters and a 2 digit integer. Your personalised data set is included
in the
  code.

  Compile with:
    cc -o CrackAZ99-With-Data CrackAZ99-With-Data.c -lcrypt

  If you want to analyse the results then use the redirection operator
to send
  output to a file that you can view using an editor or the less
utility:

    ./CrackAZ99-With-Data > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/
int n_passwords = 4;

char *encrypted_passwords[] = {

"$6$KB$AN/n6UCl9OdPRhaaofSgPdww5UskQozuQfzZo2Ao6AffX8fruLO5ilo/yGSZnSpKaN8Z1NZxPNBLp7pONgm/a0",

"$6$KB$nCETueE16hy4yjg6Htzziw4VdVxjvqVDxYt/GwFP9M7K5/2J6zcNaOwzs/Y7D7KAjvBMsVPzW2zTIT.oNAVnM1",

"$6$KB$uEuYY/4NIh3xfKZ6RUBQgEYFiN.BmK/eWwkLta5c.xdTeHl.o68TMHpwRoWdjPftdf2OaoEBDKskFS2u60fyh.",

"$6$KB$qhChdJbNF7AOLuaysOQtkjH97UFVx1aEC8Bejm5Tjrzvnsx5WE1L5GOnfmCpb8zjgTk/jRYQrBa2LVzzhp/Ux1"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All
combinations
 that are tried are displayed and when the password is found, #, is put
at the
 start of the line. Note that one of the most time consuming operations
that
 it performs is the output of intermediate results, so performance
experiments
 for this kind of program should not include this. i.e. comment out the
printfs.
*/

void crack(char *salt_and_encrypted){
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='A'; x<='Z'; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%02d", x, y, z);
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
}

int main(int argc, char *argv[]){
  struct timespec start, finish;
  long long int difference;   
  int account = 0;
  clock_gettime(CLOCK_MONOTONIC, &start);
  int i;

  for(i=0;i<n_passwords;i<i++) {
    crack(encrypted_passwords[i]);
  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &difference);
  printf("accumulated %dp\n", account);
  printf("run lasted %9.5lfs\n", difference/1000000000.0);
  printf("Time elapsed was %lldns or %0.9lfs or %0.9lfmin\n", difference, (difference/1.0e9),((difference/1.0e9)/60));
  return 0;
}
