// This program configures setup.h and changes the default settings for canvas width, height and grid size.
// Jade Hartley, UCL November 2019

#include <stdio.h>

void emptyInputBuffer(void);
int input_integer_range(const char *prompt, int min, int max); 
void write_to_setup(int CANVAS_WIDTH, int CANVAS_HEIGHT, int level);


void emptyInputBuffer()
{
  char ch;
  while ((ch = getchar()) != '\n' && ch != EOF);
}

// The following function was adapted from the solutions to C excercises 3 on the COMP0002 Moodle Page
int input_integer_range(const char *prompt, int min, int max) 
{
  int inputValue = 0;

  while(1)
  {
    printf("%s ", prompt);
    if (scanf("%d", &inputValue))
    { 
      if(inputValue>=min && inputValue<=max){
      break; 
      }}
    printf("Unable to recognise a number within the valid range, please try again.");
    emptyInputBuffer();
    printf("\n");
}
  return inputValue;
}

void write_to_setup(int CANVAS_WIDTH, int CANVAS_HEIGHT, int level){
  FILE * setupfile;
  setupfile = fopen ("setup.h", "w+");
  int gridsize = -5*level +25;

  
  fprintf(setupfile, "#define CANVAS_WIDTH %d\n", CANVAS_WIDTH);
  fprintf(setupfile, "#define CANVAS_HEIGHT %d\n", CANVAS_HEIGHT);
  fprintf(setupfile, "#define GRID_SIZE %d\n", gridsize);
  fprintf(setupfile, "#define OFFSET 40\n#define ROWS ((CANVAS_HEIGHT-OFFSET*2) / GRID_SIZE)\n#define COLUMNS ((CANVAS_WIDTH-OFFSET*2) / GRID_SIZE)");

  
  fclose(setupfile);
}


int main(void){
    int width, height, complexity;
    printf("Welcome to the maze setup. Let's pick some settings!\n");
    printf("First, let's pick a size for the canvas\n");
    width = input_integer_range("Please enter the width you would like, as an integer between 100 and 1000",100,1000);
    height = input_integer_range("Please enter the height you would like, again as an integer between 100 and 1000",100,1000);
    printf("Next, let's choose a complexity level.\nPlease enter a level of difficulty between 1 (easy) and 3 (hard).");
    complexity = input_integer_range("What level of complexity would you like?",1,3);
    write_to_setup(width, height, complexity);
    printf("Your settings have been saved.\nTo run the program, run the commands:\n- gcc cmaze.c graphics.c\n- ./a.out style show? |java -jar drawapp.jar");
    printf("\nWhere show is optional (displays route) and style is an option out of: 'princess', 'space' and 'plain'\n");

}