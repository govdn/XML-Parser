#include "parser.h"

// This function takes XML file name as Command line argument
int main(int argc, char *argv[]){

// This is example for using GET function to read data from XML file
  char* value = GET("planes_for_sale.ad.year", argv[1]);
  printf("%s\n",value);
// This is example for using SET function to write data to XML file
  value = SET("planes_for_sale.ad.year","1978", argv[1]);
  printf("%s",value);
  return 0;
}




