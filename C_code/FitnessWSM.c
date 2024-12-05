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

/*******************************/
/* definitions for readability */
/*******************************/
#define POP_SIZE 50
#define LOOP 1000
//defines the maximal and minimal number of children allowed in one class
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

//number of students
#define NUM_STUD 154


#define NUM_ATTRIBUTES 9

#define NUM_CLASSES 8

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
    double fitness_pop = 0.0;
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
        //printf("CANDIDATE %d \n \n", numcand );
        //displaySol(cand[numcand], cand[numcand].class_sizes);
        fitness_pop = fitness_pop + cand[numcand].overall_fitness;
    }
    fitness_pop = fitness_pop/POP_SIZE;

    printf("Mean Fitness of Population: %lf       ", fitness_pop );
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
    printf("BEST CAND of Population: %d with Fitness %lf \n ", best , cand[best].overall_fitness );
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
        
//EVOLUTIONARY LOOP
for (int i = 1; i < LOOP; i++){
    for (int p = 0; p < POP_SIZE; p++){
       
    
    
    createOffspring(offspring ,cand);}
    double fitness_pop = 0.0;
    for (int numcand = 0; numcand < POP_SIZE; numcand++){
        calcFitness(&cand[numcand]);
        //printf("CANDIDATE %d \n \n", numcand );
        //displaySol(cand[numcand], cand[numcand].class_sizes);
        
        fitness_pop = fitness_pop + cand[numcand].overall_fitness;

    }
    fitness_pop = fitness_pop/POP_SIZE;
    printf("LOOP: %d     ", i );
    printf("Mean Fitness of Population: %lf        ", fitness_pop );
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
for (int numcand = 0; numcand < POP_SIZE; numcand++){
    displaySol(cand[numcand], cand[numcand].class_sizes);
        
}
    return EXIT_SUCCESS;
}

/*******************************/
/*Create students with their predefinded ramdomly chosen values*/

/*******************************/

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
    int i, v, t, g;
    int norm_classsize = NUM_STUD / NUM_CLASSES;
    int left = NUM_STUD % NUM_CLASSES;

    // Initialize class sizes
    for (i = 0; i < NUM_CLASSES; i++) {
        class_size[i] = norm_classsize;
    }

    // Distribute the remaining students among classes
    i = 0;
    v = 0;
    while (v < left) {
        class_size[i]++;
        i++;
        v++;
    }

    // Ensure class sizes are within MIN_CLASS and MAX_CLASS constraints
    if (norm_classsize - 3 < MIN_CLASS && norm_classsize + 3 > MAX_CLASS) {
        // Swap students between classes if needed
        for (i = 0; i < 20; i++) {
            int rand_swap1 = rand() % NUM_CLASSES;
            int rand_swap2 = rand() % NUM_CLASSES;
            if (class_size[rand_swap1] > MIN_CLASS && class_size[rand_swap2] < MAX_CLASS) {
                class_size[rand_swap1]--;
                class_size[rand_swap2]++;
            }
        }

        // Validate constraints and reset if necessary
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

    // Assign students to classes
    int class_size_count[NUM_CLASSES];
    for (t = 0; t < NUM_CLASSES; t++) {
        class_size_count[t] = class_size[t];
    }

    for (g = 0; g < NUM_STUD; g++) {
        int class_num = rand() % NUM_CLASSES;

        // Ensure valid class assignment
        while (class_size_count[class_num] == 0) {
            class_num = rand() % NUM_CLASSES; // Keep trying until we find a valid class
        }

        // Assign the student to a class and decrement the class size count
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
   for (i = 0; i < NUM_STUD; i++) {
        //printf("Student %d: ", i );
        for (j = 8; j < NUM_ATTRIBUTES; j++) {
            printf("%d ", cand.genome[i][j]);
        }
        printf("\n");
    }
  
 printf("\n");
    for (g = 0; g < NUM_CLASSES; g++) {
    printf("Number of children in class %d: %d \n", g , class_size[g]);
        
    }

    printf("OVERALL FITNESS %lf \n", cand.overall_fitness);

    printf("-------------------------------------------");
    printf(" \n");
   
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
    }




/*Even though there is a min class and max class size it is not beneficial for a school and the teachers if
one class has 14 and the other 30 children in the class, therefore it should be considered in the fitness how much each build class
size is deviated from the mean class size*/
double mean_classsize = NUM_STUD/NUM_CLASSES;
double dev_classsize = 0.0;
for (int t =0; t< NUM_CLASSES; t++){
    dev_classsize = dev_classsize + sqrt((mean_classsize- cand->class_sizes[t])*(mean_classsize- cand->class_sizes[t])/NUM_CLASSES);
}



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
/*_____________________________________________________________________________________________________
The next step is to get the right weights for each partial fitness and sum it all up.
______________________________________________________________________________________________________*/

double weightSex = 4.0;
double weightHypLogLang = 1.0;
double weightFriends = 2.0/(NUM_STUD*3);
double weightClass = 4.0/17.0;//4/17
double weight = weightClass*dev_classsize;
double wsm_FITNESS = (weightSex*partFitness[0]+ weightHypLogLang* (partFitness[1]+partFitness[2]+partFitness[3])+ weightFriends*friendCount+weightClass*dev_classsize);

cand->overall_fitness = wsm_FITNESS;
 
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

void createOffspring(individual offspring[2], individual cand[POP_SIZE]) {
 /*
Function: createOffspring
Description: This function generates two offspring using genetic algorithm operators: tournament selection, crossover, and mutation. 
The offspring compete with their parents to potentially replace them in the population based on fitness and constraints.

Parameters:
- offspring: An array of two individuals to store the newly created offspring.
 - cand: The population of candidates, represented as an array of individuals
*/   
    int pot_parent0[2], pot_parent1[2], parent[2]; // Arrays for potential parents and final selected parents.
    int z, t, g;

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