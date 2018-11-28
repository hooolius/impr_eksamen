#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* constants */
#define MAX_RACE_NAME 30
#define MAX_FIRST_NAME 30
#define MAX_LAST_NAME 30
#define MAX_NAME 60
#define MAX_SHORTHAND 4
#define MAX_TIME 10
#define MAX_LINES 2000
#define RACE_COUNT 4

/* enums */
/* the race_name enum is used for keeping track of which slot in the race array 
 * of the racer struct the data for each race should be placed in, as uniformity
 * in the indexes of these arrays is ideal. */
enum race_name{
  paris = 0,
  amstel = 1,
  fleche = 2,
  bastogne = 3
};

/* structs */
/* parsed_line is used for storing raw data from the input file, 
 * before it is then properly sorted into the racer and race structs. */
typedef struct parsed_line{
  char race_name[MAX_RACE_NAME];
  char racer_name[MAX_NAME];
  double racer_age;
  char team_name[MAX_SHORTHAND];
  char racer_country[MAX_SHORTHAND];
  char position[MAX_SHORTHAND];
  char time[MAX_TIME];
} parsed_line;

/* race is used for storing individual race data for a racer. */
typedef struct race{
  char name[MAX_RACE_NAME];
  char position[MAX_SHORTHAND];
  char time[MAX_TIME];
  int points;
  int participants;
} race;

/* racer is used for storing data about a racer, and contains a race 
 * array for keeping track of what races the racer has participated in */
typedef struct racer{
  char first_name[MAX_FIRST_NAME];
  char last_name[MAX_LAST_NAME];
  double age;
  char country[MAX_SHORTHAND];
  char team[MAX_SHORTHAND];
  race races[RACE_COUNT];
} racer;

/* function prototypes */
int print_or_prompt(int argc, char *argv[]);
int prompt_for_function();
int line_counter(FILE *pinput_file, char *search_string);
void file_parser(FILE *pinput_file, int lines, parsed_line results[]);
void gen_racers(parsed_line *results);
void gen_races(FILE *pinput_file, parsed_line results[], racer racers[], int i, int j);
int gen_points(char position[], int participants);
int participation_points(char position[]);
int placement_points(char position[], int participants);
int struct_compare(const void *a, const void *b);
int result_italian_o30();
void calc_italian_o30();
int result_danish_finished();
int result_top10();
int result_paris_amstel();
int result_top10_avg_age();

int main(int argc, char *argv[]){
  int quit = 0;
  parsed_line *results = (parsed_line*) malloc(790 * sizeof(parsed_line));
  gen_racers(results);
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

int line_counter(FILE *pinput_file, char *search_string){
  int n = 0;
  char line[MAX_LINES];

  while (fgets(line, MAX_LINES, pinput_file) != NULL){
    if (strstr(line, search_string) != NULL){
      n++;
    }
  }
  rewind(pinput_file);
  return n;
}


void gen_racers(parsed_line *results){
  /*int i, j;
    char full_name[MAX_FIRST_NAME + MAX_LAST_NAME];
    char *pfull_name;*/
  int i, j = -1;
  racer racers[500];
  FILE *pinput_file = fopen("cykelloeb", "r");
  int lines = line_counter(pinput_file, "\n");
  file_parser(pinput_file, lines, results);

  for (i = 0; i < lines; i++){
    if (strcmp(results[i].racer_name, results[i-1].racer_name) == 0){
      gen_races(pinput_file, results, racers, i, j);
    }
    else{
      j++;
      racers[j].age = results[i].racer_age;
      strcpy(racers[j].country, results[i].racer_country);
      strcpy(racers[j].team, results[i].team_name);

      gen_races(pinput_file, results, racers, i, j);
    }
  } 
  /*
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
     for (i = 0; i < lines; i++){
     printf("name: %s age: %lf team: %s country: %s pos: %s time: %s\n",
     results[i].racer_name, 
     results[i].racer_age, results[i].team_name, results[i].racer_country,
     results[i].position, results[i].time);
     }
     */
  for (i = 0; i < 100; i++){
    printf("age: %f team: %s country: %s\n"
        "paris: pos: %s time: %s participants: %d points: %d\n"
        "amstel: pos: %s time: %s participants: %d points: %d\n"
        "fleche: pos: %s time: %s participants: %d points: %d\n"
        "bastogne: pos: %s time: %s participants: %d points: %d\n\n\n",
        racers[i].age,
        racers[i].team, racers[i].country,
        racers[i].races[(int)paris].position, racers[i].races[(int)paris].time, racers[i].races[(int)paris].participants, racers[i].races[(int)paris].points,
        racers[i].races[(int)amstel].position, racers[i].races[(int)amstel].time, racers[i].races[(int)amstel].participants, racers[i].races[(int)amstel].points,
        racers[i].races[(int)fleche].position, racers[i].races[(int)fleche].time, racers[i].races[(int)fleche].participants, racers[i].races[(int)fleche].points,
        racers[i].races[(int)bastogne].position, racers[i].races[(int)bastogne].time, racers[i].races[(int)bastogne].participants, racers[i].races[(int)bastogne].points);

  }
}

void gen_races(FILE *pinput_file, parsed_line results[],
    racer racers[], int i, int j){
  int participants_paris = line_counter(pinput_file, "ParisRoubaix");
  int participants_amstel = line_counter(pinput_file, "AmstelGoldRace");
  int participants_fleche = line_counter(pinput_file, "LaFlecheWallonne");
  int participants_bastogne = line_counter(pinput_file, "LiegeBastogneLiege");

      if (strcmp(results[i].race_name, "ParisRoubaix") == 0){
        strcpy(racers[j].races[(int)paris].name, results[i].race_name);
        strcpy(racers[j].races[(int)paris].position, results[i].position);
        strcpy(racers[j].races[(int)paris].time, results[i].time);
        racers[j].races[(int)paris].participants = participants_paris;
        racers[j].races[(int)paris].points = gen_points(racers[j].
            races[(int)paris].position, participants_paris);
      }
      else if (strcmp(results[i].race_name, "AmstelGoldRace") == 0){
        strcpy(racers[j].races[(int)amstel].name, results[i].race_name);
        strcpy(racers[j].races[(int)amstel].position, results[i].position);
        strcpy(racers[j].races[(int)amstel].time, results[i].time);
        racers[j].races[(int)amstel].participants = participants_amstel;
        racers[j].races[(int)amstel].points = gen_points(racers[j].
            races[(int)amstel].position, participants_amstel);
      }
      else if (strcmp(results[i].race_name, "LaFlecheWallonne") == 0){
        strcpy(racers[j].races[(int)fleche].name, results[i].race_name);
        strcpy(racers[j].races[(int)fleche].position, results[i].position);
        strcpy(racers[j].races[(int)fleche].time, results[i].time);
        racers[j].races[(int)fleche].participants = participants_fleche;
        racers[j].races[(int)fleche].points = gen_points(racers[j].
            races[(int)fleche].position, participants_fleche);
      }
      else if (strcmp(results[i].race_name, "LiegeBastogneLiege") == 0){
        strcpy(racers[j].races[(int)bastogne].name, results[i].race_name);
        strcpy(racers[j].races[(int)bastogne].position, results[i].position);
        strcpy(racers[j].races[(int)bastogne].time, results[i].time);
        racers[j].races[(int)bastogne].participants = participants_bastogne;
        racers[j].races[(int)bastogne].points = gen_points(racers[j].
            races[(int)bastogne].position, participants_bastogne);
      }
}
void file_parser(FILE *pinput_file, int lines, parsed_line results[]){
  int i;

  for (i = 0; i < lines; i++){
    fscanf(pinput_file, " %s \"%[' a-zA-Z -]\" | %lf %s %s | %s %[0-9 : -]",
        results[i].race_name, results[i].racer_name, &results[i].racer_age, 
        results[i].team_name, results[i].racer_country, results[i].position,
        results[i].time);
  }
  rewind(pinput_file);
  qsort(results, lines, sizeof(parsed_line), struct_compare);
  return;
}

int struct_compare(const void *a, const void *b){
  parsed_line *ia = (parsed_line *)a;
  parsed_line *ib = (parsed_line *)b;
  return strcmp(ia->racer_name, ib->racer_name);
}

int gen_points(char position[], int participants){
  int points = 0;
  points += participation_points(position);
  points += placement_points(position, participants);

  return points;
}

int participation_points(char position[]){
  int points = 0;
  if (atoi(position) != 0){
    points = 3; 
  }
  else if (strcmp(position, "OTL") == 0){
    points = 1;
  }
  return points;
}

int placement_points(char position[], int participants){
  int points = 0;
  int pos;
  if (atoi(position) != 0){
    pos = atoi(position);
    points += (participants - pos) / 13;

    if (pos == 1){
      points += 10; 
    }
    else if (pos == 2){
      points += 5; 
    }
    else if (pos == 3){
      points += 2; 
    }
  }
  return points;
}

int result_italian_o30(){
  return 1;

}

void calc_italian_o30(){
  return;
}

