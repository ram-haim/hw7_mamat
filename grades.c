#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked-list.h"
#include "grades.h"
//Adt struct declare
struct node *node;

//Returning 1 for unvalid inserts
//Returning -1 for unvalid malloc


//My structs declare
//Each student has a list of course
struct student {
  int   student_id;
  char *student_name;
  struct list *courses_list;
};
struct courses {
  int course_grade;
  char *course_name;
};
//Grades contain a list of students
struct grades {
 struct list *students;
};


//Adt user Functions declerations
int clone_course(void *elem, void **output);
void destroy_courses(void *elem);
int clone_student(void *elem, void **output);
void destroy_student(void *elem);
//End of adt user functions declerations

//Helper Functions Declare
bool id_in_list(struct grades *grades,int id_to_check);
struct student* find_student_pointer(struct grades *grades ,int id);
bool course_in_list(struct student *student ,const char *course_to_check);
bool print_course_list(struct student *print_student_course);
bool valid_course(struct grades *grades,int grade, const char *name,int id);
//End of Helper Functions Declare


//Assigment Functions declerations
void grades_destroy(struct grades *grades);
struct grades* grades_init();
void grades_destroy(struct grades *grades);
int grades_add_student(struct grades *grades,const char *name, int id);
int grades_add_grade(struct grades *grades,
                      const char *name,
                     int id,
                     int grade);
float grades_calc_avg(struct grades *grades, int id,char **out);
int grades_print_student(struct grades *grades, int id);
int grades_print_all(struct grades *grades);
//End of Assigment functions declerations


//Adt user Functions implements
/**
 * clones course befor adding it to list
 * param elem: the course we want to add
 * param output : holds a clone of the course
 * returns 0 on success
 **/
int clone_course(void *elem, void **output){
  struct courses *temp_course = (struct courses *) elem;
  struct courses *course_to_add = (struct courses *) malloc(sizeof(struct courses));
  if(course_to_add == NULL){
    fprintf(stderr,"Failed to Allocate Bytes");
    return -1;
  }
  course_to_add->course_grade = temp_course->course_grade;
  course_to_add->course_name = (char *) malloc(strlen(temp_course->course_name)+1);
  if (course_to_add->course_name == NULL) {
    fprintf(stderr,"Failed to Allocate Bytes");
    free(course_to_add);//Freeing the course cause we got problem in the name.
    return -1;
  }
  strcpy(course_to_add->course_name, temp_course->course_name);
  *output = (void*)course_to_add;
  return 0;
}
/**
 * destroy a course 
 * param elem: the course we want to destroy
 **/
void destroy_courses(void *elem) {
  //First destroy name then destroy the struct itself,done the same as in the tirgol.
  struct courses *courses = (struct courses*) elem;
  free(courses->course_name);
  free(courses);
}
/**
 * clones student befor adding it to list
 * param elem: the student we want to add
 * param output : holds a clone of the student
 * returns 0 on success
 **/
int clone_student(void *elem, void **output) {
  struct student *temp_student = (struct student *) elem;
  struct student *student_to_add = (struct student *) malloc(sizeof(struct student));
   if(student_to_add == NULL){
    fprintf(stderr,"Failed to Allocate Bytes");
    free(student_to_add);
    return -1;
  }
  student_to_add->student_id = temp_student->student_id;
  student_to_add->student_name =(char *) malloc(strlen(temp_student->student_name) + 1);
  if (student_to_add->student_name == NULL) {
    fprintf(stderr,"Failed to Allocate Bytes");
    free(student_to_add);
    return -1;
  }
  strcpy(student_to_add->student_name, temp_student->student_name);
  student_to_add->courses_list = list_init(clone_course, destroy_courses);
  *output = (void*)student_to_add;
  return 0;
}
/**
 * destroy a student 
 * param elem: the student we want to destroy
 **/
void destroy_student(void *elem) {
  //First destroy name then destroy the struct itself,done the same as in the tirgol.
  struct student *student = (struct student*) elem;
  free(student->student_name);
  list_destroy(student->courses_list);
  free(student);
}


//Helper Functions implement
/** helper func , finds the the student pointer with given id. 
*  param grades : The list of students we are searching in.
*  param id : the specific id that we want to find.
*  otherwise return FALSE:
*  returns a pointer to the requested student.
**/
struct student *find_student_pointer(struct grades *grades ,int id) {
  struct node *it = list_begin(grades->students);
  while (it != NULL) {
    struct student *current_student = (struct student *)list_get(it);
    if (current_student == NULL) {
      return NULL;
    }
    if (id == current_student->student_id) {
      return current_student;
    }
    it = list_next(it);
  }
  //Couldn't find The id
  return NULL;
}
/** helper func , check if a given course is in the student courses list.. 
*  param student : pointer to the specific student struct .
*  param course_to_check : the specific course that we want to check if its in list
*  returns 0 if course in list and 1 if not or any error searching it happened.
**/
bool course_in_list(struct student *student ,const char *course_to_check) {
  struct node *it = list_begin(student->courses_list);
  while (it != NULL) {
    struct courses *current= (struct courses *)list_get(it);
    if (current == NULL) {
      return 1;
    }
    //course exist in list
    if (!(strcmp(current->course_name, course_to_check))) {
      return 0;
    }
    it = list_next(it);
  }
  return 1;
}
/** helper func , aids in printing the student course. 
*  param print_student_course : pointer to the specific student that we want to print .
*  param course_to_check : the specific course that we want to check if its in list
*  returns 0 if course in list and 1 if not or any error searching it happened.
**/
bool print_course_list(struct student *print_student_course) {
  struct node *it = list_begin(print_student_course->courses_list);
  while(it!= NULL){
    struct courses *course = (struct courses *)list_get(it);
    /*Iterator Check*/
    if (course == NULL) {
      return 1;
    }
    printf("%s %d", course->course_name, course->course_grade);
    it = list_next(it);
    if(it != NULL){
        printf(", ");
    }
  }
  printf("\n");
  return 0;
}
/** helper func , check if a given course grade is valid grade or not
*  param grades : The list of students we are searching in.
*  param name : the specific course that we want to check the grade
*  returns 0 if course in list and 1 if not or any error searching it happened.
**/
bool valid_course(struct grades *grades,int grade, const char *name,int id){
   if (grade<0 || grade>100){
    return 1;
  }
  struct student *student_to_add_grade = find_student_pointer(grades ,id);
      //Unvalid pointer value
  if (student_to_add_grade == NULL) {
    return 1;
    //Course in list already
  }
  else if (!(course_in_list(student_to_add_grade, name))){
    return 1;
    //Unvalid pointer value
  }
  else if (grades->students == NULL) {
    return 1; //
  }
    return 0; 
}
//Assignment implements.
/**
 * Initialize the "grades" data-structure.
 * returns a pointer to the data-structure, of NULL in case of an error
 **/
struct grades *grades_init() {
struct grades *grades = (struct grades *) malloc(sizeof(struct grades));
  if (grades == NULL) {//First malloc of prog.exe if this malloc is wrong must shut down the prog.
    fprintf(stderr,"Failed to Allocate Bytes");
    abort();
  }
  grades->students = list_init(clone_student, destroy_student);

  return grades;
}

/**
 * Destroys "grades", de-allocate all memory!
 * param grades : the list of students we created.
 **/
void grades_destroy(struct grades* grades){
 list_destroy(grades->students);
 free(grades);
}
/**
 * adds a student with "name" and "id" to "grades"
 * param grades: the students list
 * param name : the name of the student we want to add
 * param id : the id of the student we want to add
 * returns 0 on success
 * Failes if "grades" is invalid, or a student with 
 * the same "id" already exists in "grades".
 **/
int grades_add_student(struct grades *grades, const char *name, int id) {
  if(find_student_pointer(grades,id)!= NULL){//Id exists already
    return 1;
    }
  struct student *student_to_add = (struct student *) malloc(sizeof(struct student));
  student_to_add->student_name = (char *) malloc(strlen(name) + 1); //not sure
  if (student_to_add->student_name == NULL) {
    fprintf(stderr,"Failed to Allocate Bytes");
    return -1;
  }
  strcpy(student_to_add->student_name, name);
  student_to_add->student_id = id;
  student_to_add->courses_list = list_init(clone_course, destroy_courses);
  //Unvalid list pointer
  if (student_to_add->courses_list == NULL) {
    free(student_to_add->student_name);
    return 1;
  }
  list_push_back(grades->students, student_to_add);
  list_destroy(student_to_add->courses_list);
  free(student_to_add->student_name);
  free(student_to_add);
  return 0;
}
/**
 * adds a course with "name" and "id" to "courses list"
 * param grades: the students list
 * param name : the name of the course we want to add
 * param id : the id of the student we want to add the course to
 * param grade : the grade of the course we want to add
 * returns 0 on success
 * Failes if "course" is invalid, or a student with 
 * the same "id" doesnt exists in "grades".
 **/
int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade){
    struct student *student_to_add_grade = find_student_pointer(grades ,id);
 if(valid_course(grades,grade,name,id))
    return 1;
  struct courses *course_to_add = (struct courses*)malloc(sizeof(struct courses));
  //Init the course we want to add into student courses list
  course_to_add->course_grade = grade;
  course_to_add->course_name = (char *)malloc(strlen(name)+1);
  if (course_to_add->course_name == NULL) {
    fprintf(stderr,"Failed to Allocate Bytes");
    return -1;
  }
  strcpy(course_to_add->course_name,name);
  //Turn into void* for push function
  if(list_push_back(student_to_add_grade->courses_list, course_to_add)){
    //Unvalid insert to list.
    free(course_to_add->course_name);
    return 1;
  }
  free(course_to_add->course_name);
  free(course_to_add);
  return 0;
 /**
 * calculate average of student with id" 
 * param grades: the students list
 * param id : the id of the student we want to calc
 * param out : a pointer that holds the student name
 * returns 0 on success
 * Failes if student doesnt exist in list 
 **/
}
float grades_calc_avg(struct grades *grades , int id,char **out){
  float average = 0;
  float sum =0;
  struct student *student_to_calc = find_student_pointer(grades,id);
  //Id doesnt exist in students list.
  if(student_to_calc == NULL){
     *out = NULL;
     return -1;//To ensure it wont return a valid value to average
  }
  char *name_to_calc = (char *) malloc(strlen(student_to_calc->student_name) + 1);
  if(name_to_calc == NULL){//Bad allocation
        fprintf(stderr,"Failed to Allocate Bytes");
        return -1;
    }
  strcpy(name_to_calc,student_to_calc->student_name);
  *out = name_to_calc;
  struct node *it = list_begin(student_to_calc->courses_list);
  while(it!= NULL){
    struct courses *course = (struct courses *)list_get(it);
    if (course == NULL) {
      *out = NULL;
      free(name_to_calc);
      return 1;
    }
    sum+=course->course_grade;
    it = list_next(it);
  }
  if(!(list_size(student_to_calc->courses_list))){
    average = 0;
  }
  else {
    average = (float)(sum/list_size(student_to_calc->courses_list));
  }
  return average;
  //User responsible to free *out malloc.
}
/**
 * prints student information with
 * param grades: the students list
 * param id : the id of the student we want to print
 * returns 0 on success
 **/
int grades_print_student(struct grades *grades, int id) {

  if (grades->students == NULL) {
    return 1;
  }
  struct student *student_to_print = (find_student_pointer(grades ,id));
  if (student_to_print == NULL) {
    return 1;
  }
  printf("%s %d: ", student_to_print->student_name, id);
  if(print_course_list(student_to_print)){
    return 1;
    }
  else {
    return 0;
  }
}
/**
 * prints all students information with
 * param grades: the students list
 * returns 0 on success
 **/
int grades_print_all(struct grades *grades) {
  struct node *it = list_begin(grades->students);
  while (it != NULL) {
    struct student *current_student = (struct student *)list_get(it);
    if (current_student == NULL) {
      return 1;
    }
    printf("%s %d: ", current_student->student_name, current_student->student_id);
    it = list_next(it);
    if(print_course_list(current_student)){
        return 1;
    }
  }
  return 0;
}