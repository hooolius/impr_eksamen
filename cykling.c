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
  int participated;
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
  int total_points;
} racer;

/* function prototypes */
int print_or_prompt(int argc, char *argv[]);
int prompt_for_function();
int line_counter(FILE *pinput_file, char *search_string);
void file_parser(FILE *pinput_file, int lines, parsed_line parsed_lines[]);
void gen_racers(FILE *pinput_file, int lines, parsed_line *parsed_lines,
    racer racers[]);
void gen_first_and_last_name(char full_name[], char first_name[], char last_name[]);
void gen_races(FILE *pinput_file, parsed_line parsed_lines[], racer racers[], int i, int j);
int gen_points(char position[], int participants);
int calc_total_points(racer racer);
int participation_points(char position[]);
int placement_points(char position[], int participants);
int struct_compare(const void *a, const void *b);
int compare_points_and_name(const void *a, const void *b);
void print_italians_over30();
void calc_italians_over30();
void print_finished_danes();
void find_finished_danes();
int calc_finished_races(racer racer);
void print_top10_finishers();
void calc_top10_finishers();
void print_paris_amstel_winner();
void calc_paris_amstel_winner(FILE *pinput_file, char racer_name[],
    char time[]);
int calc_total_time(char time1[], char time2[]);
int time_compare(const void *a, const void *b);
void reformat_time(int seconds, char time[]);
void print_top10_avg_age();
double calc_top10_avg_age(FILE *pinput_file);

int main(int argc, char *argv[]){
  int quit = 0;
  if (print_or_prompt(argc, argv)){
    print_italians_over30();
    print_finished_danes();
    print_top10_finishers();
    print_paris_amstel_winner();
    print_top10_avg_age();
  }
  else{
    while(!quit){
      switch(prompt_for_function()){
        case 1:
          print_italians_over30();
          break;
        case 2:
          print_finished_danes();
          break;
        case 3:
          print_top10_finishers();
          break;
        case 4:
          print_paris_amstel_winner();
          break;
        case 5:
          print_top10_avg_age();
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
  printf("(1) Print the parsed_lines of all Italian racers over the age of 30. \n");
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


void gen_racers(FILE *pinput_file, int lines, parsed_line *parsed_lines,
    racer racers[]){
  int i, j = -1;
  file_parser(pinput_file, lines, parsed_lines);

  for (i = 0; i < lines; i++){
    if (strcmp(parsed_lines[i].racer_name, parsed_lines[i-1].racer_name) == 0){
      gen_races(pinput_file, parsed_lines, racers, i, j);
      racers[j].total_points = calc_total_points(racers[j]);
    }
    else{
      j++;
      gen_first_and_last_name(parsed_lines[i].racer_name, racers[j].first_name, racers[j].last_name);
      racers[j].age = parsed_lines[i].racer_age;
      strcpy(racers[j].country, parsed_lines[i].racer_country);
      strcpy(racers[j].team, parsed_lines[i].team_name);

      gen_races(pinput_file, parsed_lines, racers, i, j);
      racers[j].total_points = calc_total_points(racers[j]);
    }
  } 
  /*
     for (i = 0; i < 500; i++){
     printf("name: %s %s age: %f team: %s country: %s\n"
     "paris: pos: %s time: %s participants: %d points: %d\n"
     "amstel: pos: %s time: %s participants: %d points: %d\n"
     "fleche: pos: %s time: %s participants: %d points: %d\n"
     "bastogne: pos: %s time: %s participants: %d points: %d\n\n\n",
     racers[i].first_name, racers[i].last_name, racers[i].age,
     racers[i].team, racers[i].country,
     racers[i].races[(int)paris].position, racers[i].races[(int)paris].time, racers[i].races[(int)paris].participants, racers[i].races[(int)paris].points,
     racers[i].races[(int)amstel].position, racers[i].races[(int)amstel].time, racers[i].races[(int)amstel].participants, racers[i].races[(int)amstel].points,
     racers[i].races[(int)fleche].position, racers[i].races[(int)fleche].time, racers[i].races[(int)fleche].participants, racers[i].races[(int)fleche].points,
     racers[i].races[(int)bastogne].position, racers[i].races[(int)bastogne].time, racers[i].races[(int)bastogne].participants, racers[i].races[(int)bastogne].points);
     */
}

void gen_races(FILE *pinput_file, parsed_line parsed_lines[],
    racer racers[], int i, int j){
  int participants_paris = line_counter(pinput_file, "ParisRoubaix");
  int participants_amstel = line_counter(pinput_file, "AmstelGoldRace");
  int participants_fleche = line_counter(pinput_file, "LaFlecheWallonne");
  int participants_bastogne = line_counter(pinput_file, "LiegeBastogneLiege");

  if (strcmp(parsed_lines[i].race_name, "ParisRoubaix") == 0){
    strcpy(racers[j].races[(int)paris].name, parsed_lines[i].race_name);
    strcpy(racers[j].races[(int)paris].position, parsed_lines[i].position);
    strcpy(racers[j].races[(int)paris].time, parsed_lines[i].time);
    racers[j].races[(int)paris].participants = participants_paris;
    racers[j].races[(int)paris].points = gen_points(racers[j].
        races[(int)paris].position, participants_paris);
    racers[j].races[(int)paris].participated = 1;
  }
  else if (strcmp(parsed_lines[i].race_name, "AmstelGoldRace") == 0){
    strcpy(racers[j].races[(int)amstel].name, parsed_lines[i].race_name);
    strcpy(racers[j].races[(int)amstel].position, parsed_lines[i].position);
    strcpy(racers[j].races[(int)amstel].time, parsed_lines[i].time);
    racers[j].races[(int)amstel].participants = participants_amstel;
    racers[j].races[(int)amstel].points = gen_points(racers[j].
        races[(int)amstel].position, participants_amstel);
    racers[j].races[(int)amstel].participated = 1;
  }
  else if (strcmp(parsed_lines[i].race_name, "LaFlecheWallonne") == 0){
    strcpy(racers[j].races[(int)fleche].name, parsed_lines[i].race_name);
    strcpy(racers[j].races[(int)fleche].position, parsed_lines[i].position);
    strcpy(racers[j].races[(int)fleche].time, parsed_lines[i].time);
    racers[j].races[(int)fleche].participants = participants_fleche;
    racers[j].races[(int)fleche].points = gen_points(racers[j].
        races[(int)fleche].position, participants_fleche);
    racers[j].races[(int)fleche].participated = 1;
  }
  else if (strcmp(parsed_lines[i].race_name, "LiegeBastogneLiege") == 0){
    strcpy(racers[j].races[(int)bastogne].name, parsed_lines[i].race_name);
    strcpy(racers[j].races[(int)bastogne].position, parsed_lines[i].position);
    strcpy(racers[j].races[(int)bastogne].time, parsed_lines[i].time);
    racers[j].races[(int)bastogne].participants = participants_bastogne;
    racers[j].races[(int)bastogne].points = gen_points(racers[j].
        races[(int)bastogne].position, participants_bastogne);
    racers[j].races[(int)bastogne].participated = 1;
  }
}
void file_parser(FILE *pinput_file, int lines, parsed_line parsed_lines[]){
  int i;

  for (i = 0; i < lines; i++){
    fscanf(pinput_file, " %s \"%[' a-zA-Z -]\" | %lf %s %s | %s %[0-9 : -]",
        parsed_lines[i].race_name, parsed_lines[i].racer_name, &parsed_lines[i].racer_age, 
        parsed_lines[i].team_name, parsed_lines[i].racer_country, parsed_lines[i].position,
        parsed_lines[i].time);
  }
  rewind(pinput_file);
  qsort(parsed_lines, lines, sizeof(parsed_line), struct_compare);
  return;
}

int struct_compare(const void *a, const void *b){
  parsed_line *ia = (parsed_line *)a;
  parsed_line *ib = (parsed_line *)b;
  return strcmp(ia->racer_name, ib->racer_name);
}

void gen_first_and_last_name(char full_name[], char first_name[], char last_name[]){
  int j = strlen(full_name);
  char *pfull_name;
  while (!islower(full_name[j])){
    j--;
  }
  pfull_name = (char *)full_name + j + 2;
  strcpy(last_name, pfull_name);
  strncpy(first_name, full_name, j+1);
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

int calc_total_points(racer racer){
  int i, total_points = 0;
  for (i = 0; i < RACE_COUNT; i++)
    if (racer.races[i].participated == 1){
      total_points += racer.races[i].points; 
    }
  return total_points;
}

void print_italians_over30(){
  int i = 0, j;
  FILE *pinput_file = fopen("cykelloeb", "r");
  int ita_lines = line_counter(pinput_file, "ITA");
  racer *result = (racer*) malloc(ita_lines * sizeof(racer));
  calc_italians_over30(pinput_file, result);

  while (strcmp(result[i].country, "ITA") == 0){ 
    printf("%s %s, age %.0f from %s and team %s participated in the following"
        " race(s), with the following result(s): \n",
        result[i].first_name, result[i].last_name, result[i].age,
        result[i].country, result[i].team);
    for (j = 0; j < RACE_COUNT; j++){
      if (result[i].races[j].participated == 1){
        printf("%s: Position: %s, Time: %s, Points: %d\n",
            result[i].races[j].name, result[i].races[j].position,
            result[i].races[j].time, result[i].races[j].points);
      } 
    }
    printf("\n");
    i++;
  }
  return;
}

void calc_italians_over30(FILE *pinput_file, racer result[]){
  int i, j;
  int lines = line_counter(pinput_file, "\n");
  parsed_line *parsed_lines = (parsed_line*) malloc(lines * sizeof(parsed_line));
  racer *racers = (racer*) malloc(lines * sizeof(racer));
  gen_racers(pinput_file, lines, parsed_lines, racers);

  for (i = 0; i < lines; i++){
    if (strcmp(racers[i].country, "ITA") == 0 && racers[i].age > 30.0){
      result[j] = racers[i];
      j++;
    } 
  }
  return;
}

void print_finished_danes(){
  int i = 0;
  int finished_races;
  FILE *pinput_file = fopen("cykelloeb", "r");
  int den_lines = line_counter(pinput_file, "DEN");
  racer *result = (racer*) malloc(den_lines * sizeof(racer));
  find_finished_danes(pinput_file, result);

  while (strcmp(result[i].country, "DEN") == 0){ 
    finished_races = calc_finished_races(result[i]);
    if (finished_races > 0){
      printf("%s %s, age %.0f from %s and team %s finished %d races.",
          result[i].first_name, result[i].last_name, result[i].age,
          result[i].country, result[i].team, finished_races);
      printf("\n");
    }
    i++;
  }
  printf("\n");
  return;
}

void find_finished_danes(FILE *pinput_file, racer result[]){
  int i, j;
  int lines = line_counter(pinput_file, "\n");
  parsed_line *parsed_lines = (parsed_line*) malloc(lines * sizeof(parsed_line));
  racer *racers = (racer*) malloc(lines * sizeof(racer));
  gen_racers(pinput_file, lines, parsed_lines, racers);

  for (i = 0; i < lines; i++){
    if (strcmp(racers[i].country, "DEN") == 0){
      result[j] = racers[i];
      j++;
    } 
  }
  return;
}

int calc_finished_races(racer racer){
  int races = 0, i;
  for (i = 0; i < RACE_COUNT; i++){
    if (racer.races[i].participated == 1 && strcmp(racer.races[i].position,
          "DNF") != 0){
      races++; 
    }
  }
  return races;
}

void print_top10_finishers(){
  int i = 0;
  FILE *pinput_file = fopen("cykelloeb", "r");
  int top10_lines = line_counter(pinput_file, "\n");
  racer *result = (racer*) malloc(top10_lines * sizeof(racer));
  calc_top10_finishers(pinput_file, result);

  printf("The top 10 racers with the most total points are: \n");
  for (i = 0; i < 10; i++){
    printf("%02d: %s %s, age %0.f from %s and team %s, who had a total of %d points. \n",
        i+1, result[i].first_name, result[i].last_name, result[i].age, result[i].country,
        result[i].team, result[i].total_points);
    printf("\n");
  }
  return;
}

void calc_top10_finishers(FILE *pinput_file, racer result[]){
  int lines = line_counter(pinput_file, "\n");
  parsed_line *parsed_lines = (parsed_line*) malloc(lines * sizeof(parsed_line));
  gen_racers(pinput_file, lines, parsed_lines, result);
  qsort(result, lines, sizeof(racer), compare_points_and_name);

  return;
}

int compare_points_and_name(const void *a, const void *b) {

  racer *ia = (racer *)a;
  racer *ib = (racer *)b;

  int comp =  ib->total_points - ia->total_points;

  if (comp < 0)
    return -1;

  if (comp > 0)
    return 1;

  comp = strcmp(ia->last_name, ib->last_name);

  return comp;
}

void print_paris_amstel_winner(){
  FILE *pinput_file = fopen("cykelloeb", "r");
  char racer_name[MAX_FIRST_NAME + MAX_LAST_NAME];
  char time[MAX_TIME];
  calc_paris_amstel_winner(pinput_file, racer_name, time);
  printf("The racer with the lowest combined time in the"
      " Paris and Amstel races was %s, with a total time of %s \n\n",
      racer_name, time);
}

void calc_paris_amstel_winner(FILE *pinput_file, char racer_name[],
    char time[]){
  int i, lowest_time_index, lowest_time = 100000000;
  int lines = line_counter(pinput_file, "\n");
  int *times = (int*) malloc(lines * sizeof(int));
  parsed_line *parsed_lines = (parsed_line*) malloc(lines * sizeof(parsed_line));
  racer *results = (racer*) malloc(lines * sizeof(racer));
  gen_racers(pinput_file, lines, parsed_lines, results);
  for (i = 0; i < lines; i++){
    if (strcmp(results[i].races[(int)paris].time, "- ") != 0 && results[i].races[(int)paris].participated == 1 &&
        strcmp(results[i].races[(int)amstel].time, "- ") != 0 && results[i].races[(int)amstel].participated == 1){
      times[i] = calc_total_time(results[i].races[(int)paris].time,
          results[i].races[(int)amstel].time);
      if (times[i] < lowest_time){
        lowest_time_index = i;
        lowest_time = times[i];
      }
    }
  }
  sprintf(racer_name, "%s %s", results[lowest_time_index].first_name,
      results[lowest_time_index].last_name);
  reformat_time(lowest_time, time);
}

int calc_total_time(char time1[], char time2[]){
  int hour1, min1, sec1, hour2, min2, sec2;
  sscanf(time1, "%d:%d:%d", &hour1, &min1, &sec1);
  sscanf(time2, "%d:%d:%d", &hour2, &min2, &sec2);

  return (sec1 + sec2) + ((min1 + min2) * 60) + ((hour1 + hour2) * 60 * 60);
}

void reformat_time(int seconds, char time[]){
  int hours, minutes, res;

  hours = seconds / (60*60);
  res = seconds % (60*60);
  minutes = res / 60;
  res = minutes % 60;

  sprintf(time, "%d:%d:%d", hours, minutes, res);
}

int time_compare(const void *a, const void *b){ 
  const int *ia = (const int*)a;
  const int *ib = (const int*)b;
  return *ia - *ib;
}

void print_top10_avg_age(){
  FILE *pinput_file = fopen("cykelloeb", "r");
  double average_age;
  average_age = calc_top10_avg_age(pinput_file);
  printf("The average age of all top 10 finishers is: %0.2f \n", average_age);
  return;
}

double calc_top10_avg_age(FILE *pinput_file){
  int i, j;
  int lines = line_counter(pinput_file, "\n");
  int top10_finish_lines = 10*4;
  double total_age;
  int unique_racers = 0;
  racer *top10_racers = (racer*) malloc(top10_finish_lines * sizeof(racer));
  racer *racers = (racer*) malloc(lines * sizeof(racer));
  parsed_line *parsed_lines = (parsed_line*) malloc(lines * sizeof(parsed_line));
  gen_racers(pinput_file, lines, parsed_lines, racers);

  for (i = 0; i < lines; i++){
    for (j = 0; j < RACE_COUNT; j++){
      if (atoi(racers[i].races[j].position) != 0 && atoi(racers[i].races[j].position) <= 10){
        total_age += racers[i].age;
        i++;
        unique_racers++;
        j=0;
      }
    }
  }
  return (total_age/unique_racers);
}
