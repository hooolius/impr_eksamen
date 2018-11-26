#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* constants */
#define MAX_RACE_NAME 30
#define MAX_FIRST_NAME 30
#define MAX_LAST_NAME 30

/* structs */
typedef struct result{
  char race_name[MAX_RACE_NAME];
  char racer_first_name[MAX_FIRST_NAME];
  char racer_last_name[MAX_LAST_NAME];
  int racer_age;
  char team_name[3];
  char country[3];
  int position;
  int hours;
  int minutes;
  int seconds;
} result;

/* function prototypes */
int print_or_prompt(int argc, char *argv[]);
int prompt_for_function();
int result_italian_o30();
int result_danish_finished();
int result_top10();
int result_paris_amstel();
int result_top10_avg_age();

int main(int argc, char *argv[]){
  if (print_or_prompt(argc, argv)){
    // result_italian_o30();
    // result_danish_finished();
    // result_top10();
    // result_paris_amstel();
    // result_top10_avg_age();
  }
  else{
    switch(prompt_for_function()){
      case 1:
        // result_italian_o30();
        break;
      case 2:
        // result_danish_finished();
        break;
      case 3:
        // result_top10();
        break;
      case 4:
        // result_paris_amstel();
        break;
      case 5:
        // result_top10_avg_age();
        break;
      default:
        printf("Invalid input, please try again. \n");
    }
  }
}

int prompt_for_function(){
  int prompt_answer;
  printf("Please select one of the following options: \n");
  printf("(1) Print the results of all Italian racers over the age of 30.\n");
  printf("(2) Print all Danish racers that have participated in a race,"
      " as well as how many races they each participated in. \n");
  printf("(3) Print the 10 racers with the most points, sorted by points,"
      " then last name\n");
  printf("(4) Print the racer with the lowest combined time in Paris Roubaix"
      " and Amstel Gold Race, as well as their total time. \n");
  printf("(5) Print the average age of racers that have finished in the"
      " top 10 of a race. \n \n");
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
