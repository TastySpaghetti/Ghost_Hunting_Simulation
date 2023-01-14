#include "defs.h"


/*
	Function: initRoomList
	 Purpose: initializes the given roomlist's properties
	      in: linked list of rooms to be initialized
*/
void initRoomList(RoomListType *list){
	list->head = NULL;
}



/*
	Function: initRoom
	 Purpose: initializes the given room's properties
	      in: room to be initialized
	      in: name of the room
*/
void initRoom(RoomType *room, char *name){
	//initialize room properties
	strcpy(room->name, name);
	room->connectedRooms = malloc(sizeof(RoomListType));
	room->evidenceList = malloc(sizeof(EvidenceListType));
	room->ghost = NULL;
	
	if (sem_init(&(room->mutex), 0, 1) < 0){
		printf("Semaphore initialization error");
	}
}

/*
	Function: appendRoom
	 Purpose: Adds the room to the end of the given linked list
	      in: a room linked list
	      in: room to be appended
*/
void appendRoom(RoomListType *list, RoomNode *room){
	//declare variables
	RoomNode *curr = list->head;
	
	//if list is empty
	if (list->head == NULL){
		list->head = room;
		room->next = NULL;
	}
	
	else{
		//move to end of linked list
		while (curr->next != NULL){curr = curr->next;}
		
		curr->next = room;
		room->next = NULL;
	}
}

/*
	Function: connectRoom
	 Purpose: adds each room to the others connected rooms list
	      in: room to be connected to the other
	      in: room to be connected to the other
*/
void connectRooms(RoomType *r1, RoomType *r2){
	//create room nodes
	RoomNode *n1 = malloc(sizeof(RoomNode));	//create node for room 1
	n1->data = r1;					//add room 1 data to node
	RoomNode *n2 = malloc (sizeof(RoomNode));	//create node for room 2
	n2->data = r2;					//add room 2 data to node
	
	//add each room to the other's linked list
	appendRoom(r1->connectedRooms, n2);
	appendRoom(r2->connectedRooms, n1);
}

/*----------------------------HELPER FUNCTION----------------------------
	Function: getRoomListSize
	 Purpose: iterates through the given linked list and returns the size of the list
	      in: list that will have its size counted
	  return: size of the linked list
  ----------------------------HELPER FUNCTION----------------------------*/
int getRoomListSize(RoomListType *list){
	//declare variables
	RoomNode *curr = list->head;
	int size=0;
	
	//find list size
	while (curr != NULL){
		curr = curr->next;
		size++;
	}
	
	return size;
}

/*
	Function: cleanupRooms
	 Purpose: deallocate memory of each room in the list
	      in: room list to be cleaned
*/
void cleanupRooms(RoomListType *list){
	//declare variables
	RoomNode *curr = list->head;
	RoomNode *next = NULL;
	
	while (curr != NULL){
		next = curr->next;
		
		//begin cleanup
		cleanupEvidenceList(curr->data->evidenceList);		//free evidence in room
		cleanupConnectedRooms(curr->data->connectedRooms);	//free connected rooms
		//free(curr->data->connectedRooms);
		free(curr->data);					//free room data
		free(curr);						//free room
		
		curr = next;
	}
	
	free(list);
}

/*
	Function: cleanupConnectedRooms
	 Purpose: deallocate memory of each room in the list of connected rooms
	      in: connected room list to be cleaned
*/
void cleanupConnectedRooms(RoomListType* list){
	//declare variables
	RoomNode *curr = list->head;
	RoomNode *next = NULL;
	
	//begin cleaning
	while (curr != NULL){
		next = curr->next;
		
		free(curr);		//free room
		
		curr = next;
	}
	
	free(list);
}


















