
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>


#define POP_SIZE 10
#define LOOP 100
#define MIN_CLASS 14
#define MAX_CLASS 31


#define NO 0
#define YES 1



#define STUD_NUM 0

#define SEX 1
#define FEMALE 0
#define MALE 1

#define HYPERACT 2
#define LOGSKILL 3
#define LANGUAGESKILL 4
#define FRIEND1 5
#define FRIEND2 6
#define FRIEND3 7
#define CLASS_VALUE 8

#define VERY_LOW 0
#define LOW 1
#define AVERAGE 2
#define HIGH 3
#define VERY_HIGH 4
#define NUM_STUD 200


#define NUM_ATTRIBUTES 9

#define NUM_CLASSES 9

#define NUM_MUTATION 1


typedef struct {
    int genome[NUM_STUD][NUM_ATTRIBUTES];
    int class_sizes[NUM_CLASSES];
    double fit_sex; /*not used but important for later versions*/
    double fit_hyperact; /*not used but important for later versions*/
    double fit_logskill; /*not used but important for later versions*/
    double fit_languageskill; /*not used but important for later versions*/
    double fit_friends;
    double fit_value;
    double density_value;
    double overall_fitness;
    int cand_num;

} individual;

typedef struct {
    int attr[NUM_ATTRIBUTES];
} student;

int createStudents(student stud[NUM_STUD]);/*creates students with predefined values*/
void createRandPerm(int perm[NUM_STUD]);/*creates a random permutation of the students numbers the student is than assigned to its respective number*/
void displaySol(individual cand, int class_size[]);/*shows the created candidate*/
void classDistr(int num_class, individual *cand);/*creates a class distribution for a candidate*/
void calcFitness(individual *cand); //calculates fitness for a candidate
void createClasses(student stud[NUM_STUD], int class_size[NUM_CLASSES]);
void createOffspring(individual offspring[2], individual cand[POP_SIZE]);
int tournamentSelection(individual *rival1, individual *rival2);

int main(void) {
    student stud[NUM_STUD];
    double fitness_pop = 0;
    createStudents(stud); // Initialize students

    individual cand[POP_SIZE];
    for (int numcand = 0; numcand < POP_SIZE; numcand++) {
        createClasses(stud, cand[numcand].class_sizes);

        for (int i = 0; i < NUM_STUD; i++) {
            for (int j = 0; j < NUM_ATTRIBUTES; j++) {
                cand[numcand].genome[i][j] = stud[i].attr[j];
            }
        }
        calcFitness(&cand[numcand]);
        cand[numcand].cand_num = numcand;
        displaySol(cand[numcand], cand[numcand].class_sizes);
        fitness_pop = fitness_pop + cand[numcand].overall_fitness;
    }
    fitness_pop = fitness_pop/POP_SIZE;

    printf("Mean Fitness of Population: %lf \n \n", fitness_pop );
    int k=0;
    int best = 0;
    for ( int i = 0; i < POP_SIZE-1; i++){
        
        if (cand[k].overall_fitness<cand[i+1].overall_fitness){
            best = k;
        }
        else{
            best = i+1;
            k++;
        }
        
    }
    printf("BEST CAND of Population: %d with Fitness %lf \n \n", best , cand[best].overall_fitness );
    printf("======================================================================== \n \n \n" );


    
    
    individual offspring[2];
    for (int i = 0; i < NUM_STUD; i++) {
            for (int j = 0; j < NUM_ATTRIBUTES; j++) {
                offspring[0].genome[i][j] = stud[i].attr[j];
                
                offspring[1].genome[i][j] = stud[i].attr[j];
                
            }
        }
        offspring[0].cand_num = NUM_STUD;
        offspring[1].cand_num = NUM_STUD+1;
for (int i = 1; i < LOOP; i++){
    createOffspring(offspring ,cand);
    double fitness_pop = 0;
    for (int numcand = 0; numcand < POP_SIZE; numcand++){
        calcFitness(&cand[numcand]);
        displaySol(cand[numcand], cand[numcand].class_sizes);
        
        fitness_pop = fitness_pop + cand[numcand].overall_fitness;

    }
    fitness_pop = fitness_pop/POP_SIZE;

    printf("Mean Fitness of Population: %lf \n ", fitness_pop );
    int k=0;
    int best = 0;
    for ( int i = 0; i < POP_SIZE-1; i++){
        
        if (cand[k].overall_fitness<cand[i+1].overall_fitness){
            best = k;
        }
        else{
            best = i+1;
            k=i+1;
        }
        
    }
    printf("BEST CAND of Population: %d with Fitness %lf \n ", best , cand[best].overall_fitness );
    printf("======================================================================== \n " );

}

    

    return EXIT_SUCCESS;
}


int createStudents(student stud[NUM_STUD]) {
    srand(2);
    int z,p;
    for (z = 0; z < NUM_STUD; z++) {
        for (p = 5; p < 8;p ++){
        stud[z].attr[p] = NUM_STUD;
        } 
        }
    int k, t, l ;
    for (k = 0; k < NUM_STUD; k++) {
        stud[k].attr[STUD_NUM] = k; 
        int sex = rand() % 2;
        stud[k].attr[SEX] = sex;
        int a;
        double mean = (double)4.0/2.0;
        double std_dev = sqrt(2.0) / 3.0;

        for (t = 2; t < FRIEND1; t++){
        double j, f;
        double pi = 3.14159265;
        j = (double)rand() / RAND_MAX;
        f = (double)rand() / RAND_MAX;
        double nrmldis = sqrt(-2 * log(j)) * cos(2 * pi * f);
        double g = mean + std_dev * nrmldis;
        int sim = round(g);
        stud[k].attr[t] = sim;
        }

         int num_friends = rand() % 4; // Number of friends (0 to 3)
        for (int f = 0; f < num_friends; f++) {
            int rand_stud;
            do {
                rand_stud = rand() % NUM_STUD;
            } while (rand_stud == k || 
                     stud[k].attr[FRIEND1] == rand_stud || 
                     stud[k].attr[FRIEND2] == rand_stud || 
                     stud[k].attr[FRIEND3] == rand_stud);
            int added_to_k = 0; // Use integers to simulate boolean (0 = false, 1 = true)
            int added_to_rand_stud = 0;
            for (int i = FRIEND1; i <= FRIEND3; i++) {
                if (stud[k].attr[i] == NUM_STUD) {
                    stud[k].attr[i] = rand_stud;
                    added_to_k = 1;
                    break;
                }
            }
            for (int i = FRIEND1; i <= FRIEND3; i++) {
                if (stud[rand_stud].attr[i] == NUM_STUD) {
                    stud[rand_stud].attr[i] = k;
                    added_to_rand_stud = 1;
                    break;
                }
            }
            if (!added_to_k || !added_to_rand_stud) {
                for (int i = FRIEND1; i <= FRIEND3; i++) {
                    if (stud[k].attr[i] == rand_stud) {
                        stud[k].attr[i] = NUM_STUD;
                        break;
                    }
                }
                for (int i = FRIEND1; i <= FRIEND3; i++) {
                    if (stud[rand_stud].attr[i] == k) {
                        stud[rand_stud].attr[i] = NUM_STUD;
                        break;
                    }
                }
            }
        }
    }
   
    return 0;
}

void createClasses(student stud[NUM_STUD], int class_size[NUM_CLASSES]) {
    int i, v, t, g;
    int norm_classsize = NUM_STUD / NUM_CLASSES;
    int left = NUM_STUD % NUM_CLASSES;
    for (i = 0; i < NUM_CLASSES; i++) {
        class_size[i] = norm_classsize;
    }
    i = 0;
    v = 0;
    while (v < left) {
        class_size[i]++;
        i++;
        v++;
    }
    if (norm_classsize - 3 < MIN_CLASS && norm_classsize + 3 > MAX_CLASS) {
        for (i = 0; i < 20; i++) {
            int rand_swap1 = rand() % NUM_CLASSES;
            int rand_swap2 = rand() % NUM_CLASSES;
            if (class_size[rand_swap1] > MIN_CLASS && class_size[rand_swap2] < MAX_CLASS) {
                class_size[rand_swap1]--;
                class_size[rand_swap2]++;
            }
        }
        for (t = 0; t < NUM_CLASSES; t++) {
            if (class_size[t] < MIN_CLASS || class_size[t] > MAX_CLASS) {
                for (i = 0; i < NUM_CLASSES; i++) {
                    class_size[i] = norm_classsize;
                }
                i = 0;
                v = 0;
                while (v < left) {
                    class_size[i]++;
                    i++;
                    v++;
                }
            }
        }
    }
    int class_size_count[NUM_CLASSES];
    for (t = 0; t < NUM_CLASSES; t++) {
        class_size_count[t] = class_size[t];
    }

    for (g = 0; g < NUM_STUD; g++) {
        int class_num = rand() % NUM_CLASSES;
        while (class_size_count[class_num] == 0) {
            class_num = rand() % NUM_CLASSES; 
        }
        if (class_size_count[class_num] > 0) {
            stud[g].attr[CLASS_VALUE] = class_num;
            class_size_count[class_num]--;
        } else {
            printf("Error: Invalid class assignment for student %d\n", g);
        }
    }


}

void displaySol(individual cand, int class_size[]) {
    int i, j,g ;
    printf("OVERALL FITNESS %lf \n", cand.overall_fitness);

    printf("-------------------------------------------");
    printf(" \n");
   
  printf(" \n");
    printf(" \n");
    
}

void calculateMeanAndStdDev(int attribute, individual *cand, double *meanOverall, double *sdOverall, double *meanClass, double *sdClass) {
    double sum = 0;
    double varianceSum = 0;
    for (int i = 0; i < NUM_STUD; i++) {
        sum += cand->genome[i][attribute];
    }
    *meanOverall = sum / NUM_STUD;
    for (int i = 0; i < NUM_STUD; i++) {
        varianceSum += pow(cand->genome[i][attribute] - *meanOverall, 2);
    }
    *sdOverall = sqrt(varianceSum / NUM_STUD);

    for (int c = 0; c < NUM_CLASSES; c++) {
        sum = 0;
        varianceSum = 0;

        for (int i = 0; i < NUM_STUD; i++) {
            if (cand->genome[i][CLASS_VALUE] == c) {
                sum += cand->genome[i][attribute];
            }
        }

        if (cand->class_sizes[c] > 0) {
            meanClass[c] = sum / cand->class_sizes[c];

            for (int i = 0; i < NUM_STUD; i++) {
                if (cand->genome[i][CLASS_VALUE] == c) {
                    varianceSum += pow(cand->genome[i][attribute] - meanClass[c], 2);
                }
            }
            sdClass[c] = (cand->class_sizes[c] > 1) ? sqrt(varianceSum / (cand->class_sizes[c] - 1)) : 0.0;
        } else {
            meanClass[c] = 0;
            sdClass[c] = 0;
        }
    }
}


void calcFitness(individual *cand){
        for (int p = 0; p < NUM_CLASSES; p++){
         cand->class_sizes[p] = 0;
        }

        for (int i = 0; i < NUM_STUD; i++){
        for (int p = 0; p < NUM_CLASSES; p++){
            if(cand->genome[i][CLASS_VALUE] == p){
             cand->class_sizes[p] = cand->class_sizes[p]+1;
            }
        }
        
        }
    double meanOverall[4], sdOverall[4];
    double meanClass[4][NUM_CLASSES], sdClass[4][NUM_CLASSES];
    int attributes[] = {SEX, HYPERACT, LOGSKILL, LANGUAGESKILL};
    for (int i = 0; i < 4; i++) {
        calculateMeanAndStdDev(attributes[i], cand, &meanOverall[i], &sdOverall[i], meanClass[i], sdClass[i]);
    }

    // Partial fitness
    double partFitness[4] = {0};
    for (int i = 0; i < 4; i++) {
        double fitness = 0;
        for (int p = 0; p < NUM_CLASSES; p++) {
            fitness += fabs(meanClass[i][p] - meanOverall[i]) + fabs(sdClass[i][p] - sdOverall[i]);
        }
        partFitness[i] = fitness;
    }

double mean_classsize = NUM_STUD/NUM_CLASSES;
double dev_classsize = 0;
for (int t =0; t< NUM_CLASSES; t++){
    dev_classsize = dev_classsize + sqrt((mean_classsize- cand->class_sizes[t])*(mean_classsize- cand->class_sizes[t])/NUM_CLASSES);
}

    
    int friendCount = NUM_STUD*3;
    int a;
    int classStudNumbers[NUM_CLASSES][NUM_STUD];
    for (int u = 0; u < NUM_STUD; u++){
        for (a = 0; a < NUM_CLASSES; a++){
                classStudNumbers[a][u] = NUM_STUD;
            }
    }
    

    for (a = 0; a < NUM_CLASSES; a++){
        int count = 0;
        for (int u = 0; u < NUM_STUD; u++){
            if (cand->genome[u][CLASS_VALUE]==a){
                classStudNumbers[a][count] = cand->genome[u][STUD_NUM];
                count ++;
            }
        }
        
       
        
    }
    
    int g, d, f; 
    for (a = 0; a < NUM_CLASSES; a++){
        
        for (g = 0; g < NUM_STUD; g++){
        if (classStudNumbers[a][g+1] != NUM_STUD){
           

            
         for (d = FRIEND1; d <= FRIEND3; d++){ 
            for (f = g+1; f < NUM_STUD; f++){
             if (classStudNumbers[a][f] != NUM_STUD){
                if (cand->genome[classStudNumbers[a][g]][d]== cand->genome[classStudNumbers[a][f]][STUD_NUM]){

                    friendCount = friendCount-2;

                }
            }
             }
            }

         }

         
        }  
            

    }

double weightSex = 4;
double weightHypLogLang = 1;
double weightFriends = 2/(NUM_STUD*3);
double weightClass = 4/17;
double wsm_FITNESS = (weightSex*partFitness[0]+ weightHypLogLang* (partFitness[1]+partFitness[2]+partFitness[3])+ weightFriends*friendCount+weightClass*dev_classsize);

cand->overall_fitness = wsm_FITNESS;
 
}


int tournamentSelection(individual *rival1, individual *rival2) {
    int best; 
    if (rival1->overall_fitness > rival2->overall_fitness) {
        int lucky_num = rand() / 100; 
        if (lucky_num < 99) {
            best = rival2->cand_num; printf("Select offspring better");
        } else {
            best = rival1->cand_num;
        }
    }

    else if (rival1->overall_fitness == rival2->overall_fitness) {
        int lucky_num = rand() / 100; 
        if (lucky_num < 50) {
            best = rival1->cand_num;
        } else {
            best = rival2->cand_num;
        }
    }

    else {
        int lucky_num = rand() / 100; 
        if (lucky_num < 99) {
            best = rival1->cand_num;
        } else {
            best = rival2->cand_num;
        }
    }
    return best; 
}