#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "symtable.h"
#define num 66000

void test_function_print(const char *pcKey, void *pvValue,void *pvExtra){
    printf("no.%s: %s\n",pcKey,(char *)pvValue);
}

int main(){
    SymTable_T table = SymTable_new();
    int i;
    char array[50];

    /* Add 1st */
    if(SymTable_put(table,"a","b"))printf("1. PASS\n");

    /* Check if it ads it twice */
    if(!SymTable_put(table,"a","b"))printf("2. PASS\n");

    /* Add 2nd node */
    if(SymTable_put(table,"c","d"))printf("3. PASS\n");

    /* Add 3rd node */
    if(SymTable_put(table,"e","f"))printf("4. PASS\n");

    /* Remove 3rd node */
    if(SymTable_remove(table,"e"))printf("5. PASS\n");

    for(i=0;i<num;i++){
       sprintf(array, "%d", i);
       SymTable_put(table, array ,"test");
    }

    /* Removed node 1*/
    if(SymTable_remove(table,"1"))
        printf("6. PASS\n");

    /* Get node 2*/
    if(SymTable_get(table, "2"))
        printf("8. PASS %s\n",(char*)SymTable_get(table, "3"));

    /* Try to get a node that doesnt exist to see if it prints NULL */
    if(SymTable_get(table, "2"))printf("9. PASS %s\n",(char*)SymTable_get(table, "-1"));

    SymTable_map(table,test_function_print,"");
    printf("If you got 9xPASS you are an OG\n");
    SymTable_free(table);

     return 0;

}
