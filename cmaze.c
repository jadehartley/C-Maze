// This is a drawing program that will generate mazes with options that allow the 
// size, complexity, visual style and other attributes to be selected.
// It is also possible to show the route through the maze
// Code by Jade Hartley, UCL, November 2019

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "graphics.h"
#include "setup.h"


struct cell 
{
    int x_position;
    int y_position;
    int walls[5]; 
    int neighbours[5];
    int visited;
};

struct node
{ 
    struct cell * grid_cell;
    struct node * next;
    struct node * prev;
};

struct cell *cells[COLUMNS*ROWS+1]; 
struct node *head = NULL; 
struct cell *route[COLUMNS*ROWS+1]; 


void setWindowSize(int width, int height);
void setRGBColour(int red, int green, int blue);
struct node* push(struct node* head,struct cell * current);
struct node* pop(struct node *head);
struct cell *create_cell(int i, int j);

void create_grid(void);
void remove_walls(struct cell * first, struct cell * second);
void store_stack(void);
int neighbour(struct cell * current, int i);
void visit_cells(void);
void increase_difficulty(void);
void draw_space(void);
void draw_princess(void);
void draw_route(void);
void draw_grid(int argc, char * argv[]);
time_t t;




struct node* push(struct node* head,struct cell * current)
{
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    if(new_node == NULL) 
    {
        exit(0);
    }
    new_node->next = head;
    new_node->grid_cell = current;
    head = new_node;
    if (head->next != NULL){
        head->next->prev = head;
    }
    return head;
}

struct node* pop(struct node *head)
{
    head = head->next;
    free(head->prev);
    return head;
}

struct cell * create_cell(int x, int y)
{
    struct cell *thiscell = calloc(sizeof(struct cell),1);
    thiscell->x_position = x;
    thiscell->y_position = y;
    thiscell->walls[0] = 1; // top wall
    thiscell->walls[1] = 1; // right 
    thiscell->walls[2] = 1; // bottom 
    thiscell->walls[3] = 1; // left
    thiscell->visited = 0;
    thiscell->neighbours[0] = x*ROWS + y-1; //index position of top neighbour
    thiscell->neighbours[1] = (x+1)*ROWS + y; // right ""
    thiscell->neighbours[2] = x*ROWS + y+1; //bottom ""
    thiscell->neighbours[3] = (x-1)*ROWS + y; // left ""
    return thiscell;
}

void create_grid()
{
    int i,j,cell_number=0;
    for(i=0;i<COLUMNS;++i)
    {
        for(j=0;j<ROWS;++j)
        {
            cells[cell_number] = create_cell(i,j); // It's index (cell_number) will be equivalent to y + x * no_of_rows
            ++cell_number; 
        }
    }
    cells[cell_number] = NULL;
}

void remove_walls(struct cell * first, struct cell * second)
{
    if (abs(first->x_position - second->x_position)==0)
    {
        if(first->y_position - second->y_position==1) 
        {
            first->walls[0] = 0;
            second->walls[2] = 0;
            }
        else 
        {
            first->walls[2] = 0;
            second->walls[0] = 0;
        }
    }
    else{
        if((first->x_position - second->x_position)==1)
        { 
            first->walls[3] = 0;
            second->walls[1] = 0;
        }
        else
        {
            first->walls[1] = 0;
            second->walls[3] = 0;
        }
    }
}

void store_stack()
{

    int count =0;
    head->prev = NULL;
    while(head->next != NULL){
        route[count] = head->grid_cell;
        head = head->next;
        count += 1;
    }
    route[count] = NULL;

    while(head->prev != NULL){
        head = head->prev;
    }
}

int neighbour(struct cell * current, int i)
{
    if (current->neighbours[i] > 0 && current->neighbours[i] < (ROWS*COLUMNS) && !(i==0 && current->y_position == 0) && !(i==2 && current->y_position == ROWS - 1)){
        return 1; // This checks that the index of neighbour actually borders the cell (doesn't 'wrap' around to bottom/otherside)
    }
    return 0;
}

void visit_cells()
{
    struct cell * current;
    current = cells[0];
    current->walls[3] =0; // Entrance
    cells[ROWS*COLUMNS-1]->walls[1] = 0; // Exit
    do
    {
        struct cell * neighbours[5];
        int no_neighbours = 0;
        int i =0;

        for (i=0;i<4;++i){
            if(neighbour(current,i)==1 && cells[current->neighbours[i]]->visited==0)
            {
                neighbours[no_neighbours] = cells[current->neighbours[i]];
                no_neighbours += 1;
            }
        }

        if(no_neighbours!=0)
        { 
            head = push(head, current);
            int r = rand()%no_neighbours;
            remove_walls(current, neighbours[r]);
            current = neighbours[r];
            current->visited = 1;
        }
        else
        {
            if (current != head->grid_cell) head = push(head,current);
            if(current == cells[ROWS*COLUMNS-1]) store_stack(); 
            head = pop(head);
            current = head->grid_cell;
        }
    }while (current != cells[0]);
}

void increase_difficulty(){
    int i;
    for(i=0;i<ROWS;++i)
    {
        int r = rand()%(ROWS*COLUMNS-1);
        if (((r % ROWS != 0) && (r+1) %ROWS != 0) && r >= ROWS && r<(ROWS*(COLUMNS-1)-1)){
            cells[r]->walls[rand()%3] = 0;
        }
    }
}

void draw_space()
{
    setRGBColour(0, 82, 204);
    fillRect(0,0,CANVAS_WIDTH, CANVAS_HEIGHT);
    int i;
    setColour(yellow);
    for (i=0;i<50;++i){
        fillRect(rand()%CANVAS_WIDTH, rand()%CANVAS_HEIGHT,rand()%4,rand()%4);
    }
    setColour(white);
    drawOval(10,10,10,10); //Draw astronaut
    drawOval(5,5,20,20);
    drawLine(15,20,15,40);
    drawLine(10,27,20,27);
    drawLine(15,40,10,45);
    drawLine(15,40,20,45);
    drawOval(CANVAS_WIDTH - 40, CANVAS_HEIGHT -40, 30,30); // Draw moon
    drawOval(CANVAS_WIDTH - 35, CANVAS_HEIGHT - 35, 5,5);
    drawOval(CANVAS_WIDTH - 20, CANVAS_HEIGHT -30, 10,10);
    drawOval(CANVAS_WIDTH - 30, CANVAS_HEIGHT -25, 7,7);
    drawString("Help the astronaut get to the moon!", 10, CANVAS_HEIGHT - 20);

}

void draw_princess()
{
    setRGBColour(255, 204, 255);
    fillRect(0,0,CANVAS_WIDTH, CANVAS_HEIGHT);
    setColour(black);
    drawOval(10,20,10,10); //Draw princess
    drawLine(15,30,5,50);
    drawLine(15,30,25,50);
    drawLine(5,50,25,50);

    drawLine(10,20,20,20); // crown
    drawLine(10,15,10,20);
    drawLine(20,15,20,20);
    drawLine(10,15,13,17);
    drawLine(13,17,15,12);
    drawLine(15,12,17,17);
    drawLine(17,17,20,15);

    setColour(gray); // castle
    fillRect(CANVAS_WIDTH-35, CANVAS_HEIGHT-30, 30,20);
    fillRect(CANVAS_WIDTH-35, CANVAS_HEIGHT-42, 5,12);
    fillRect(CANVAS_WIDTH-10, CANVAS_HEIGHT-42, 5,12);
    fillRect(CANVAS_WIDTH-25, CANVAS_HEIGHT-40, 10,10);
    setColour(black);
    fillRect(CANVAS_WIDTH-22, CANVAS_HEIGHT-15,4,5);

    drawString("Help the princess get to her castle!", 10, CANVAS_HEIGHT - 20);

}

void draw_route(){
    int i;
    setColour(red);
    for(i=0;route[i]!= NULL;++i)
    {
        fillRect(route[i]->x_position*GRID_SIZE+OFFSET-GRID_SIZE/2,route[i]->y_position*GRID_SIZE+OFFSET-GRID_SIZE/2,GRID_SIZE, GRID_SIZE);
    }
}


void draw_grid(int argc, char * argv[])
{
    int i, xcoordinate, ycoordinate, topleftx, toplefty;
    if (argc>1) {
        if (strcmp("space", argv[1])==0) setColour(white);
        else setColour(black);
    }
    

    for(i=0;cells[i]!=NULL;i++){
        topleftx = ((cells[i]->x_position)*GRID_SIZE)+OFFSET-GRID_SIZE/2; //Need to offset because positions give centre of square
        toplefty = ((cells[i]->y_position)*GRID_SIZE)+OFFSET-GRID_SIZE/2;
        if (cells[i]->walls[0] ==1) drawLine(topleftx, toplefty, topleftx+GRID_SIZE, toplefty);
        if (cells[i]->walls[3]== 1) drawLine(topleftx, toplefty, topleftx          , toplefty+GRID_SIZE);
        if (cells[i]->walls[2] == 1)drawLine(topleftx, toplefty+GRID_SIZE, topleftx+GRID_SIZE, toplefty +GRID_SIZE); //
        if (cells[i]->walls[1] == 1)drawLine(topleftx + GRID_SIZE, toplefty+ GRID_SIZE, topleftx + GRID_SIZE, toplefty);
        }
    drawString("->", topleftx + GRID_SIZE, toplefty+ GRID_SIZE*0.75);

}


int main(int argc, char *argv[])
{
    setWindowSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    //UNCOMMENT THE LINE BELOW TO SEED RANDOM
    //srand(time(&t));     
    create_grid();
    visit_cells();
    if (GRID_SIZE <= 15) increase_difficulty();

    int show_route = 0;
    if (argc>1){
        if (strcmp("space", argv[1])==0) draw_space();
        else if (strcmp("princess", argv[1])==0) draw_princess();
        if (strcmp("show", argv[argc-1])==0) draw_route();
    }

    draw_grid(argc, argv);
    return 0;
}