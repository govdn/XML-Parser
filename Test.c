#include "parser.h"

int main(int argc, char *argv[]){

  char* value = GET("planes_for_sale.ad.year", argv[1]);
  printf("%s",value);
  value = SET("planes_for_sale.ad.year","1978", argv[1]);
  printf("%s",value);
  return 0;
}




