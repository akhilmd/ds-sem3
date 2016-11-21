// Function called on application startup
static void activate (GtkApplication*, gpointer);

// Funtion to load image and pixel buffer required for the map
GtkWidget* loadImgMap(char*, GdkPixbuf**);

// Function to check if a given pixel is black
int is_black(guchar*);

// Function executed everytime a mouse button is pressed
static gboolean set_pos(GtkWidget *, GdkEventButton *, Map*);

// Functions used to change the pixel_buffer and generate new image 
// to update GUI to new state of the Map to indicate the progress
// of the algorithm
void put_pixel_with_width (Map*, int, int, guchar, guchar, guchar, int, char);
void put_pixel(Map*, int, int, guchar, guchar, guchar);

// Main logic loop of the A* algorithm is called every 1/1000 seconds.
static gboolean loop_astar(Map*);