#include "defs.h"


/*
	Function: initGhost
	 Purpose: initializes the given ghost's properties
	      in: ghost to be initialized
*/
void initGhost(GhostType *ghost, GhostClassType gt,  RoomType *room){
	//set ghost evidence types
	if (gt == POLTERGEIST){			//if ghost is poltergeist
		ghost->evidenceType[0] = EMF;
		ghost->evidenceType[1] = TEMPERATURE;
		ghost->evidenceType[2] = FINGERPRINTS;
	}
	else if (gt == BANSHEE){		//if ghost is banshee
		ghost->evidenceType[0] = EMF;
		ghost->evidenceType[1] = TEMPERATURE;
		ghost->evidenceType[2] = SOUND;
	}
	else if (gt == BULLIES){		//if ghost is bullies
		ghost->evidenceType[0] = EMF;
		ghost->evidenceType[1] = FINGERPRINTS;
		ghost->evidenceType[2] = SOUND;
	}
	else if (gt == PHANTOM){		//if ghost is phantom
		ghost->evidenceType[0] = TEMPERATURE;
		ghost->evidenceType[1] = FINGERPRINTS;
		ghost->evidenceType[2] = SOUND;
	}
	
	//initialize the rest of ghost's properties
	ghost->ghostType = gt;
	ghost->currRoom = room;
	ghost->boredom = BOREDOM_MAX;
}

/*
	Function: ghostThread
	 Purpose: the function that will be used for the ghost thread
	 	  encompasses all other ghost action functions
*/
void *ghostThread(void *arg){
	//declare variables
	BuildingType *b = (BuildingType *) arg;	//get parameter value
	int RNG=-1;				//used for decision making
	
	printf("\nGhost is now active!\n");
	
	//begin ghost actions
	while (b->ghost->boredom >= 0){
		sleep(1);
		//usleep(100000);
		
		//if hunter is in room
		if (b->ghost->currRoom->hunters[0] != NULL){
			RNG = randInt(0, 2);			//generate random number
			b->ghost->boredom = BOREDOM_MAX;	//reset ghost boredom
			
			//do nothing
			if (RNG == 0){
				printf("The ghost did nothing\n");
			}
			
			//leave evidence
			else if (RNG == 1){
				//lock thread
				if (sem_wait(&(b->ghost->currRoom->mutex)) < 0){
					printf("[ERROR] Semaphore wait error\n");
					exit(1);
				}
				
				//leave evidence
				ghostLeaveEvidence(b->ghost);
				
				//unlock thread
				if (sem_post(&(b->ghost->currRoom->mutex)) < 0){
					printf("[ERROR] Semaphore wait error\n");
					exit(1);
				}
			}
		}
		
		//if hunter is not in the room
		else{
			RNG = randInt(0, 3);	//generate random number
			b->ghost->boredom--;	//decrement ghost boredom
			
			//do nothing
			if (RNG == 0){
				printf("The ghost did nothing\n");
			}
			
			//leave evidence
			else if (RNG == 1){
				//lock thread
				if (sem_wait(&(b->ghost->currRoom->mutex)) < 0){
					printf("[ERROR] Semaphore wait error\n");
					exit(1);
				}
				
				//leave evidence
				ghostLeaveEvidence(b->ghost);
				
				//unlock thread
				if (sem_post(&(b->ghost->currRoom->mutex)) < 0){
					printf("[ERROR] Semaphore wait error\n");
					exit(1);
				}
			}
			
			//move
			else if (RNG == 2){
				ghostMove(b->ghost);
			}
		}
	}
	
	//remove ghost from the room
	b->ghost->currRoom = NULL;
	
	//print result of ghost getting bored and leaving
	printf("==[NOTICE] The ghost got bored and left!==\n");
	return 0;
}

/*
	Function: ghostMove
	 Purpose: randomly moves the ghost to an adjacent room
	      in: the ghost struct that will move
*/
void ghostMove(GhostType *g){
	//declare variables
	RoomListType *list = g->currRoom->connectedRooms;	//get list of adjacent rooms
	int size = getRoomListSize(list);			//get size of list
	int RNG = randInt(0, size);				//index of room ghost will move to
	
	//get room to move to
	RoomNode *new = list->head;	//store next room
	for (int x=0; x<RNG; x++){	//iterate through all connected rooms until index
		new = new->next;
	}
	
	//set ghost point in the old room to NULL
	g->currRoom->ghost = NULL;
	
	//move ghost to new room
	g->currRoom = new->data;
	
	//display action
	printf("The ghost has moved to: %s\n", g->currRoom->name);

}

/*
	Function: ghostLeaveEvidence
	 Purpose: randomly moves the ghost to an adjacent room
	      in: the ghost struct that will move
*/
void ghostLeaveEvidence(GhostType *g){
	//declare variables
	int RNG = randInt(0, 3);
	float value;
	const char *type[] = {"EMF", "TEMPERATURE", "FINGERPRINTS", "SOUND"};
	char ghostly[MAX_STR] = "STANDARD";
	
	//figure out which evidence type to leave
	if (g->evidenceType[RNG] == EMF){
		value = randFloat(4.7, 5.0);
		if (value > 4.9) strcpy(ghostly, "GHOSTLY");
	}
	else if (g->evidenceType[RNG] == TEMPERATURE){
		value = randFloat(-10.00, 1.0);
		if (value < 1.0) strcpy(ghostly, "GHOSTLY");
	}
	else if (g->evidenceType[RNG] == FINGERPRINTS){
		value = 1.0;
		strcpy(ghostly, "GHOSTLY");
	}
	else if (g->evidenceType[RNG] == SOUND){
		value = randFloat(65.0, 75.0);
		if (value > 70.0) strcpy(ghostly, "GHOSTLY");
	}
	
	//create and add evidence to room
	EvidenceType e;
	initEvidence(&e, g->evidenceType[RNG], value);
	appendEvidence(g->currRoom->evidenceList, &e);
	
	//display action
	printf("The ghost left %s %s evidence in: %s\n", 
	ghostly, type[g->evidenceType[RNG]], g->currRoom->name);
}























