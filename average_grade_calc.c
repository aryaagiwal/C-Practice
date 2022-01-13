#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_COURSE_LENGTH 40 

struct course {
    char *name;
    char *gpa;
    float course_average;
    int num_tests;
    int num_hw;
    int num_quizzes;
    unsigned short int hours;
};

float calc_course_gpa_float(float course_average);

int report_course(struct course *temp) {
    printf("%s :: Course Average: %4.2f, Number of Tests: %d", temp->name, temp->course_average, temp->num_tests);
    printf(", Number of Homework Assignments: %d, Number of Quizzes: %d", temp->num_hw, temp->num_quizzes);
    float temp_val = calc_course_gpa_float(temp->course_average);
    printf(", Credit Hours: %d, GPA: %s, %4.2f\n", temp->hours, temp->gpa, temp_val);
}

float calc_course_grade() {
    float course_average;
    int num_grades;
    printf("\tHow many total grades did you have for this course?\n");
    scanf("%d", &num_grades);
    for (int i = 0; i < num_grades; i++) {
        float temp_weight;
        float temp_grade;
        printf("\tWhat percent weight (out of 100) does this assignment have out of the course?\n");
        scanf("%f",&temp_weight);
        printf("\tWhat grade (out of 100) did you get on this assignment?\n");
        scanf("%f",&temp_grade);
        course_average += (temp_weight*temp_grade/100);
        if (course_average > 100) {
            printf("You have not put in the grades and weights properly. Course grade exceeding 100.");
            return course_average;
        }
    }
    return course_average;
}

char * calc_course_gpa_char(float course_average) {
    char *gpa;
    if (course_average >= 92.5) {
        gpa = "A";
    } else if (course_average >= 90) {
        gpa = "A-";
    } else if (course_average >= 87.5) {
        gpa = "B+";
    } else if (course_average >= 82.5) {
        gpa = "B";
    } else if (course_average >= 80) {
        gpa = "B-";
    } else if (course_average >= 77.5) {
        gpa = "C+";
    } else if (course_average >= 72.5) {
        gpa = "C";
    } else if (course_average >= 70) {
        gpa = "C-";
    } else if (course_average >= 60) {
        gpa = "D";
    } else {
        gpa = "F";
    }
    return gpa;
}

float calc_course_gpa_float(float course_average) {
    if (course_average >= 92.5) {
        return 4.0;
    } else if (course_average >= 90) {
        return 3.67;
    } else if (course_average >= 87.5) {
        return 3.33;
    } else if (course_average >= 82.5) {
        return 3;
    } else if (course_average >= 80) {
        return 2.67;
    } else if (course_average >= 77.5) {
        return 2.33;
    } else if (course_average >= 72.5) {
        return 2;
    } else if (course_average >= 70) {
        return 1.67;
    } else if (course_average >= 60) {
        return 1;
    } else {
        return 0;
    }
}

float calc_overall_gpa(struct course list[], int list_size) {
    float overall_gpa = 0;
    int total_hours = 0;
    for (int i = 0; i < list_size; i++) {
        total_hours += list[i].hours;
        overall_gpa += list[i].hours * calc_course_gpa_float(list[i].course_average);
    }
    return overall_gpa/total_hours;
}

void print_report_card(struct course list[], int list_size) {
    for (int i = 0; i < list_size; i++) {
        report_course(&list[i]);
    }
}

int start_calc() {
    int num_courses;
    printf("How many courses did you take this semester?\n");
    scanf("%d",&num_courses);
    /*At this point, num_courses should be set.*/
    struct course course_list[num_courses];
    for (int i = 0; i < num_courses; i++) {
        struct course temp;
        /*Variables for the current course that need to be assigned.*/
        char name[MAX_COURSE_LENGTH];
        /*User input + Creating the course structure*/
        printf("What is the name of course number %d?\n",i+1);
        scanf("%40s", name);
        temp.name = malloc(strlen(name)+1);
        strcpy(temp.name, name);
        printf("What is your grade for this course? (-1 if grade needs to be calculate)\n");
        scanf("%f",&temp.course_average);
        /*Special case for -1 input*/
        if(temp.course_average == -1){
            temp.course_average = calc_course_grade();
        }
        printf("How many tests were in this course, including the final?\n");
        scanf("%d",&temp.num_tests);
        printf("How many quizzes were in this course?\n");
        scanf("%d",&temp.num_quizzes);
        printf("How many homework assignments were in this course?\n");
        scanf("%d",&temp.num_hw);
        char *gpa = calc_course_gpa_char(temp.course_average);
        temp.gpa = malloc(strlen(gpa)+1);
        strcpy(temp.gpa, gpa);
        printf("How many credit hours was this course?\n");
        scanf("%hu",&temp.hours);
        course_list[i] = temp;
    }
    print_report_card(course_list,num_courses);
    float overall_gpa = calc_overall_gpa(course_list,num_courses);
    printf("\nOverall GPA for this semester: %4.2f\n",overall_gpa);
    return num_courses;
}

int main() {
    int temp = start_calc();

    return EXIT_SUCCESS;
}