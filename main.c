#include "defs.h"



int main(int argc, char *argv[])
{
	// Initialize a random seed for the random number generators
	srand(time(NULL));
	
	//declare variables
	char input[MAX_STR];
	char n1[MAX_STR], n2[MAX_STR], n3[MAX_STR], n4[MAX_STR];
	int rng;
	HunterType h1, h2, h3, h4;
	GhostType g;

	// You may change this code; this is for demonstration purposes
	BuildingType building;
	initBuilding(&building);
	populateRooms(&building);
	
	//user inputs hunter's names
	printf("Enter the names of the 4 hunters (space separated): ");
	fgets(input, sizeof(input), stdin);
	sscanf(input, "%s %s %s %s", n1, n2, n3, n4);
	printf("\n");
	
	//create hunters, start position at the van
	initHunter(&h1, building.rooms->head->data, EMF, n1);
	initHunter(&h2, building.rooms->head->data, TEMPERATURE, n2);
	initHunter(&h3, building.rooms->head->data, FINGERPRINTS, n3);
	initHunter(&h4, building.rooms->head->data, SOUND, n4);
	
	//add hunters to building's array of hunters
	building.allHunters[0] = &h1;
	building.allHunters[1] = &h2;
	building.allHunters[2] = &h3;
	building.allHunters[3] = &h4;
	
	
	//get ghost spawn point
	rng = randInt(1, 13);
	RoomNode *spawn = building.rooms->head;
	for (int x=1; x<=rng; x++){		//iterate through rooms until certain room found
		spawn = spawn->next;
	}
	
	//spawn ghost
	initGhost(&g, randInt(0, 4), spawn->data);	//create ghost
	building.ghost = &g;				//add ghost to building
	
	//create and joinhunter threads
	pthread_t th1, th2, th3, th4, tg;
	pthread_create(&th1, NULL, hunterThread, &h1);
	pthread_create(&th2, NULL, hunterThread, &h2);
	pthread_create(&th3, NULL, hunterThread, &h3);
	pthread_create(&th4, NULL, hunterThread, &h4);
	pthread_create(&tg, NULL, ghostThread, &building);
	
	//join threads
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	pthread_join(th4, NULL);
	pthread_join(tg, NULL);
	
	//check results
	printf("\n");
	printf("================RESULTS================\n");
	
	int tally=0;
	const char *type[] = {"POLTERGEiST", "BANSHEE", "BULLIES", "PHANTOM"};
	HunterType *temp;
	
	
	//fear check
	for (int x=0; x<MAX_HUNTERS; x++){
		//tally hunters that fled
		if (building.allHunters[x]->fear >= 100){
			tally++;
		}
		
		//hunter that didn't flee
		else if (building.allHunters[x]->fear < 100){
			printf("Hunter %s was brave and figured out the ghost!\n",
			building.allHunters[x]->name);
			printf("FEAR: %d\n", h1.fear);
			temp = (building.allHunters[x]);
			break;
		}
	}
	
	//print hunter loss statement
	if (tally == 4){
		printf("All hunters ran away in their cowardice... The ghost prevails!!!\n");
	}
	
	//print hunter win statement
	else{
		printf("Hunter %s was able to figure out the ghost\n", temp->name);
		printf("The ghost was a: %s!", type[building.ghost->ghostType]);
	}
	
	cleanup(&building);
	printf("DONE\n");
	return 0;
}


/*
	Function:  randInt
	Purpose:   returns a pseudo randomly generated number, 
	    	   in the range min to (max - 1), inclusively
	in:   	   upper end of the range of the generated number
	return:    randomly generated integer in the range [min, max-1) 
*/
int randInt(int min, int max)
{
	return rand() % (max - min) + min;
}

/*
	Function:  randFloat
	Purpose:   returns a pseudo randomly generated number, 
	     	   in the range min to max, inclusively
	in:        upper end of the range of the generated number
	return:    randomly generated integer in the range [0, max-1) 
*/
float randFloat(float a, float b) {
    // Get a percentage between rand() and the maximum
    float random = ((float) rand()) / (float) RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;
}


















