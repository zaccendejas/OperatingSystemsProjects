#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
	srand(time(NULL));
	int TotalMallocs = 1000;
	int *p[TotalMallocs];
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// CREATE A LARGE HEAP WITH VARIABLE BLOCK SIZES //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	for (int i = 0; i < TotalMallocs; i++){
		int Size = rand() % 500 + 1;
		p[i] = malloc(Size);
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// TURN THE HEAP INTO ONE LARGE BLOCK //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	for (int i = 0; i < TotalMallocs; i++){
		free(p[i]);
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~//
	// TRY TO REFILL THE HEAP //
	//~~~~~~~~~~~~~~~~~~~~~~~~//
	for (int i = 0; i < TotalMallocs; i++){
		int Size = rand() % 500 + 1;
		p[i] = malloc(Size);
		if (i > 0 && i % 10 == 0) free(p[i-4]); //Free an arbitrary block every once in a while to see the impact on coalescing
	}

}
