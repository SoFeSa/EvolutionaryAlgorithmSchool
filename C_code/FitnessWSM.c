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
#define POP_SIZE 6

//defines the maximal and minimal number of children allowed in one class
#define MIN_CLASS 2
#define MAX_CLASS 4


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
#define NUM_STUD 10


#define NUM_ATTRIBUTES 9

#define NUM_CLASSES 3


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

int main(void) {
    student stud[NUM_STUD];
    createStudents(stud); // Initialize students

    individual cand[POP_SIZE];
    for (int numcand = 0; numcand < POP_SIZE; numcand++) {
        createClasses(stud, cand[numcand].class_sizes);

        for (int i = 0; i < NUM_STUD; i++) {
            for (int j = 0; j < NUM_ATTRIBUTES; j++) {
                cand[numcand].genome[i][j] = stud[i].attr[j];
            }
        }
        displaySol(cand[numcand], cand[numcand].class_sizes);
        calcFitness(&cand[numcand]);
    }
    return EXIT_SUCCESS;
}

/*******************************/
/*Create students with their predefinded ramdomly chosen values*/
/*First Test includes 8 students:
1. Stud_Num (0),Female (0), Hyperact (3), Logskill (2), LanguageSkill(4), Friend1(3), 
2. Stud_Num (1),Male (1), Hyperact (4), Logskill (3), LanguageSkill(1), Friend1(4)
3. Stud_Num (2),Male (1), Hyperact (1), Logskill (4), LanguageSkill(3),
4. Stud_Num (3),Female (0), Hyperact (0), Logskill (3), LanguageSkill(4), Friend1(0)
5. Stud_Num (4),Male (1), Hyperact (5), Logskill (3), LanguageSkill(0), Friend1(1)
6. Stud_Num (5),Male (1), Hyperact (2), Logskill (3), LanguageSkill(3), Friend1(6),Friend2(7),Friend3(2)
7. Stud_Num (6),Female (0), Hyperact (4), Logskill (0), LanguageSkill(2), Friend1(5),Friend2(7)
8. Stud_Num (7),Male (1), Hyperact (0), Logskill (3), LanguageSkill(1),Friend1(5),Friend2(6)
*/
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
        double nrmldis = sqrt(-2 * log(j)) * cos(2 * pi * f);
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

    // Calculate normalized class sizes
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
        for (i = 0; i < 20; i++) {
            int rand_swap1 = rand() % NUM_CLASSES;
            int rand_swap2 = rand() % NUM_CLASSES;
            class_size[rand_swap1]++;
            class_size[rand_swap2]--;
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

        while (class_size_count[class_num] == 0) {
            class_num = rand() % NUM_CLASSES; // Fixed shadowing issue
        }

        if (class_size_count[class_num] > 0) {
            stud[g].attr[CLASS_VALUE] = class_num;
            class_size_count[class_num]--;
        }
    }
}

void displaySol(individual cand, int class_size[]) {
    int i, j,g ;
    for (i = 0; i < NUM_STUD; i++) {
        printf("Student %d: ", i );
        for (j = 0; j < NUM_ATTRIBUTES; j++) {
            printf("%d ", cand.genome[i][j]);
        }
        printf("\n");
    }
    for (g = 0; g < NUM_CLASSES; g++) {
    printf("Number of children in class %d: %d \n", g , class_size[g]);
        
    }
    
}



void calcFitness(individual *cand){
/*A reference value is needed to calculate the fitness, as it is important to calculate the difference
of the mean of the value sex, hyperact, logskill, langskill of all students and the students of each class*/
    int u;
    double z = 0;
    double k = 0;
    for (u = 0; u < NUM_STUD; u++) {
        z = z + cand->genome[u][SEX];

    }
    double meansex = z/NUM_STUD;
    for (u = 0; u < NUM_STUD; u++) {
        k = k + ((meansex - cand->genome[u][SEX])*(meansex - cand->genome[u][SEX]));

    }
    double sdsex = sqrt(k/NUM_STUD);



    printf("Mean SEX: %lf \n", meansex);
    z = 0;
    k = 0;
    for (u = 0; u < NUM_STUD; u++) {
        z = z + cand->genome[u][HYPERACT];
    }
    double meanhyp = z/NUM_STUD;
    for (u = 0; u < NUM_STUD; u++) {
        k = k + ((meanhyp - cand->genome[u][HYPERACT])*(meanhyp - cand->genome[u][HYPERACT]));

    }
    double sdhyp = sqrt(k/NUM_STUD);
    
    printf("Mean Hyperact: %lf \n", meanhyp);

    z = 0;
    k = 0;
    for (u = 0; u < NUM_STUD; u++) {
        z = z + cand->genome[u][LOGSKILL];
    }
    double meanlogskill = z/NUM_STUD;
    for (u = 0; u < NUM_STUD; u++) {
        k = k + ((meanlogskill - cand->genome[u][LOGSKILL])*(meanlogskill - cand->genome[u][LOGSKILL]));

    }
    double sdlogskill = sqrt(k/NUM_STUD);
    printf("Mean Logical Skill: %lf \n", meanlogskill);
    


    z = 0;
    k = 0;
    for (u = 0; u < NUM_STUD; u++) {
        z = z + cand->genome[u][LANGUAGESKILL];
    }
    double meanlangkill = z/NUM_STUD;
    for (u = 0; u < NUM_STUD; u++) {
        k = k + ((meanlangkill - cand->genome[u][LANGUAGESKILL])*(meanlangkill - cand->genome[u][LANGUAGESKILL]));

    }
    double sdlangkill = sqrt(k/NUM_STUD);

    printf("Mean Language Skill: %lf \n", meanlangkill);

/*next step is to calculate the mean values for each class and the overall fitness for the cnand*/
   int count;
//Calc for SEX
    double m = 0;
    k = 0;
    int l;
    double sdSexClass[NUM_CLASSES]; 
    double meansexClass[NUM_CLASSES];
    double fitsexClass[NUM_CLASSES];
    for (l = 0;l<NUM_CLASSES;l++){
        for (u = 0; u < NUM_STUD; u++) {
            if (cand->genome[u][CLASS_VALUE]==l){
                m = m + cand->genome[u][SEX];
            }
        meansexClass[l] = m / cand->class_sizes[l];  
        for (u = 0; u < NUM_STUD; u++) {
            if (cand->genome[u][CLASS_VALUE]==l){
                k = k + ((meansexClass[l] - cand->genome[u][SEX])*(meansexClass[l] - cand->genome[u][SEX]));
            } 
        sdSexClass[l] = sqrt(k/cand->class_sizes[l]);  
        }
    }
    double overall_fitSex = 0;
    for (l= 0; l<NUM_CLASSES; l++){
        fitsexClass[l] = fabs(meansexClass[l]-meansex) + fabs(sdSexClass[l]-sdsex);
        printf("Mean Class %d: %lf \n", l, meansexClass[l]);
        printf("Fitness Class %d: %lf \n", l, fitsexClass[l]);
        overall_fitSex = overall_fitSex + fitsexClass[l];
    }
    printf("Fitness SEX: %lf \n",  overall_fitSex);
    }
}
//Calc for HYPERACT
 /*   m = 0;
    k = 0;
    double meanhypClass[NUM_CLASSES];
    double fithypClass[NUM_CLASSES];
    double fitHyp = 0; 
    for (u = 0; u < cand->classbreak[k]; u++) {
            m = m + cand->genome[u][HYPERACT];
        }
    meanhypClass[k] = m/cand->classbreak[k];
    fithypClass[k] = fabs(meanhypClass[k]-meanhyp);
    printf("Mean Class %d: %lf \n", k, meanhypClass[k]);
    printf("Fitness Class %d: %lf \n", k, fithypClass[k]);

    count = cand->classbreak[k];
    for (k = 1; k < NUM_CLASSES; k++){
    for (u = count; u < count+cand->classbreak[k]; u++) {
            m = m + cand->genome[u][HYPERACT];
        }
    count = count+cand->classbreak[k];  
    meanhypClass[k] = m/cand->classbreak[k];
    fithypClass[k] = fabs(meanhypClass[k]-meanhyp);
    printf("Mean Class %d: %lf \n", k, meanhypClass[k]);
    printf("Fitness Class %d: %lf \n", k, fithypClass[k]);  
    m = 0;
    }
    for (u = 0; u < NUM_CLASSES; u++) {
            fitHyp = fitHyp + fithypClass[u];
        }
    printf("overall Fitness Class: %lf \n", fitHyp);


//CALC for LOGSKILL 
    k = 0;
    double meanlogskillClass[NUM_CLASSES];
    double fitlogskillClass[NUM_CLASSES];
    double fitLogskill = 0; 
    for (u = 0; u < cand->classbreak[k]; u++) {
            m = m + cand->genome[u][LOGSKILL];
        }
    meanlogskillClass[k] = m/cand->classbreak[k];
    fitlogskillClass[k] = fabs(meanlogskillClass[k]-meanlogskill);
    printf("Mean Class %d: %lf \n", k, meanlogskillClass[k]);
    printf("Fitness Class %d: %lf \n", k, fitlogskillClass[k]);

    count = cand->classbreak[k];
    for (k = 1; k < NUM_CLASSES; k++){
    for (u = count; u < count+cand->classbreak[k]; u++) {
            m = m + cand->genome[u][LOGSKILL];
        }
    count = count+cand->classbreak[k];  
    meanlogskillClass[k] = m/cand->classbreak[k];
    fitlogskillClass[k] = fabs(meanlogskillClass[k]-meanlogskill);
    printf("Mean Class %d: %lf \n", k, meanlogskillClass[k]);
    printf("Fitness Class %d: %lf \n", k, fitlogskillClass[k]);  
    m = 0;
    }
    for (u = 0; u < NUM_CLASSES; u++) {
            fitLogskill = fitLogskill + fitlogskillClass[u];
        }
    printf("overall Fitness Class: %lf \n", fitLogskill); 
    

//CALC for LANGUAGESKILL
   k = 0;
    double meanlangskillClass[NUM_CLASSES];  
    double fitlangkillClass[NUM_CLASSES];
    double fitLangskill = 0; 
    for (u = 0; u < cand->classbreak[k]; u++) {
            m = m + cand->genome[u][LOGSKILL];
        }
    meanlangskillClass[k] = m/cand->classbreak[k];
    fitlangkillClass[k] = fabs(meanlangskillClass[k]-meanlangkill);
    printf("Mean Class %d: %lf \n", k, meanlangskillClass[k]);
    printf("Fitness Class %d: %lf \n", k, fitlangkillClass[k]);

    count = cand->classbreak[k];
    for (k = 1; k < NUM_CLASSES; k++){
    for (u = count; u < count+cand->classbreak[k]; u++) {
            m = m + cand->genome[u][LOGSKILL];
        }
    count = count+cand->classbreak[k];  
    meanlangskillClass[k] = m/cand->classbreak[k];
    fitlangkillClass[k] = fabs(meanlangskillClass[k]-meanlangkill);
    printf("Mean Class %d: %lf \n", k, meanlangskillClass[k]);
    printf("Fitness Class %d: %lf \n", k, fitlangkillClass[k]);  
    m = 0;
    }
    for (u = 0; u < NUM_CLASSES; u++) {
            fitLangskill = fitLangskill + fitlangkillClass[u];
        }
    printf("overall Fitness Class: %lf \n", fitLangskill); 

/*For the friendships there is an ideal value of 0 which can be accomplished when all students have 3 friend entered
and all 3 friend of all students are in a class together. The reason for this is to accomplish a multi.objective fitness
were ideally all fitness values are 0*/
/*    int allfriend = NUM_STUD*3;
    printf("overall Fitness Friends Class: %d \n", allfriend); 
    int fitfriendsclass[NUM_CLASSES];
 // Loop to calculate fitness for each class
    int classcount = 0;
    int q,i,x,j;
    for (q = 0; q < NUM_CLASSES; q++) {
        fitfriendsclass[q] = allfriend;
        for (i = classcount; i < classcount + cand->classbreak[q]; i++) {
            for (x = FRIEND1; x <= FRIEND3; x++) {
                for (j = classcount; j < classcount + cand->classbreak[q]; j++) {
                    if (cand->genome[i][x] == cand->genome[j][STUD_NUM]) {
                        fitfriendsclass[q] = fitfriendsclass[q] - 1;
                    }
                }
            }
        }
        printf(" Fitness Friends Class %d: %d \n", q, fitfriendsclass[q]);
        classcount = classcount + cand->classbreak[q];
    }

}
*/