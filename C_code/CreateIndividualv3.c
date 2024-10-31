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
#define POP_SIZE 10

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
void displaySol(individual cand);/*shows the created candidate*/
void classDistr(int num_class, individual *cand);/*creates a class distribution for a candidate*/
void calcFitness(individual *cand); //calculates fitness for a candidate

int main(void) {
    student stud[NUM_STUD];
    createStudents(stud);

    int candperm[NUM_STUD];
    

    individual cand[POP_SIZE];
    int i, j, x, numcand;
    for(numcand = 0; numcand < POP_SIZE; numcand++){
        createRandPerm(candperm);
        for (i = 0; i < NUM_STUD; i++) {
            for (j = 0; j < NUM_ATTRIBUTES; j++) {
                int k = 0;
                while (candperm[i] != stud[k].attr[STUD_NUM]) {
                    k++;
                }

                if (candperm[i] == stud[k].attr[STUD_NUM]) {
                    cand[numcand].genome[i][j] = stud[k].attr[j];
                }
            }
        }
        classDistr(NUM_CLASSES, &cand[numcand]);
        displaySol(cand[numcand]);
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
    srand(1);
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

        for (t = 2; t < 5; t++){
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
        int num_friend = rand() % 4;//4 as there is a possibility of no friends
        
        int x = 5 + num_friend;
        int b;
        for (l = 5; l < x; l ++){
            int rand_stud = rand() % NUM_STUD;
            while (rand_stud == k) {
                 int rand_stud = rand() % NUM_STUD;
            }
            if (stud[k].attr[l] == NUM_STUD && stud[rand_stud].attr[5] == NUM_STUD){
                stud[k].attr[l] = rand_stud; 
                stud[rand_stud].attr[5] = k; 
    
            }
           if (stud[k].attr[l] == NUM_STUD && stud[rand_stud].attr[5] =! NUM_STUD){
                for (b = 6; b < 8; b++){
                    if (stud[k].attr[l] == NUM_STUD && stud[rand_stud].attr[b] == NUM_STUD){
                        stud[k].attr[l] = rand_stud; 
                        stud[rand_stud].attr[b] = k;
                        b == 9;
                    }
                }
                if (b =! 9){
                    int rand_stud_count;
                    for (rand_stud_count = rand_stud+1; rand_stud_count=!rand_stud;rand_stud_count++){
                        if (rand_stud_count == NUM_STUD){
                           rand_stud_count = 0; 
                        }
                        if (stud[rand_stud_count].attr[5] == NUM_STUD){
                            stud[k].attr[l] = rand_stud_count; 
                            stud[rand_stud_count].attr[5] = k;
                            rand_stud_count = rand_stud;
                        }
                        if (stud[rand_stud_count].attr[5] =! NUM_STUD){
                            int s;
                            for (s = 6; s < 8; s++){
                                if stud[k].attr[l] == NUM_STUD && stud[rand_stud_count].attr[s] == NUM_STUD{
                                    stud[k].attr[l] = rand_stud_count; 
                                    stud[rand_stud_count].attr[s] = k;
                                    s == 9;
                                    rand_stud_count = rand_stud;
                                }
                            }
                        }

                    }
                }
           

    
            }
            
           
        }

    }
        //implementation of classes
        int i,v,t,g;
        int class_count[NUM_CLASSES];
        for (i=0; i< NUM_CLASSES; i++){
            class_count[i] = 0;
        }
        while ()
        for (v=0; v< NUM_STUD; v++){
            int rand_class = rand() % NUM_CLASSES;
            stud[v].attr[CLASS_VALUE] = rand_class;
            class_count[rand_class]= class_count[rand_class]+1;
        }
        
            if (class_count[t] < MIN_CLASS && class_count[t] > MAX_CLASS){
                for (g=0; g< NUM_CLASSES; g++){
                    class_count[g] = 0;
                }
                for (v=0; v< NUM_STUD; v++){
                    int rand_class = rand() % NUM_CLASSES;
                    stud[v].attr[CLASS_VALUE] = rand_class;
                    class_count[rand_class]= class_count[rand_class]+1;
                }
            }
        
        
    return 0;
}

void createRandPerm(int *perm) {
    int i, j;
    int r;
    int avail[NUM_STUD] = {0, 1, 2, 3, 4, 5, 6, 7};

    for (i = NUM_STUD; i > 1; i--) {
        r = rand() % i;
        perm[i - 1] = avail[r];

        for (j = r; j < i - 1; j++)
            avail[j] = avail[j + 1];
    }
    perm[0] = avail[0];
}

void displaySol(individual cand) {
    int i, j,g,studcount ;
    for (i = 0; i < NUM_STUD; i++) {
        printf("Student %d: ", i + 1);
        for (j = 0; j < NUM_ATTRIBUTES; j++) {
            printf("%d ", cand.genome[i][j]);
        }
        printf("\n");
    }
    studcount  = 0;
    for (g = 0; g < NUM_CLASSES-1; g++) {
    printf("Number of children in class %d: %d \n", g , cand.classbreak[g]);
        studcount = studcount + cand.classbreak[g];
        
    }
    int r = NUM_STUD - studcount ;
    printf("Number of children in class %d: %d \n", g , r);
    printf("\n");
}

/*********************************/
/*
For the class distribution:
There are given values for max and min of students in a class in school systems
To enable some randomness of the class size a uniformly choosen size would be an option
which is probably not near to a approach of choosing the right size in the school system
therefore a normal distribution is used, where the triple of the standard deviation is not allowed to exceed the boundary values
and the mean is NUM_STUD/NUM_CLASSES
In the first version the boundaries should be 1 and 4 (standard deviation is 0.4467), the mean is 2.66 
to create a normal distribution out of uniformly random numbers (rand()) a box-muller-transformation is used
*/
/*********************************/
void classDistr(int num_class, individual *cand) {
    int a;
    double mean = (double)NUM_STUD / num_class;
    double std_dev = sqrt((double)NUM_STUD / num_class) / 3.0;

    for (a = 0; a < num_class - 1; a++) {
        double j, k;
        double pi = 3.14159265;
        j = (double)rand() / RAND_MAX;
        k = (double)rand() / RAND_MAX;
        double nrmldis = sqrt(-2 * log(j)) * cos(2 * pi * k);
        double f = mean + std_dev * nrmldis;
        int sim = round(f);

        // Ensure the class size is within bounds
        while (sim < MIN_CLASS || sim > MAX_CLASS){
            j = (double)rand() / RAND_MAX;
            k = (double)rand() / RAND_MAX;
            double nrmldis = sqrt(-2 * log(j)) * cos(2 * pi * k);
            double f = mean + std_dev * nrmldis;
            int sim = round(f);
        }

        cand->classbreak[a] = sim;
    }   
     // The size of the last class is determined to make the total match NUM_STUD
    int remaining_students = NUM_STUD;
    for (a = 0; a < num_class - 1; a++) {
        remaining_students -= cand->classbreak[a];
    }
    cand->classbreak[num_class - 1] = remaining_students;
}


void calcFitness(individual *cand){
/*A reference value is needed to calculate the fitness, as it is important to calculate the difference
of the mean of the value sex, hyperact, logskill, langskill of all students and the students of each class*/
    int u;
    double z = 0;
    for (u = 0; u < NUM_STUD; u++) {
        z = z + cand->genome[u][SEX];
    }
    double meansex = z/NUM_STUD;

    printf("Mean SEX: %lf \n", meansex);
    z = 0;
    for (u = 0; u < NUM_STUD; u++) {
        z = z + cand->genome[u][HYPERACT];
    }
    double meanhyp = z/NUM_STUD;
    printf("Mean Hyperact: %lf \n", meanhyp);

    z = 0;
    for (u = 0; u < NUM_STUD; u++) {
        z = z + cand->genome[u][LOGSKILL];
    }
    double meanlogskill = z/NUM_STUD;
    printf("Mean Logical Skill: %lf \n", meanlogskill);
    
    z = 0;
    for (u = 0; u < NUM_STUD; u++) {
        z = z + cand->genome[u][LANGUAGESKILL];
    }
    double meanlangkill = z/NUM_STUD;
    printf("Mean Language Skill: %lf \n", meanlangkill);

/*next step is to calculate the mean values for each class and the overall fitness for the cnand*/
    int count;
//Calc for SEX
    int k = 0;
    double m = 0;
    double meansexClass[NUM_CLASSES];
    double fitsexClass[NUM_CLASSES];
    for (u = 0; u < cand->classbreak[k]; u++) {
            m = m + cand->genome[u][SEX];
        }
    meansexClass[k] = m/cand->classbreak[k];
    fitsexClass[k] = fabs(meansexClass[k]-meansex);
    printf("Mean Class %d: %lf \n", k, meansexClass[k]);
    printf("Fitness Class %d: %lf \n", k, fitsexClass[k]);

    count = cand->classbreak[k];
    for (k = 1; k < NUM_CLASSES; k++){
    for (u = count; u < count+cand->classbreak[k]; u++) {
            m = m + cand->genome[u][SEX];
        }
    count = count+cand->classbreak[k];  
    meansexClass[k] = m/cand->classbreak[k];
    fitsexClass[k] = fabs(meansexClass[k]-meansex);
    printf("Mean Class %d: %lf \n", k, meansexClass[k]);
    printf("Fitness Class %d: %lf \n", k, fitsexClass[k]);  
    m = 0;
    }
    double fitSex = 0; 
    for (u = 0; u < NUM_CLASSES; u++) {
            fitSex = fitSex + fitsexClass[u];
        }
    printf("overall Fitness Class: %lf \n", fitSex);

//Calc for HYPERACT
    m = 0;
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
    int allfriend = NUM_STUD*3;
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