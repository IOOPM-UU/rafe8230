#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define PI 3.14159265358979323846

typedef struct point
{
  int x;
  int y;
} point_t;

typedef struct rectangle
{
    point_t upper_left; 
    point_t lower_right;
} rectangle_t;

typedef struct circle
{
    point_t str_m;
    int radius;
} circle_t;

int max(int a, int b)
{
    return a == b ? a : a > b ? a : b;
}

int min(int a, int b)
{
    return a == b ? a : a < b ? a : b;
}

void print_point(point_t *point) 
{
    printf("point(x: %d, y: %d)       ", point->x, point->y);
}

point_t make_point(int x, int y)
{
    point_t p = { .x = x, .y = y};
    return p; 
}

void print_rect(rectangle_t *rect)
{
    printf("upper left: ");
    print_point(&rect->upper_left);
    printf("lower right: ");
    print_point(&rect->lower_right);
    printf("\n");
}

rectangle_t make_rect(int ul_x, int ul_y, int lr_x, int lr_y)
{
    point_t ul_p = make_point(ul_x, ul_y);
    point_t lr_p = make_point(lr_x, lr_y);
    rectangle_t rect = { .lower_right = lr_p, .upper_left = ul_p};
    return rect;
}

int area_rect(rectangle_t *rect)
{
    int base = abs(rect->upper_left.x - rect->lower_right.x);
    int height = abs(rect->lower_right.y - rect->upper_left.y);

    return base * height;
}

bool intersects_rect(const rectangle_t *rect1, const rectangle_t *rect2)
{
    return rect1->upper_left.y <= rect2->lower_right.y &&
           rect1->lower_right.y >= rect2->upper_left.y && // checks overlap in y-coordinates

           rect1->lower_right.x >= rect2->upper_left.x &&
           rect1->upper_left.x <= rect2->lower_right.x; // checks overlap in x-coordinates

}

int intersecition_rects(const rectangle_t *rect1, const rectangle_t *rect2)
{

    int non_intersection = 0;
    if (!intersects_rect(rect1, rect2)) return non_intersection;

    point_t top_left = { .y = max(rect1->upper_left.y, rect2->upper_left.y), 
                         .x = max(rect1->upper_left.x, rect2->upper_left.x)};

    point_t bottom_right = { .y = min(rect1->lower_right.y, rect2->lower_right.y),
                             .x = min(rect1->lower_right.x, rect2->lower_right.x)};

    rectangle_t result = { .lower_right = bottom_right, .upper_left = top_left}; 
    return area_rect(&result); 
}

float area_circle(circle_t *crl)
{
    return PI * crl->radius * crl->radius;  
}

void print_circle(circle_t *crl)
{
    printf("circle(center = point(%d, %d), radius = %d, Area = %.3f )\n", crl->str_m.x, crl->str_m.y, crl->radius, area_circle(crl));
}

circle_t make_circle(int x, int y, int r)
{
    circle_t result = { .radius = r, .str_m.x = x, .str_m.y = y}; 
    return result; 
}


int main(void)
{
    // rectangle_t rect1 = make_rect(9, 9, 1, 1); 
    // rectangle_t rect2 = make_rect(0, 0, 10, 10); // rect1 inside rect2
    
    // rectangle_t rect3 = make_rect(0, 0, 10, 10);
    // rectangle_t rect4 = make_rect(8, 8, 15, 15); // intersection in bottom left corner of rect3

    // rectangle_t rect5 = make_rect(0, 0, 10, 10);
    // rectangle_t rect6 = make_rect(11, 11, 20, 20); // no collision

    // rectangle_t rect7 = make_rect(0, 0, 7, 7);
    // rectangle_t rect8 = make_rect(1, 1, 8, 8); 
    // printf("%d\n", intersecition_rects(&rect7, &rect8));

    
    circle_t crl = make_circle(0, 0, 10); 
    print_circle(&crl);
    


    return 0;
}
