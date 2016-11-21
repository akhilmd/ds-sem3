#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "./include/Errors.h"
#include "./include/Node.h"
#include "./include/Graph.h"
#include "./include/Heap.h"
#include "./include/PriorityQueue.h"
#include "./include/AStar.h"
#include "./include/GUI.h"

// Used to indicate whether start and end are selected.
int hasStart = 0;
int hasEnd = 0;

int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("in.akhil.ds-project", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}

static void activate (GtkApplication* app, gpointer user_data)
{
    GtkWidget *window = NULL;
    GtkWidget *image = NULL;
    GtkWidget* event_box = NULL;

    GdkPixbuf* ipb = NULL;
    // Path to be at most 100 chars
    char fil_n[100] = {0};
    int err = 0;
    printf("Enter Path to Map: ");
    scanf("%s",fil_n);
    // get image and pixel_buffer from the file
    image  = loadImgMap(fil_n, &ipb);

    // main map for the astar algorithm
    Map* in_map = NULL;

    // Initialize the Map with the image and pixel buffer.
    if (err = astar_map_init(&in_map, ipb, image))
        printf("ERROR: %d\n", err);

    // Add image to an event box
    event_box = gtk_event_box_new ();
    gtk_container_add (GTK_CONTAINER (event_box), image);
 
    printf("PLEASE SELECT A START POINT\n");

    // Whenever a click is detected, call set_pos() with arg in_map
    g_signal_connect (G_OBJECT (event_box), "button_press_event", G_CALLBACK (set_pos), in_map);

    // Gtk - gui stuff
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "A* - Data Structures");
    gtk_window_set_default_size (GTK_WINDOW (window), 0, 0);
    gtk_container_add (GTK_CONTAINER (window), event_box);
    gtk_window_set_resizable(window, FALSE);

    gtk_widget_show_all (window);
}

GtkWidget* loadImgMap(char* path, GdkPixbuf** pix_buf)
{
    // get pixel buffer from file
    *pix_buf = gdk_pixbuf_new_from_file(path, NULL);

    // return gtk image widget
    return gtk_image_new_from_pixbuf(*pix_buf);
}

int astar_map_init(Map** map, GdkPixbuf* pix_buf, GtkWidget* image)
{
    int err = 0;

    int h = gdk_pixbuf_get_height(pix_buf);
    int w = gdk_pixbuf_get_width(pix_buf);

    int width, height, rowstride, n_channels;
    guchar *pixels, *p;

    // needed for getting the pixel values
    n_channels = gdk_pixbuf_get_n_channels (pix_buf);
    rowstride = gdk_pixbuf_get_rowstride (pix_buf);
    pixels = gdk_pixbuf_get_pixels (pix_buf);

    // allocate memory for a map
    Map* new_map = malloc(sizeof(Map));
    // initalize members
    new_map->graph = NULL;
    new_map->pix_buf = pix_buf;
    new_map->image = image;

    // initialize open_list as a Priority Queue (Implemented as a Binary Heap)
    if (err = pq_init(&(new_map->open_list)))
        return err;

    // initalize the graph (nodes stored as 2D array)
    if (err = graph_init(&(new_map->graph),h,w))
        return err;

    int i = 0, j = 0, a = 0, b = 0;
    char tmp = 0;
    Node* tmp_node = NULL;

    for (i=0;i<h;++i)
    {
        for (j=0;j<w;++j)
        {
            // pixels are stored row-wose and then channel wise
            // p is the array of rgb values for each pixel at i,j index.
            p = pixels + j * rowstride + i * n_channels;

            // black pixels are non traversable and are represented by '#'
            if (is_black(p))
                tmp = '#';
            // all other pixels are traversable and are shown as '-'
            else
                tmp = '-';

            // initalize the node for that pixel with co-ordinates
            if (err = node_init(&tmp_node,i,j))
                return err;
            // set key
            tmp_node->key = tmp;
            // set initial prioirity
            tmp_node->priority = -1;

            // add node to the graph
            if (err = graph_add_node(new_map->graph, tmp_node, i, j))
                return err;
        }
    }

    // Add neighbors for each node in graph
    for (i=0;i<h;++i)
    {
        for (j = 0;j<w;++j)
        {
            // for each node
            tmp_node = new_map->graph->nodes[i][j];
            for (a=-1;a<=1;++a)
                for (b=-1;b<=1;++b)
                    // for each of the eight neighbors
                    if (!(a==0 && b==0))
                        // if the neighbor is traversable 
                        if (astar_is_valid(i+a,j+b,new_map))
                            // add an edge between the node and the neighbor
                            if (err = graph_add_edge(tmp_node,new_map->graph->nodes[i+a][j+b]))
                                return err;
        }
    }

    *map = new_map;
    return SUCCESS;
}

int is_black(guchar* p)
{
    // rgb value of black is 0,0,0
    return (p[0] == 0 && p[1] == 0 && p[2] == 0) ? 1 : 0;
}

int astar_is_valid(int i, int j, Map* map)
{
    // if i and j are not valid indices
    if (i<0 || i>=map->graph->height || j<0 || j>=map->graph->width)
        return 0;
    // if node at i,j is not traversable
    if (map->graph->nodes[i][j]->key == '#')
        return 0;
    return 1;
}

static gboolean set_pos(GtkWidget *event_box, GdkEventButton *event, Map* map)
{
    // printf("click: %f\t%f\n", event->x, event->y);

    int x = (int) event->x;
    int y = (int) event->y;

    int i = 0, j = 0, rad = 5;

    int f = 0;
    int err = 0;
    // Get position of valid start position around the clicked location
    for (i=-rad;i<=rad;++i)
    {
        for (j=-rad;j<=rad;++j)
        {
            if (!(x+i < 0 || y+j < 0 || x+i >=map->graph->height || y+j >= map->graph->width) &&  map->graph->nodes[x+i][y+j]->key == '-')
            {
                x = x+i;
                y = y+j;
                f = 1;
                break;
            }
        }
    }

    // If a valid traversable position on the map is clicked, only then attempt to set start or end
    if (f)
    {
        // printf("inds : %d\t%d  :  %c\n",x,y, map->graph->nodes[x][y]->key);
        // If start position is not set, set the clicked position to start.
        if (!hasStart)
        {
            map->start = map->graph->nodes[x][y];
            put_pixel_with_width(map,x,y,206,244,66,10,1);
            hasStart = 1;
            printf("START POINT SELECTED AT (%d, %d)\nPLEASE SELECT AN END POINT\n", map->start->x, map->start->y);
            return TRUE;
        }
        // If end position is not set, set the clicked position to end.
        else if (!hasEnd)
        {
            map->end = map->graph->nodes[x][y];
            put_pixel_with_width(map,x,y,244,66,173,10,1);
            hasEnd = 1;
            printf("END POINT SELECTED AT (%d, %d)\nCLICK ANYWHERE ON THE MAP TO START PLOTTING\n", map->end->x, map->end->y);
            return TRUE;
        }
    }
    else if (!hasStart)
    {
        printf("PLEASE CLICK ON A ROAD (WHITE)\n");
    }

    // If both start and end are set and click happens, start Path-Finding.
    if (hasEnd && hasStart)
    {
        printf("PLOTTING ROUTE\n");

        // Add start position to the Open List (Priority Queue)
        if (err = pq_insert(map->open_list, map->start))
            printf("ERROR: %d\n", err);
        
        map->start->key = 'O';
        put_pixel_with_width(map,map->start->x,map->start->y,0,0,255,2,0);
        
        // Mark end point for text output.
        map->end->key = '$';
        // astar_print_map(map);

        // Main logic loop is called every 1/1000 seconds.
        g_timeout_add(LOOP_DELAY, G_CALLBACK (loop_astar), map);
        return FALSE;
    }
    return TRUE;
}

// funtion to update the GUI 
void put_pixel_with_width (Map* map, int x, int y, guchar red, guchar green, guchar blue, int w, char ig)
{
    int i = 0, j = 0;

    for (i=-w;i<=w;++i)
        for (j=-w;j<=w;++j)
            if (!(x+i < 0 || y+j < 0 || x+i >=map->graph->height || y+j >= map->graph->width) && (ig || map->graph->nodes[x+i][y+j]->key != '#'))
                put_pixel(map,x+i,y+j,red,green,blue);

    gtk_image_set_from_pixbuf(map->image, map->pix_buf);
}

// funtion to update the GUI 
void put_pixel(Map* map, int x, int y, guchar red, guchar green, guchar blue)
{
    GdkPixbuf* pixbuf = map->pix_buf;

    int width, height, rowstride, n_channels;
    guchar *pixels, *p;

    n_channels = gdk_pixbuf_get_n_channels (pixbuf);

    g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
    g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
    g_assert (n_channels == 3);

    width = gdk_pixbuf_get_width (pixbuf);
    height = gdk_pixbuf_get_height (pixbuf);

    g_assert (x >= 0 && x < width);
    g_assert (y >= 0 && y < height);

    rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels (pixbuf);

    p = pixels + y * rowstride + x * n_channels;
    p[0] = red;
    p[1] = green;
    p[2] = blue;
}

static gboolean loop_astar(Map* in_map)
{
    // if the open list is empty, then no route is found.
    if (in_map->open_list->size == 0)
    {
        printf("NO ROUTE!\n");
        return FALSE;
    }

    int err = 0;

    // Variable to hold the current node.
    // The current node is always the node with least (distance_from_start + approx_from_end) value.
    Node* current_node = NULL;

    // Get High Priority Element from the Priority Queue.
    if (err = pq_pull_highest_priority_element(in_map->open_list,&current_node))
        printf("ERROR: %d\n", err);

    // If we find the end, follow parent pointers till start to get path.
    if (current_node == in_map->end)
    {
        printf("ROUTE HAS BEEN FOUND IN GREEN\n");
        // follow parent pointers till Start.
        Node* tmp  = current_node;

        for (;;)
        {
            // Set key for text output.
            tmp->key = '*';
            // Color the path in Green.
            put_pixel_with_width(in_map,tmp->x,tmp->y,0,255,0,2,0);

            // If we are at the start, path has been generated.
            if (tmp == in_map->start)
                break;

            // Go to the parent.
            tmp = tmp->parent;
        }

        // Show map as text.
        // astar_print_map(in_map);

        // Gtk will stop executing loop_astar
        return FALSE;
    }

    // We did not reach the end node.
    // Add neighbors of current Node to the open list.
    if (err = astar_add_neighbors_ol(in_map, current_node, in_map->end))
        printf("ERROR: %d\n", err);

    // Add the current Node to the closed set
    // Change color to red to indicate presence in closed set
    put_pixel_with_width(in_map,current_node->x,current_node->y,255,0,0,2,0);
    // chane key to 'C' to indicate presence in closed set
    current_node->key = 'C';

    // Gtk will continue to execute this function till it returns false.
    return TRUE;
}

int astar_add_neighbors_ol(Map* map, Node* node, Node* goal)
{
    int i = 0, err = 0;

    Node* tmp = NULL;

    // level is distance from the start.
    // since a neighbor is 1 unit away from its parent,
    // new level will be 1 + parent level
    int new_level = node->level + 1;

    // For each neighbor
    for (i=0;i<node->num_neighbors;++i)
    {
        tmp = node->neighbors[i];

        // If it is in the closed set, ignore this neighbor.
        if (tmp->key == 'C')
            continue;

        // If it is already in the open list AND
        // the distance of the new path to it is greater than the old path, 
        // ignore this neighbor.
        if (tmp->key=='O' && new_level >= tmp->level)
            continue;

        // this neighbor is either not in the open list OR
        // a better path to this neighbor in the open list was found.

        // Add neighbor to the open list

        // change key to indicate presence in open list
        tmp->key='O';
        // change volor to blue to indicate presence in openlist
        put_pixel_with_width(map,tmp->x,tmp->y,0,0,255,2,0);
        // set the neighbor's parent to the node
        tmp->parent = node;
        // update distance from start
        tmp->level = new_level;
        // calculate approximate distance to end using heuristic
        // and update the priority.
        tmp->priority = tmp->level + astar_heuristic(tmp,goal);

        // Add it to the open list Priority Queue.
        if (err = pq_insert(map->open_list,tmp))
            return err;
    }

    return SUCCESS;
}

// Heuristic funtion to find an esimate to the distance to the end point
// Here, we use the Euclidean distance as the heuristic function
// Other heuristic functions are: Manhattan Distance, Diagonal Distance
// If the heuristic function results in a value that is <= the actual distance,
// Then it will always result in a shortest path.
// the closer the estimate to the actual distance the faster it is.
// If the heuristic returns 0, the algorithm becomes Dijkstra’s Algorithm.
float astar_heuristic(Node* s, Node* e)
{
    // Euclidean Distance
    return sqrt(pow(s->x - e->x,2)+pow(s->y - e->y,2));

    // Diagonal Distance
    // int dx = abs(s->x - e->x);
    // int dy = abs(s->y - e->y);
    // float D = 1, D2 = 1;
    // return (float)(D * (dx + dy) + (D2 - 2 * D) * (dx<dy?dx:dy));

    // Manhattan Distance
    // return (float) (abs(s->x - e->x)+abs(s->y - e->y));

    // Dijkstra’s Algorithm
    // return 0.0;
}

void astar_print_map(Map* map)
{
    int i = 0, j = 0;

    for (i=0;i<map->graph->height;++i)
    {
        for(j=0;j<map->graph->width;++j)
            printf("%c", map->graph->nodes[i][j]->key);
        printf("\n");
    }

}