#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <papi.h>

struct Event{
    int id;
    char name[50];
};

const struct Event events[] = {
                            {PAPI_L1_DCM, "DCM(L1)"},
                            //{PAPI_L2_DCM, "DCM(L2)"},
                            //{PAPI_LD_INS, "Load instructions"},
                            {PAPI_SR_INS, "Store instructions"},
                            {PAPI_MEM_WCY,"Cycles Stalled Waiting for memory writes"},
                            {PAPI_TOT_INS, "Instructions completed"},
                            //{PAPI_SP_OPS, "Floating point ops(single precision)"}
                            };

int createEventSet(){
    int ret;
    int EventSet = PAPI_NULL;

    ret = PAPI_library_init( PAPI_VER_CURRENT );
	if ( ret != PAPI_VER_CURRENT )
		printf("ERROR: Failed version\n");

    ret = PAPI_create_eventset(&EventSet);
	if (ret != PAPI_OK)
        printf("ERROR on create eventset\n");

    for(int i=0; i<sizeof(events)/sizeof(events[0]); i++){
        ret = PAPI_add_event(EventSet, events[i].id);
        if (ret != PAPI_OK)
            printf("ERROR %d on adding event %s\n",ret, events[i].name);
    }
    printf("\n");

    return EventSet;
}

void generateAndMultiplyMatrices(int n);

int main(){
    //PAPI inits
    int ret;
    int EventSet = createEventSet();
    const int nEvents = sizeof(events)/sizeof(events[0]);


    //starting
    srand(time(NULL));

    for(int i=500; i<=3000; i+=500){
        long long papiVals[nEvents];
        //start counting with PAPI
        ret = PAPI_start(EventSet);
        if (ret != PAPI_OK)
            printf("ERROR: Start PAPI\n");

        generateAndMultiplyMatrices(i);

        //stop counting PAPI and print
        ret = PAPI_stop(EventSet, papiVals);
        if (ret != PAPI_OK)
            printf("ERROR: Stop PAPI\n");

        printf("%dx%d matrix:\n",i,i);
        for(int j=0; j<nEvents; j++){
            printf("%s: %lld\n",events[j].name,papiVals[j]);
        }
        printf("\n");

        ret = PAPI_reset( EventSet );
		if ( ret != PAPI_OK )
			printf("FAIL reset\n");
    }

    //remove events and destroy eventset
    for(int i=0; i<nEvents; i++) {
        ret = PAPI_remove_event( EventSet, events[i].id );
		if(ret != PAPI_OK)
			printf("FAIL on remove event %s\n",events[i].name);
    }

		ret = PAPI_destroy_eventset( &EventSet );
		if(ret != PAPI_OK)
			printf("FAIL on destroy\n");

    return 0;
}

int** generateZeroMatrix(int n){
    int** m = malloc(n*sizeof(int*));

    for(int i=0; i<n; i++){
        int* row = malloc(n*sizeof(int));
        for(int j=0; j<n; j++){
            row[j] = 0;
        }
        m[i]=row;
    }
    return m;
}

int** generateMatrix(int n){
    int** m = malloc(n*sizeof(int*));

    for(int i=0; i<n; i++){
        int* row = malloc(n*sizeof(int));
        for(int j=0; j<n; j++){
            row[j] = rand() % 10;
        }
        m[i]=row;
    }

    return m;
}

int** multiplyMatrices(int** m1, int** m2, int n){
    int** m = generateZeroMatrix(n);

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            for(int t=0; t<n; t++){
                m[i][j] += m1[i][t] * m2[t][j];
            }
        }
    }

    return m;
}

void generateAndMultiplyMatrices(int n){
    int** m1 = generateMatrix(n);
    int** m2 = generateMatrix(n);

    /*int** m3 = */multiplyMatrices(m1,m2, n);
}