#include "symtable.h"
#define BUCKETS 509
#define MULTIPLIER 65599


struct binding{
    char *pcKey;
    void *pvValue;
    struct binding *next;
};

struct SymTable{
    unsigned int size;
    struct binding **head; /* points to the first pointer of the list */
};

/* hashing function */
static unsigned int SymTable_hash(const char *pcKey, int count){
    size_t ui;
    unsigned int uiHash;
    uiHash = 0U;
    for (ui = 0U; pcKey[ui] != '\0'; ui++)
    uiHash = uiHash * MULTIPLIER + pcKey[ui];
    return uiHash%BUCKETS;
}

/* create a new symtable */
SymTable_T SymTable_new(void){
    SymTable_T oSymTable;
    struct binding **Bucks;
    oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
    assert(oSymTable);
    Bucks = malloc(BUCKETS * sizeof(struct binding *)); /* 509 blocks alocated of size struct binding* */
    oSymTable->size = 0;
    oSymTable->head = Bucks;
    return oSymTable;
}

/* free all the allocated memory */
void SymTable_free(SymTable_T oSymTable)
{
    struct binding *temp, *temp_next;
    int i;
    i=0;
    assert(oSymTable);
    while(i<BUCKETS){
        temp = oSymTable->head[i]; /* initialization of temp */
        while(temp!=NULL){
            temp_next = temp->next;
            free(temp->pcKey);
            free(temp);
            temp = temp_next;
          }
        i++;
    }

    free(oSymTable->head);
    free(oSymTable);
}

/* gets the size */
unsigned int SymTable_getsize(SymTable_T oSymTable)
{
    assert(oSymTable);
    return oSymTable->size;
}

/* insert new node */
int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue)
{
    struct binding *new;
    unsigned int Hash;
    assert(oSymTable && pcKey && pvValue);
    if(SymTable_contains(oSymTable,pcKey)==1)return 0;

    new=malloc(sizeof(struct binding));
    new->pcKey=(char*) malloc(strlen(pcKey) + 1);
    strcpy(new->pcKey, pcKey);
    new->pvValue=(void *)pvValue;
    Hash=SymTable_hash(pcKey,BUCKETS);
    new->next=oSymTable->head[Hash];
    oSymTable->head[Hash]=new;
    oSymTable->size++;
    return 1;
}

/* remove a node */
int SymTable_remove(SymTable_T oSymTable, const char *pcKey)
{
    struct binding *temp_prev,*temp;
    int hashing;
    assert(oSymTable);
    assert(pcKey);
    hashing = SymTable_hash(pcKey, BUCKETS);

    temp = oSymTable->head[hashing];
    temp_prev = NULL;
    while(temp != NULL){
        if (strcmp(pcKey, temp->pcKey) == 0) {
            if (temp_prev == NULL)
                oSymTable->head[hashing] = temp->next;
            else
                temp_prev->next = temp->next;
            free(temp->pcKey);
            free(temp);
            oSymTable->size--;
            return 1;
        }
        temp_prev = temp;
        temp = temp->next;
    }
  return 0;
}

/* check if it exists */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey)
{
    struct binding *temp;
    int hashing;
    assert(oSymTable && pcKey);
    hashing = SymTable_hash(pcKey, BUCKETS);
    temp = oSymTable->head[hashing];
    while(temp != NULL){
      if (strcmp(pcKey, temp->pcKey) == 0)return 1;
      temp = temp->next;
    }
    return 0;
}


void *SymTable_get(SymTable_T oSymTable, const char *pcKey)
{
    struct binding *temp;
    int hashing;
    assert(oSymTable);
    assert(pcKey);
    hashing = SymTable_hash(pcKey, BUCKETS);
    temp = oSymTable->head[hashing];
    while(temp!=NULL){
        if (strcmp(pcKey, temp->pcKey) == 0)return temp->pvValue;
        temp = temp->next;
    }
    return NULL;
}


void SymTable_map(SymTable_T oSymTable,void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),const void *pvExtra)
{
    struct binding *temp;
    int i=0;
    assert(oSymTable && pfApply);

    while(i<BUCKETS){
        temp = oSymTable->head[i];
        while(temp!=NULL){
            pfApply(temp->pcKey, temp->pvValue, (void*)pvExtra);
            temp = temp->next;
        }
        i++;
      }
}
