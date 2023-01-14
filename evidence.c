#include "defs.h"



/*
	Function: initEvidenceList
	 Purpose: initializes the given evidencelist
	      in: evidence list to be initialized
*/
void initEvidenceList(EvidenceListType* list){
	list->head = NULL;
}

/*
	Function: initEvidence
	 Purpose: initializes the given evidence properties
	      in: evidence struct to be initialized
	      in: evidence class type
	      in: value of the evidence
*/
void initEvidence(EvidenceType *evidence, EvidenceClassType et, float v){
	evidence->evidenceType = et;
	evidence->value = v;
}

/*
	Function: appendEvidence
	 Purpose: Adds the evidence to the end of the given linked list
	      in: an evidence linked list
	      in: evidence to be appended
*/
void appendEvidence(EvidenceListType *list, EvidenceType *ev){
	//declare variables
	EvidenceNode *curr = list->head;
	EvidenceNode *new = NULL;
	
	//allocate new node and data
	new = malloc(sizeof(EvidenceNode));
	new->data = ev;
	
	//if list is empty
	if (list->head == NULL){
		list->head = new;
		new->next = NULL;
	}
	
	//if list is not empty
	else{
		while (curr->next != NULL){curr = curr->next;}
		
		//append to end of list
		curr->next = new;
		new->next = NULL;
	}
}

/*
	Function: deleteEvidence
	 Purpose: removes the evidence from the linked list
	      in: an evidence linked list
	      in: value of the evidence that will be deleted
*/
void deleteEvidence(EvidenceListType *list, float v){
	//create variables
	EvidenceNode *curr = list->head;
	EvidenceNode *prev = NULL;
	
	//if evidence is at the head
	if (list->head->data->value == v){
		list->head = curr->next;
		return;
	}
	
	//if evidence is not at the head
	while (curr != NULL){
		//check if current evidence matches
		if (curr->data->value == v){
			prev->next = curr->next;	//overwrite the node in the list
		}
		
		//move to the next node
		prev = curr;
		curr = curr->next;
	}
}

/*----------------------------HELPER FUNCTION----------------------------
	Function: checkGhostly
	 Purpose: checks if the given evidence is ghostly
	      in: the evidence to check if its ghostly
	  return: 1 if evidence is ghostly, 0 if not
  ----------------------------HELPER FUNCTION----------------------------*/
int checkGhostly(EvidenceType *ev){
	if (ev->evidenceType == 0 && ev->value > 4.9) return 1;		//if EMF is ghostly
	else if (ev->evidenceType == 1 && ev->value < 1.0) return 1;	//if fingerprints is ghostly
	else if (ev->evidenceType == 2 && ev->value == 1.0) return 1;	//if temperature is ghostly
	else if (ev->evidenceType == 3 && ev->value > 70.0) return 1;	//if sound is ghostly
	
	return 0;	//if not ghostly
}

/*
	Function: cleanupEvidenceList
	 Purpose: deallocate memory of each evidence in list
	      in: evidencelist to be cleaned
*/
void cleanupEvidenceList(EvidenceListType *list){
	//declare variables
	EvidenceNode *curr = list->head;
	EvidenceNode *next = NULL;
	
	//begin cleaning
	while (curr != NULL){
		next = curr->next;
		free(curr);		//free evidence node
		curr = next;
	}
	
	free(list);			//free evidence list
}





















