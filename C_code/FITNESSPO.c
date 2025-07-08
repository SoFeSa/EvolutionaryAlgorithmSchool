/*----------------------------------------------------------------*/
/*----------------------------------------------------------------*/
/*
This code creates one individual for the project ARP2023ClassDistricution
Version 3.0 (31/10/2024)

The version 1.0 description:
The purpose is to test the Creation of one class distribution (1 candidate or individual) 
with 3 classes out of 8 randomly created students with 5 atrributes corresponding to each student
The goals are:
1. get a random permutation or sequence/order of students
2. get a class distribution of 3 classes with some randomness
 (the average class would have 8/3= 2.66 students, which is of course not possible.
  the goal should be that the number of students should be around that average and later
  a high distance from that average should be possible to be punished with a lower fitness)
3. display Individual

The version 2.0 description:
In this version more candidates are created and the students get the possibility to have friends
they want to be in a class with. Additionally the fitness of candidates should be calculated.
GOALS:
1. enter friennds as a attribute for the students and enter the values
2. get the value displayed
3. calculate the fitness
4. create 10 candidates

The version 3.0 description
In this version the individuals are created differently:
1. The order of the students is not changed but the class is assigned as another characteristics uniformly random --> enabled crossover 
2. The values of the students should be normally random distributed
3. friend should be able to be assigned randomly (IMPORTANT: when one side is assigned the other individual ge the same friend)

*/
/*----------------------------------------------------------------*/
/*----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
/*******************************/
/* definitions for readability */
/*******************************/
#define POP_SIZE 20
#define LOOP 10

//defines the maximal and minimal number of children allowed in one class
#define MIN_CLASS 8
#define MAX_CLASS 15


#define NO 0
#define YES 1

#define RANDOM 1

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

//number of students
#define NUM_STUD 32


#define NUM_ATTRIBUTES 9

#define NUM_CLASSES 3

#define NUM_MUTATION 1
#define TOURNM_PROP 0.05



/********************************/
/* Design Parameters            */
/********************************/

typedef struct {
    int genome[NUM_STUD][NUM_ATTRIBUTES];
    int class_sizes[NUM_CLASSES];
    double fit_sex; /*not used but important for later versions*/
    double fit_hyperact; /*not used but important for later versions*/
    double fit_logskill; /*not used but important for later versions*/
    double fit_languageskill; /*not used but important for later versions*/
    double fit_friends;
    double fit_devclass;
    double fitness_values[6];
    double density_value;
    double fitness_si;
    double fitness_raw;
    double overall_fitness;
    double array_distance[POP_SIZE+POP_SIZE];
    double array_distance_all[POP_SIZE*3];
    int cand_num;
    int valid; /*Used to check if candidate in archive is valid*/;

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
void calcSPEAFitness(int archive_boolean, individual *cand, individual archcand[POP_SIZE+POP_SIZE],individual candall[POP_SIZE]);
void createEmptyArchive (individual archcand[POP_SIZE+POP_SIZE]);
int tournamentSelection(individual *rival1, individual *rival2);
double euclideanDistance(individual *cand1, individual *cand2);
double findMaximum(const double arr[], int size);
void sort_array(double* array, int size);
int dominates(individual *a, individual *b);
int compareArrays(double a[], double b[], int size);
void replaceArchive(individual cand[POP_SIZE], individual archcand[POP_SIZE]);
void calcSIFitness(individual archcand[POP_SIZE+POP_SIZE],individual candall[POP_SIZE]);


int main(void) {
    srand(3);
    student stud[NUM_STUD];
    double fitness_pop = 0.0;
    createStudents(stud); // Initialize students

    individual cand[POP_SIZE];
    individual archcand[POP_SIZE+POP_SIZE];
    if (NUM_STUD < NUM_CLASSES * MIN_CLASS || NUM_STUD > NUM_CLASSES * MAX_CLASS) {
        printf("Error: Cannot assign %d students into %d classes within bounds [%d, %d].\n",
               NUM_STUD, NUM_CLASSES, MIN_CLASS, MAX_CLASS);
        return 1;
    }
    createEmptyArchive(archcand);
    for (int numcand = 0; numcand < POP_SIZE; numcand++) {
        createClasses(stud, cand[numcand].class_sizes);

        for (int i = 0; i < NUM_STUD; i++) {
            for (int j = 0; j < NUM_ATTRIBUTES; j++) {
                cand[numcand].genome[i][j] = stud[i].attr[j];
            }
        }
        calcFitness(&cand[numcand]);
        cand[numcand].cand_num = numcand;
        //printf("CANDIDATE %d \n \n", numcand );
        
        
        displaySol(cand[numcand], cand[numcand].class_sizes);
        
    }
    calcSIFitness(archcand,cand);
    for (int numcand = 0; numcand < POP_SIZE; numcand++) {
        calcSPEAFitness(0,&cand[numcand],archcand,cand);
        printf("OVERALL SPEA FITNESS CAND %d : %f\n", numcand, cand[numcand].overall_fitness);
        
    }      
    replaceArchive(cand,archcand);    
    
for (int numcand = 0; numcand < POP_SIZE+POP_SIZE; numcand++){
    if (archcand[numcand].valid == 1){
        printf("OVERALL SPEA FITNESS ARCHCAND %d : %f\n", numcand, archcand[numcand].overall_fitness);
    }
    
    
  //  displaySol(cand[numcand], cand[numcand].class_sizes);
        
}
for ( int il = 1; il  < LOOP; il++){
    printf("ROUND %d : \n", il);
        createStudents(stud); // Initialize students
        if (NUM_STUD < NUM_CLASSES * MIN_CLASS || NUM_STUD > NUM_CLASSES * MAX_CLASS) {
            printf("Error: Cannot assign %d students into %d classes within bounds [%d, %d].\n",
                NUM_STUD, NUM_CLASSES, MIN_CLASS, MAX_CLASS);
            return 1;
        }
    
        for (int numcand = 0; numcand < POP_SIZE; numcand++) {
             
            
            createClasses(stud, cand[numcand].class_sizes);

            for (int i = 0; i < NUM_STUD; i++) {
                for (int j = 0; j < NUM_ATTRIBUTES; j++) {
                    cand[numcand].genome[i][j] = stud[i].attr[j];
                }
            }
            calcFitness(&cand[numcand]);
            cand[numcand].cand_num = numcand;
            //printf("CANDIDATE %d \n \n", numcand );
            
            
            displaySol(cand[numcand], cand[numcand].class_sizes);
            
        }
        calcSIFitness(archcand,cand);
        for (int numcand = 0; numcand < POP_SIZE; numcand++) {
            calcSPEAFitness(0,&cand[numcand],archcand,cand);
            printf("OVERALL SPEA FITNESS CAND %d : %f\n", numcand, cand[numcand].overall_fitness);
            
        }      
        replaceArchive(cand,archcand);    
        
    for (int numcand = 0; numcand < POP_SIZE+POP_SIZE; numcand++){
        if (archcand[numcand].valid == 1){
            printf("OVERALL SPEA FITNESS ARCHCAND %d : %f\n", numcand, archcand[numcand].overall_fitness);
        }
        
        
    //  displaySol(cand[numcand], cand[numcand].class_sizes);
            
    }
    
}

    return EXIT_SUCCESS;
}

/*******************************/
/*Create students with their predefinded ramdomly chosen values*/

/*******************************/

int createStudents(student stud[NUM_STUD]) {
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
        // for the attributes log skill, lang skill, and hyperactivity, the (hopefully) case is that these skills are normally distributed amongst the children and
        //uniformly therefore: a box-muller-transformation is used
        int a;
        double mean = (double)4.0/2.0;
        double std_dev = sqrt(2.0) / 3.0;

        for (t = 2; t < FRIEND1; t++){
        double j, f;
        double pi = 3.14159265;
        j = (double)rand() / RAND_MAX;
        f = (double)rand() / RAND_MAX;
        double nrmldis = sqrt(-2.0 * log(j)) * cos(2.0 * pi * f);
        double g = mean + std_dev * nrmldis;
        int sim = round(g);
        stud[k].attr[t] = sim;
        }


        //implementation of the friends, difficult...
         int num_friends = rand() % 4; // Number of friends (0 to 3)
        for (int f = 0; f < num_friends; f++) {
            int rand_stud;

            // Find a random student who is not already a friend and is not self
            do {
                rand_stud = rand() % NUM_STUD;
            } while (rand_stud == k || 
                     stud[k].attr[FRIEND1] == rand_stud || 
                     stud[k].attr[FRIEND2] == rand_stud || 
                     stud[k].attr[FRIEND3] == rand_stud);

            // Add friend bidirectionally
            int added_to_k = 0; // Use integers to simulate boolean (0 = false, 1 = true)
            int added_to_rand_stud = 0;

            // Add rand_stud to k's friends
            for (int i = FRIEND1; i <= FRIEND3; i++) {
                if (stud[k].attr[i] == NUM_STUD) {
                    stud[k].attr[i] = rand_stud;
                    added_to_k = 1;
                    break;
                }
            }

            // Add k to rand_stud's friends
            for (int i = FRIEND1; i <= FRIEND3; i++) {
                if (stud[rand_stud].attr[i] == NUM_STUD) {
                    stud[rand_stud].attr[i] = k;
                    added_to_rand_stud = 1;
                    break;
                }
            }

            // If only one side was added, remove the incomplete friendship
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
    int i, c;

    // Reset class sizes
    for (c = 0; c < NUM_CLASSES; c++) {
        class_size[c] = 0;
    }

    // Initial random assignment
    for (i = 0; i < NUM_STUD; i++) {
        int assigned_class = rand() % NUM_CLASSES;
        stud[i].attr[CLASS_VALUE] = assigned_class;
        class_size[assigned_class]++;
    }

    int all_inBound = 0;
    int max_iterations = 10000;
    int iteration = 0;

    while (!all_inBound && iteration < max_iterations) {
        iteration++;

        // Check if all classes are within bounds
        all_inBound = 1;
        int min_class = 0, max_class = 0;

        for (c = 0; c < NUM_CLASSES; c++) {
            if (class_size[c] < MIN_CLASS || class_size[c] > MAX_CLASS) {
                all_inBound = 0;
            }
            if (class_size[c] < class_size[min_class]) min_class = c;
            if (class_size[c] > class_size[max_class]) max_class = c;
        }

        if (all_inBound) break; // All balanced!

        // Instead of breaking if max_class is not over MAX_CLASS,
        // we must allow moving students even if max_class is not overfilled,
        // because some classes may be underfilled.

        // But moving student from max_class to min_class only makes sense if max_class > min_class
        if (class_size[max_class] > class_size[min_class]) {
            // Find a student in max_class to move
            for (i = 0; i < NUM_STUD; i++) {
                if (stud[i].attr[CLASS_VALUE] == max_class) {
                    stud[i].attr[CLASS_VALUE] = min_class;
                    class_size[max_class]--;
                    class_size[min_class]++;
                    break; // Only move one student per iteration
                }
            }
        } else {
            // Can't fix by moving students (all classes are equal size), break to avoid infinite loop
            break;
        }
    }

    if (iteration == max_iterations) {
        printf("Warning: Could not balance classes within bounds after %d iterations.\n", max_iterations);
    }
}
    


void displaySol(individual cand, int class_size[]) {
    int i, j,g ;
 /*  for (i = 0; i < NUM_STUD; i++) {
        printf("Student %d: ", i );
        for (j = 0; j < NUM_ATTRIBUTES; j++) {
            printf("%d ", cand.genome[i][j]);
        }
        printf("\n");
    }
  */
 
    for (g = 0; g < NUM_CLASSES; g++) {
    //printf("Number of children in class %d: %d \n", g , class_size[g]);
        
    }


   // printf("-------------------------------------------");
   // printf(" \n");
   
 // printf(" \n");
   // printf(" \n");
    
}

void calculateMeanAndStdDev(int attribute, individual *cand, double *meanOverall, double *sdOverall, double *meanClass, double *sdClass) {
    double sum = 0.0;
    double varianceSum = 0.0;

    // Calculate overall mean
    for (int i = 0; i < NUM_STUD; i++) {
        sum += cand->genome[i][attribute];
    }
    *meanOverall = sum / NUM_STUD;

    // Calculate overall standard deviation
    for (int i = 0; i < NUM_STUD; i++) {
        varianceSum += pow(cand->genome[i][attribute] - *meanOverall, 2);
    }
    *sdOverall = sqrt(varianceSum / NUM_STUD);

    // Calculate mean and standard deviation for each class
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

    // Calculate metrics for all attributes
    for (int i = 0; i < 4; i++) {
        calculateMeanAndStdDev(attributes[i], cand, &meanOverall[i], &sdOverall[i], meanClass[i], sdClass[i]);
    }

    // Partial fitness
    double partFitness[4] = {0.0};
    for (int i = 0; i < 4; i++) {
        double fitness = 0.0;
        for (int p = 0; p < NUM_CLASSES; p++) {
            fitness += fabs(meanClass[i][p] - meanOverall[i]) + fabs(sdClass[i][p] - sdOverall[i]);
        }
        partFitness[i] = fitness;
        cand->fitness_values[i] = fitness;
    }




/*Even though there is a min class and max class size it is not beneficial for a school and the teachers if
one class has 14 and the other 30 children in the class, therefore it should be considered in the fitness how much each build class
size is deviated from the mean class size*/
double mean_classsize = NUM_STUD/NUM_CLASSES;
double dev_classsize = 0.0;
for (int t =0; t< NUM_CLASSES; t++){
    dev_classsize = dev_classsize + sqrt((mean_classsize- cand->class_sizes[t])*(mean_classsize- cand->class_sizes[t])/NUM_CLASSES);
}
cand->fitness_values[4] = dev_classsize;


/*For the friendships there is an ideal value of 0 which can be accomplished when all students have 3 friend entered
and all 3 friend of all students are in a class together. The reason for this is to accomplish a multi.objective fitness
were ideally all fitness values are 0*/

    
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
                //printf("Class count %d: %d \n", a, classStudNumbers[a][count]);
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
    cand->fitness_values[5] = friendCount;
}
void calcSIFitness(individual archcand[POP_SIZE+POP_SIZE],individual candall[POP_SIZE]){
    for (int i = 0; i < POP_SIZE; i++) {
        candall[i].fitness_si = 0;
        for (int j = 0; j < POP_SIZE; j++) {
            if (j != i){
                candall[i].fitness_si += dominates(&candall[i], &candall[j]);
            }
        }
        for (int j = 0; j < POP_SIZE + POP_SIZE; j++) {
            if (archcand[j].valid == 1){
                candall[i].fitness_si += dominates(&candall[i], &archcand[j]);
            }
        }
    }
}

void calcSPEAFitness(int archive_boolean, individual *cand, individual archcand[POP_SIZE+POP_SIZE],individual candall[POP_SIZE]){//boolean: if candidate part of archive = 1




/*_____________________________________________________________________________________________________
Ri raw fitness is the sum of Si of individual j that dominate i
______________________________________________________________________________________________________*/
    cand->fitness_raw = 0;
    cand->overall_fitness = 0;

    for (int j = 0; j < POP_SIZE; j++){

        if (j == cand->cand_num && archive_boolean == 1){
            continue;
        }
        
        if (dominates(&candall[j], cand) == 1){
             printf("CAND %d is dominating \n", j);
            
            cand->fitness_raw = cand->fitness_raw+candall[j].fitness_si;
           
        }
    
    }
        

    for (int j = 0; j < POP_SIZE+POP_SIZE; j++){
        if (j == cand->cand_num && archive_boolean == 0){
            continue;
        }
        if (archcand[j].valid == 1){
 
            if (dominates(&archcand[j], cand) == 1){
                printf("ARCHCAND %d is dominating \n", j);
                cand->fitness_raw = cand->fitness_raw+archcand[j].fitness_si;
            }
            
        }    
        
    }




/*_________________________________________________________________________________________________
CREATE FITNESS DISTANCE
___________________________________________________________________________________________________*/
   double kdoub = round(sqrt(POP_SIZE+POP_SIZE));

    int k = (int)kdoub + 1;

    for (int j = 0; j < POP_SIZE+POP_SIZE; j++){
        cand->array_distance[j]=-1;
    }
    for (int j = 0; j < POP_SIZE; j++){
        if (j == cand->cand_num){
            cand->array_distance[j] = 0;
            continue;    
        }
        else{
            cand->array_distance[j] = 0;
            cand->array_distance[j] = euclideanDistance(cand, &candall[j]);  
        }
        
        }
        
        
    

    int j_plus = POP_SIZE;
    for (int m = 0; m < POP_SIZE+POP_SIZE; m++){
        
        if (archcand[m].valid == 1){
                cand->array_distance[j_plus] = 0;
                cand->array_distance[j_plus] = euclideanDistance(cand, &archcand[m]);
                j_plus++;
        
        }
    
    
    }
        

    
    double max_distance = findMaximum(cand->array_distance, POP_SIZE+POP_SIZE);
    for (int b = 0; b < POP_SIZE+POP_SIZE; b++){
        if (cand->array_distance[b] == -1){
            cand->array_distance[b] = max_distance+1;
        }
    }
    sort_array(cand->array_distance, POP_SIZE+POP_SIZE);
    cand->density_value = 1/(cand->array_distance[k]+2);
    cand->overall_fitness = cand->fitness_raw + cand->density_value;
}

/*
=========================================================================================================
Next step is to create an archive of the best candidates and replace them with new candidates
The archive is as big as POP_SIZE
The first archive P'_0 is empty
The first place of the archive is filled with the candidates of the P_0 which are not dominated (overall_fitness<1)
After that it is filled with candidates that are dominated if the archive is not full
======================================================================================================

*/

/*
CREATE EMPTY ARCHIVE Description: This function initializes an archive of
candidates where the value for valid is 0 for false Additionally the overall
length of the Archive should be POP_SIZE, the problem is that for later steps
(Trunction) of the archive the archive is "overfilled" with candidates and then
reduced to a certain size, therefore the archive is initialized with
POP_SIZE+POP_SIZE candidates This is done to avoid problems with the trunction
process later on
*/

void createEmptyArchive(individual archcand[POP_SIZE+POP_SIZE]){
    for (int i = 0; i < POP_SIZE+POP_SIZE; i++) { 
            archcand[i].valid = 0; // Initialize all candidates in the archive as invalid
        }
       
    }



void replaceArchive(individual cand[POP_SIZE], individual archcand[POP_SIZE]){
    int archnum = 0; // needed to go through the archive and replace candidates 
    int archive_size = POP_SIZE; // Size of the archive, should be POP_SIZE
    int archive_count = 0;
/*------------------------------------------------------------------------
For all candidates in the population do the following:
1. Find If candidate is dominated or not
--> If the candidate is not dominated, check if the archive is full or there is a candidate in the archive that is dominated
--> Archive is full and there are the candates are not dominated add the candidate to the archive but count the oversize
2. If the candidate is dominated, check if the archive is full or there is a candidate in the archive that has a higher overall fitness
--> Replace the candidate in the archive with the dominated candidate if there is a candidate with a higher overall fitness
--------------------------------------------------------------------------*/
    
   for (int j = 0; j < POP_SIZE+POP_SIZE; j++) {
            archive_count = archive_count + archcand[j].valid; // Count valid candidates in the archive
        }
    for (int i = 0; i < POP_SIZE; i++) {
        if (archive_count < POP_SIZE) { // If the archive is not full
                for (int z = 0; z < POP_SIZE+POP_SIZE; z++) {
                    if (archcand[z].valid == 0) { // If there is an empty space in the archive
                        archcand[z] = cand[i]; // Add candidate to archive
                        archcand[z].valid = 1; // Mark candidate as valid
                        archive_count++; // Increase archive count
                        z = POP_SIZE+POP_SIZE; // Exit the loop after adding the candidate
                    }
                }
            
        }
        else {  // If the archive is full 
            if (cand[i].overall_fitness < 1) { // If the candidate is not dominated
                int case1 = 0; // Flag to check if candidate is added to archive
                
                for (int y = 0; y < POP_SIZE+POP_SIZE; y++) {
                
                    if (archcand[y].valid == 1 &&  archcand[y].overall_fitness >= 1){
                        archcand[y] = cand[i]; // Add candidate to archive if it is empty or dominated
                        y = POP_SIZE+POP_SIZE; // Exit the loop after adding the candidate
                        case1 = 1;
                    }
                    
                }
                if (case1 == 0) { // If candidate is not added to archive
                    for (int y = 0; y < POP_SIZE+POP_SIZE; y++) {
                        if (archcand[y].valid == 0) {
                            archcand[y] = cand[i]; // Add candidate to archive if it is empty
                             // Exit the loop after adding the candidate
                            archcand[y].valid = 1; // Mark candidate as valid
                            archive_count++;
                            case1 = 1; // Mark candidate as added to archive
                            y = POP_SIZE+POP_SIZE;
                        }
                    }


                }
                    
            }
            else{ // If candidate is dominated
                for (int y = 0; y < POP_SIZE+POP_SIZE; y++){
                    if (archcand[y].valid == 1 && archcand[y].overall_fitness > cand[i].overall_fitness) { // If there is a candidate in the archive with a higher overall fitness
                        archcand[y] = cand[i]; // Replace candidate in archive if it is dominated
                         // Exit the loop after replacing the candidate
                        archcand[y].valid = 1; // Mark candidate as valid
                        y = POP_SIZE+POP_SIZE;
                    }
                }
                

            }
            
        }
    }


/*------------------------------------------------------------------------------------------------------
Trunction process: Needed if overall size of the archive is bigger than POP_SIZE

-----------------------------------------------------------------------------------------------------*/
   
 printf("ARCHIVE COUNTr %d \n",archive_count);
while (archive_count > POP_SIZE) {
    //In this case find the distances of the archive candidates to each other
        for (int k = 0; k < POP_SIZE+POP_SIZE; k++) {
            for(int j = 0; j < POP_SIZE+POP_SIZE; j++) {
                archcand[k].array_distance[j] = -1; // Initialize array_distance to -1 to indicate not calculated yet
            }
        }
        for(int z = 0; z < POP_SIZE+POP_SIZE; z++){
            double max_distance = 10000000000000000;
            if (archcand[z].valid == 1){
                for (int j = 0; j < POP_SIZE+POP_SIZE; j++){
                    if (archcand[j].valid == 1 && j != z) {
                        archcand[z].array_distance[j] = euclideanDistance(&archcand[z], &archcand[j]); // Calculate distance between archive candidates
                    }      

                }
                max_distance = findMaximum(archcand[z].array_distance, POP_SIZE+POP_SIZE); // Find maximum distance in the array 
            }
            
            for (int j = 0; j < POP_SIZE+POP_SIZE; j++){
                if(archcand[z].array_distance[j] < 0) {   
                        archcand[z].array_distance[j] = max_distance+1; // Set negative distances to maximum distance
                }
            }
            // Sort the array_distance for each candidate
            sort_array(archcand[z].array_distance, POP_SIZE+POP_SIZE);
        }
        double minimal_distance_array[POP_SIZE+POP_SIZE];
        for(int z = 0; z < POP_SIZE+POP_SIZE; z++){
                minimal_distance_array[z]   = archcand[z].array_distance[0];   
        }
        sort_array(minimal_distance_array, POP_SIZE+POP_SIZE);
        double global_min = minimal_distance_array[0];
        
        // Identify arrays with that global min
        int candidates[POP_SIZE+POP_SIZE]; // Array to store indices of candidates with global minimum distance
        int candidate_count = 0;
        for (int i = 0; i < POP_SIZE+POP_SIZE; i++) {
            if (archcand[i].array_distance[0] == global_min) {
                candidates[candidate_count] = i;
                candidate_count++;
            }
        }
        
        if (candidate_count == 1) {
            archcand[candidates[0]].valid = 0; // Mark candidate as invalid
             printf("REMOVE ARCHCAND %d \n",candidates[0]);
            archive_count--; // Decrease archive size
        }
        else if(candidate_count > 1){
            int best_index = candidates[0];
            for (int i = 1; i < candidate_count; i++) {
                int cmp = compareArrays(archcand[candidates[i]].array_distance, archcand[best_index].array_distance, POP_SIZE+POP_SIZE);
                if (cmp < 0) {
                    best_index = candidates[i];
                }
            }
            // Replace the candidate with the best index
             printf("REMOVE ARCHCAND %d \n",best_index);
            archcand[best_index].valid = 0; // Mark candidate as invalid
            archive_count--; // Decrease archive size
        }
        
    }

 printf("ARCHIVE COUNTr AFTER %d \n",archive_count);   
}
                
                    

int tournamentSelection(individual *rival1, individual *rival2) {
    double prob = (double)rand() / RAND_MAX; /* Generate a random number between 0 and 1 */
    int best;
    //printf("rival 1 number %d \n",rival1->cand_num);
    //printf("rival 2 number %d \n",rival2->cand_num);

    if (prob >= TOURNM_PROP) { /* 95% chance to select the stronger candidate */
       if (rival1->overall_fitness > rival2->overall_fitness) {
            best = rival2->cand_num;
        } else {
            best = rival1->cand_num;
        }
    } else { /* 5% chance to select the weaker candidate */
        prob = (double)rand() / RAND_MAX;
        if (prob >= TOURNM_PROP) {
            best = rival1->cand_num;
       } else {
            best = rival2->cand_num;
        }
    }

    return best;
}
/*
void createOffspring(individual offspring[2], individual cand[POP_SIZE]) {
 /*
Function: createOffspring
Description: This function generates two offspring using genetic algorithm operators: tournament selection, crossover, and mutation. 
The offspring compete with their parents to potentially replace them in the population based on fitness and constraints.

Parameters:
- offspring: An array of two individuals to store the newly created offspring.
 - cand: The population of candidates, represented as an array of individuals
*/   
 /*   int pot_parent0[2], pot_parent1[2], parent[2]; // Arrays for potential parents and final selected parents.
 /*   int z, t, g;

    // Select two potential parents for each offspring using random indices
    for (z = 0; z < 2; z++) {
        pot_parent0[z] = rand() % POP_SIZE;
        pot_parent1[z] = rand() % POP_SIZE;
    }
     // Ensure the two selected parents are different
    while (pot_parent0[0] == pot_parent0[1]) {pot_parent0[1] = rand() % POP_SIZE;}
    while (pot_parent1[0] == pot_parent1[1]) {pot_parent1[1] = rand() % POP_SIZE;}

    // TOURNAMENT SELECTION: Select the final parents based on fitness
    parent[0] = tournamentSelection(&cand[pot_parent0[0]], &cand[pot_parent0[1]]);
    parent[1] = tournamentSelection(&cand[pot_parent1[0]], &cand[pot_parent1[1]]);

    // CROSSOVER: Create offspring genomes by combining parts of the parents
    int crossover = rand() % NUM_STUD; // Random crossover point
    for (t = 0; t < crossover; t++) {
        offspring[0].genome[t][CLASS_VALUE] = cand[parent[0]].genome[t][CLASS_VALUE];
        offspring[1].genome[t][CLASS_VALUE] = cand[parent[1]].genome[t][CLASS_VALUE];
    }
    for (t = crossover; t < NUM_STUD; t++) {
        offspring[0].genome[t][CLASS_VALUE] = cand[parent[1]].genome[t][CLASS_VALUE];
        offspring[1].genome[t][CLASS_VALUE] = cand[parent[0]].genome[t][CLASS_VALUE];
    }

    // MUTATION: Introduce random mutations into the offspring genomes
    int mutation[NUM_MUTATION];
    for(g = 0; g < NUM_MUTATION; g++) {
        mutation[g] = rand() % NUM_STUD; // Random gene index for mutation
        offspring[0].genome[mutation[g]][CLASS_VALUE] = rand() % NUM_CLASSES; // New random class value
        offspring[1].genome[mutation[g]][CLASS_VALUE] = rand() % NUM_CLASSES;
    }

    // CALCULATE FITNESS: Compute the fitness of the offspring
    calcFitness(&offspring[0]);
    calcFitness(&offspring[1]);

    // VALIDATION: Check if offspring satisfy class size constraints
    int i, class_valid0 = 1, class_valid1 = 1;
    for(i = 0; i < NUM_CLASSES; i++) {
        if (offspring[0].class_sizes[i] < MIN_CLASS || offspring[0].class_sizes[i] > MAX_CLASS) {
            class_valid0 = 0;
        }
    }
    for (i = 0; i < NUM_CLASSES; i++) {
        //printf("class size %d \n" ,offspring[1].class_sizes[i]);
        if (offspring[1].class_sizes[i] < MIN_CLASS || offspring[1].class_sizes[i] > MAX_CLASS) {
            class_valid1 = 0;
        }
    }


    // REPLACEMENT: Replace parents with offspring if offspring are valid and fitter
    if (class_valid0 == 1) {
        int besti = tournamentSelection(&cand[parent[0]], &offspring[0]);
        //printf("bestpp: %d \n",besti);
         if (besti >= NUM_STUD){
            offspring[0].cand_num = cand[parent[0]].cand_num;
            //printf("Choose Offspring, for parent %d, fitness off = %lf; fitness parent = %lf \n ",parent[0],offspring[0].overall_fitness,cand[parent[0]].overall_fitness);
        
            for(int i =0; i<NUM_ATTRIBUTES; i++){
                for (int j = 0; j < NUM_STUD; j++)
                {
                    cand[parent[0]].genome[j][i] = offspring[0].genome[j][i] ;
                }
                


            }
            cand[parent[0]].overall_fitness = offspring[0].overall_fitness;
            offspring[0].cand_num = NUM_STUD;
            for (int z = 0; z < NUM_CLASSES; z++){
                 cand[parent[0]].class_sizes[z] = offspring[0].class_sizes[z];
            }
            
        }
        
    
    }
    if (class_valid1 == 1) {
        int besti = tournamentSelection(&cand[parent[1]], &offspring[1]);
        //printf("bestp2: %d \n",besti);
        if (besti >= NUM_STUD){
            offspring[1].cand_num = cand[parent[1]].cand_num;
            //printf("Choose Offspring for parent %d, fitness off = %lf; fitness parent = %lf \n",parent[1],offspring[1].overall_fitness, cand[parent[1]].overall_fitness);
                for(int i =0; i<NUM_ATTRIBUTES; i++){
                for (int j = 0; j < NUM_STUD; j++)
                {
                    cand[parent[1]].genome[j][i] = offspring[1].genome[j][i] ;
                }
            }
            cand[parent[1]].overall_fitness = offspring[1].overall_fitness;
            for (int z = 0; z < NUM_CLASSES; z++){
                 cand[parent[0]].class_sizes[z] = offspring[0].class_sizes[z];
            }
            offspring[1].cand_num = NUM_STUD+1;
        
        }
        
    }
}

*/
void sort_array(double* array, int size) {
    int i, j;
    double temp;
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                // Swap
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}


double euclideanDistance(individual *cand1, individual *cand2) {
    double distance = 0.0;
    double count[6]; // Array to hold squared differences for each fitness value
    for (int u = 0; u < 6; u++){
        count[u] = (cand1->fitness_values[u] - cand2->fitness_values[u])*(cand1->fitness_values[u] - cand2->fitness_values[u]);
        distance = distance + count[u];
        
    }
    distance = sqrt(distance);
    return distance;
}

double findMaximum(const double arr[], int size) {

    double max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// Compare two arrays lexicographically
int compareArrays(double a[], double b[], int size) {
    for (int i = 0; i < size; i++) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
    }
    return 0; // Arrays are identical
}

int dominates(individual *a, individual *b) {
    int better_or_equal = 0;
    int strictly_better = 0;

    for (int i = 0; i < 6; i++) {
        if (b->fitness_values[i] >= a->fitness_values[i]) {
            better_or_equal++;
            if (b->fitness_values[i] > a->fitness_values[i]) {
                strictly_better = 1;
            }
        }
    }

    // b dominates a if it's no worse in all and better in at least one
    return (better_or_equal == 6 && strictly_better);
}
