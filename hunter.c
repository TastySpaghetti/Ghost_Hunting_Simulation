#include "defs.h"



/*
	Function: initHunter
	 Purpose: initializes the given hunters properties
	      in: hunter struct to be initialized
	      in: room that the hunter will start off in
	      in: evidence class type the hunter can collect
	      in: name of the hunter
*/
void initHunter(HunterType *hunter, RoomType* room, EvidenceClassType et, char *name){
	hunter->currRoom = room;
	hunter->evidenceType = et;
	hunter->evidence = malloc(sizeof(EvidenceListType));
	strcpy(hunter->name, name);
	hunter->fear = 0;
	hunter->boredom = BOREDOM_MAX;
}

/*
	Function: cleanupHunter
	 Purpose: deallocate memory of each evidence in the hunters list
	      in: hunter's evidence list to be cleaned
*/
void cleanupHunter(HunterType *h){
	//declare variables
	cleanupEvidenceList(h->evidence);
}

void *hunterThread(void* arg){
	//declare variables
	HunterType *h = (HunterType *) arg;	//get parameter value
	int RNG=-1;				//used for decision making
	
	printf("Hunter %s is now active!\n", h->name);
	
	//begin decision making
	while (1){
		sleep(2);		//time taken between each action by the hunter
		//usleep(200000);
		
		//boredom break check
		if (h->boredom <= 0){
			//remove hunter from room
			for (int i=0; i<MAX_HUNTERS; i++){
				//find yourself in current room
				if (h->currRoom->hunters[i] != NULL
				&& strcmp(h->name, h->currRoom->hunters[i]->name) == 0){
					h->currRoom->hunters[i] = NULL;
				}
			}
			
			printf("==[NOTICE] Hunter %s got bored and left!==\n", h->name);
			return 0;
		}
		
		//fear break check
		if (h->fear >= 100){
			//remove hunter from room
			for (int i=0; i<MAX_HUNTERS; i++){
				//find yourself in current room
				if (h->currRoom->hunters[i] != NULL
				&& strcmp(h->name, h->currRoom->hunters[i]->name) == 0){
					h->currRoom->hunters[i] = NULL;
				}
			}
		
			printf("==[NOTICE] Hunter %s was too scared and ran away!==\n", h->name);
			return 0;
		}
		
		//if ghost is in room
		if (h->currRoom->ghost == NULL){
			h->fear++;	//increment fear
		}
		
		
		RNG = randInt(0, 2);	//generate hunter's decision	
		
		//if hunter chooses to collect evidence
		if (RNG == 0){
			//lock thread
			if (sem_wait(&(h->currRoom->mutex)) < 0){
				printf("[ERROR] Semaphore wait error\n");
				exit(1);
			}
			
			//collect evidence
			hunterCheckEvidence(h);
			
			//unlock thread
			if (sem_post(&(h->currRoom->mutex)) < 0){
				printf("[ERROR] Semaphore wait error\n");
				exit(1);
			}
		}
		
		//if hunter chooses to move
		else if(RNG == 1){
			hunterMove(h);	//move to another room
			h->boredom--;	//decrement boredom timer
		}	
		
		else if(RNG == 2){
			//if another hunter is present
			for (int x=0; x<MAX_HUNTERS; x++){
				//look for a hunter in current room that is not you
				if (h->currRoom->hunters[x] != 0
				&& strcmp(h->name, h->currRoom->hunters[x]->name) != 0){
					printf("Attempting to communicate...\n");
					communicate(h);
				}
			}
		}
		
		
	}
	
	
	
	return 0;
}

/*
	Function: hunterMove
	 Purpose: randomly moves the hunter to an adjacent room
	      in: the hunter struct that will move
*/
void hunterMove(HunterType *h){
	//declare variables
	RoomListType *list = h->currRoom->connectedRooms;	//get list of adjacent rooms
	int size = getRoomListSize(list);			//get size of list
	int RNG = randInt(0, size);				//index of room ghost will move to
	
	//get room to move to
	RoomNode *new = list->head;	//store next room
	for (int x=0; x<RNG; x++){	//iterate through all connected rooms until index
		new = new->next;
	}
	
	//remove hunter from old room's hunter array
	for (int x=0; x<MAX_HUNTERS; x++){
		//find hunter in old rooms array
		if (h->currRoom->hunters[x] != 0
		&& strcmp(h->name, h->currRoom->hunters[x]->name) == 0){
			//remove hunter from old rooms array
			h->currRoom->hunters[x] = NULL;
		}
	}
	
	//move hunter to new room
	h->currRoom = new->data;
	
	//add hunter to new rooms array
	for (int i=0; i<MAX_HUNTERS; i++){
		//find space in array
		if (h->currRoom->hunters[i] == 0){
			//add hunter to array
			h->currRoom->hunters[i] = h;
		}
	}
	
	//display action
	printf("Hunter %s has moved to: %s\n", h->name, h->currRoom->name);
}

/*
	Function: hunterCheckEvidence
	 Purpose: checks the evidence list within the room for collectable evidence
		  if found, add to hunter's list and delete from room's list
		  if not, generate standard evidence
	      in: the hunter that will check for evidence
*/
void hunterCheckEvidence(HunterType* h){
	//if evidence of same type as hunter exists, add to hunter and delete from room
	//if not, generate standard evidence of same type as hunter
	
	//declare variables
	EvidenceNode *curr = h->currRoom->evidenceList->head;
	float v;
	const char *type[] = {"EMF", "TEMPERATURE", "FINGERPRINTS", "SOUND"};
	
	//check room's evidence list for evidence of same type as hunter
	while (curr != NULL){
		//check if evidence types match
		if (curr->data->evidenceType == h->evidenceType){
			//check if evidence if ghostly
			if (checkGhostly(curr->data)){
				//reset hunter boredom
				h->boredom = BOREDOM_MAX;
				
				//take evidence
				appendEvidence(h->evidence, curr->data);
				deleteEvidence(h->currRoom->evidenceList, curr->data->value);
				return;
			}
		}
		curr = curr->next;
	}
	
	//if evidence is not found
	EvidenceType e;
	
	//generate standard evidence and add to hunters list
	if (h->evidenceType == 0) v = randFloat(0.0, 4.90);		//create stan. EMF
	else if (h->evidenceType == 1) v = randFloat(0.0, 27.00);	//create stan. FINGERPRINTS
	else if (h->evidenceType == 2) v = 0.0;				//create stan. TEMPERATURE
	else if (h->evidenceType == 3) v = randFloat(40.0, 70.0);	//create stan. SOUND
	
	initEvidence(&e, h->evidenceType, v);	//generate standard evidence
	appendEvidence(h->evidence, &e);	//add standard evidence to hunters list
	
	//display result
	printf("Hunter %s has found STANDARD %s evidence\n", 
	h->name, type[h->evidenceType]);
}

/*
	Function: communicate
	 Purpose: get ghostly evidence from another hunter in the same room
	      in: the hunter that will begin communication
*/
void communicate(HunterType *h){
	//look at evidence of a hunter in room that is not you
	//iterate through their evidence list
	//check if its ghostly
	//append to your own list if it is
	
	//declare variables
	EvidenceListType *flist = NULL;
	EvidenceNode *fcurr = NULL;
	
	//find friend
	for (int x=0; x<MAX_HUNTERS; x++){
		//look for a hunter in current room that is not you
		if (h->currRoom->hunters[x] != 0
		&& strcmp(h->name, h->currRoom->hunters[x]->name) != 0){
			//access friend's evidence
			flist = h->currRoom->hunters[x]->evidence;
			fcurr = flist->head;
			break;
		}
	}
	
	//look for ghostly evidence in friend's list
	while (fcurr != NULL){
		if (checkGhostly(fcurr->data)){			//if evidence is ghostly
			appendEvidence(h->evidence, fcurr->data);	//add to own evidence
		}
		
		fcurr = fcurr->next;	//move to next evidence
	}
	
	//print result
	printf("Hunter %s has traded evidence with another hunter!\n", h->name);
}



















