#include "symtable.h"

struct binding{
	char *key;
	void *value;
	struct binding *next;
};

struct SymTable{
	int length;
	struct binding *head;
};

/* create new */
SymTable_T SymTable_new(){
	SymTable_T oSymTable;
	oSymTable = (SymTable_T) malloc (sizeof(struct SymTable));
	oSymTable->length = 0;
	oSymTable->head = NULL;
	return oSymTable;
}

/* free the allocated memory */
void SymTable_free(SymTable_T oSymTable)
{
    struct binding *temp,*temp_next;
    temp=oSymTable->head;
    while(temp!=NULL){
        temp_next=temp->next;
        free(temp->key);
        free(temp);
        temp=temp_next;
    }
    free(oSymTable);
}

/* gets length */
unsigned int SymTable_getLength(SymTable_T oSymTable){
		assert(oSymTable);
    return oSymTable->length;
}


int SymTable_put(SymTable_T oSymTable, const char *pcKey,const void *pvValue){
	struct binding *temp;
  assert(oSymTable);
  assert(pcKey);

	if (SymTable_contains(oSymTable, pcKey)== 1)return 0;

	temp = (struct binding*)malloc(sizeof (struct binding));
	assert(temp);
	temp->key = malloc((strlen(pcKey)+1)*sizeof(char));
	temp->value = (void*)pvValue;
	oSymTable->length++;
	temp->next = oSymTable->head;
	oSymTable->head = temp;

	return 1;
}

int SymTable_remove(SymTable_T oSymTable, const char *pcKey){
	struct binding *temp, *temp_prev;
	assert(oSymTable);
	assert(pcKey);
	/* initialization of the temp pointing on Head, and obviously the prev of it to NULL */
	temp = oSymTable -> head;
	temp_prev = NULL;
	while(temp != NULL){
		/* if we find the key to be removed, we exit the loop */
		if(temp->key != NULL  &&  strcmp(temp->key,pcKey)==0)break;
		temp_prev = temp; /* temp will change to temp->next, so our next prev is the current temp */
		temp = temp -> next;
	}

	/* if temp is null, we did not find the key, so the function fails */
	if(temp == NULL)return -1;

	/* if the prev from temp is null, the key is on the first node */
	if(temp_prev == NULL){
		oSymTable->head = temp->next;
		free(temp->key);
		free(temp);
		oSymTable->length -= 1;
	}else{
		/* skip temp from list, then free it so it's completely removed */
		temp_prev->next = temp->next;
		free(temp->key);
		free(temp);
		oSymTable->length -= 1;
	}
	return 1;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey){
	struct binding* temp;
	assert(oSymTable);
	assert(pcKey);
	temp = oSymTable->head;
	while(temp != NULL){
		/* if we find it, return 1 */
		if(strcmp(temp->key,pcKey) == 0)return 1;
		temp = temp->next;
	}
	return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey){
    struct binding *temp;
    assert(oSymTable);
		assert(pcKey);

    temp = oSymTable->head;
    while(temp != NULL){
				/* if we find it, return it's value */
        if(strcmp(temp->key,pcKey) == 0) return temp->value;
        temp = temp->next;
    }
    return NULL;
}

void SymTable_map(SymTable_T oSymTable,
	void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
	const void *pvExtra){

		struct binding* temp;
		assert(oSymTable);
		temp = oSymTable->head;
		assert(pfApply);
		while(temp != NULL){
				pfApply(temp->key, temp->value,(void*)pvExtra);
				temp = temp->next;
		}
		return;
}
