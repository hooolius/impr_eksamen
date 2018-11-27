#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* constants */
#define MAX_RACE_NAME 30
#define MAX_FIRST_NAME 30
#define MAX_LAST_NAME 30
#define MAX_SHORTHAND 4
#define MAX_TIME 10

/* structs */
typedef struct result{
  char race_name[MAX_RACE_NAME];
  char racer_first_name[MAX_FIRST_NAME];
  char racer_last_name[MAX_LAST_NAME];
  int racer_age;
  char team_name[MAX_SHORTHAND];
  char racer_country[MAX_SHORTHAND];
  char position[MAX_SHORTHAND];
  char time[MAX_TIME];
} result;

/* function prototypes */
int print_or_prompt(int argc, char *argv[]);
int prompt_for_function();
int line_counter(FILE *pinput_file);
result *file_parser(FILE *pinput_file, int lines, result results[]);
int result_italian_o30();
result *calc_italian_o30();
int result_danish_finished();
int result_top10();
int result_paris_amstel();
int result_top10_avg_age();

int main(int argc, char *argv[]){
  int quit = 0;
  FILE *pinput_file = fopen("cykelloeb", "r");
  int lines = line_counter(pinput_file);
  result *results = (result*) malloc(lines * sizeof(result));
  file_parser(pinput_file, lines, results);
  if (print_or_prompt(argc, argv)){
    /* result_italian_o30();
       result_danish_finished();
       result_top10();
       result_paris_amstel();
       result_top10_avg_age(); */
  }
  else{
    while(!quit){
      switch(prompt_for_function()){
        case 1:
          /* result_italian_o30(); */
          break;
        case 2:
          /* result_danish_finished(); */
          break;
        case 3:
          /* result_top10(); */
          break;
        case 4:
          /* result_paris_amstel(); */
          break;
        case 5:
          /* result_top10_avg_age(); */
          break;
        case -1:
          printf("Exiting program... \n");
          quit = 1;
          break;
        default:
          printf("\n\x1B[31mInvalid input, please try again. \x1B[0m \n \n");
          break;
      }
    }
  }
  return 0;
}

int prompt_for_function(){
  int prompt_answer = 0;

  printf("Please select one of the following options: \n");
  printf("(1) Print the results of all Italian racers over the age of 30. \n");
  printf("(2) Print all Danish racers that have participated in a race,"
      " as well as how many races they each participated in. \n");
  printf("(3) Print the 10 racers with the most points, sorted by points,"
      " then last name\n");
  printf("(4) Print the racer with the lowest combined time in Paris Roubaix"
      " and Amstel Gold Race, as well as their total time. \n");
  printf("(5) Print the average age of racers that have finished in the"
      " top 10 of a race. \n");
  printf("(-1) To exit the program. \n \n");
  printf("Select an option: ");
  scanf(" %d", &prompt_answer);
  return prompt_answer;
}

int print_or_prompt(int argc, char *argv[]){
  int bool_print = 0, i;
  for (i = 0; i<argc; i++){
    if (strcmp(argv[i], "--print") == 0){
      bool_print = 1;
    }
  }
  return bool_print;
}

int line_counter(FILE *pinput_file){
  int lines = 0;
  char newline_searcher;
  while (!feof(pinput_file)){
    newline_searcher = fgetc(pinput_file);
    if (newline_searcher == '\n'){
      lines++; 
    }
  }
  rewind(pinput_file);

  return lines;
}

result *file_parser(FILE *pinput_file, int lines, result results[]){
  int i, j;
  char full_name[MAX_FIRST_NAME + MAX_LAST_NAME];
  char *pfull_name;
  
  for (i = 0; i < lines; i++){
    fscanf(pinput_file, " %s \"%[\' a-zA-Z -]\" | %d %s %s | %s %[0-9 : -]",
        results[i].race_name, full_name, &results[i].racer_age, 
        results[i].team_name, results[i].racer_country, results[i].position,
        results[i].time);

    j = strlen(full_name);
    while (!islower(full_name[j])){
      j--;
    }
    pfull_name = (char *)full_name + j + 2;
    strcpy(results[i].racer_last_name, pfull_name);
    strncpy(results[i].racer_first_name, full_name, j+1);

    printf("first name: %s last name: %s age: %d team: %s country: %s pos: %s time: %s\n",
        results[i].racer_first_name, results[i].racer_last_name,
        results[i].racer_age, results[i].team_name, results[i].racer_country,
        results[i].position, results[i].time);
  }
  return results;
}

int result_italian_o30(){
  
}

result *calc_italian_o30(){

}
