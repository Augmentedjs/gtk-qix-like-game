#include <gtk/gtk.h>
#include <cairo.h>
#include <stdlib.h>
#include <time.h>

#define TRAIL_COUNT 5

// Line properties
static double line_x1, line_y1, line_x2, line_y2;
static double dx1, dy1, dx2, dy2;
static int width = 400, height = 400;

typedef struct {
    double x1, y1, x2, y2;
    double opacity;
} Trail;

static Trail trails[TRAIL_COUNT];

static void initialize_positions_and_directions() {
    // Seed the random number generator
    srand(time(NULL));

    // Initialize line positions randomly within the window bounds
    line_x1 = rand() % width;
    line_y1 = rand() % height;
    line_x2 = rand() % width;
    line_y2 = rand() % height;

    // Initialize direction vectors randomly (-2, -1, 1, 2)
    dx1 = (rand() % 3 + 1) * (rand() % 2 ? 1 : -1);
    dy1 = (rand() % 3 + 1) * (rand() % 2 ? 1 : -1);
    dx2 = (rand() % 3 + 1) * (rand() % 2 ? 1 : -1);
    dy2 = (rand() % 3 + 1) * (rand() % 2 ? 1 : -1);

    // Initialize trails
    for (int i = 0; i < TRAIL_COUNT; i++) {
        trails[i].x1 = line_x1;
        trails[i].y1 = line_y1;
        trails[i].x2 = line_x2;
        trails[i].y2 = line_y2;
        trails[i].opacity = 0.0;
    }
}

static void draw_line(cairo_t *cr, double x1, double y1, double x2, double y2, double opacity) {
    // Set the color for the line (red in this case)
    cairo_set_source_rgba(cr, 1.0, 0.0, 0.0, opacity);
    // Set the line width
    cairo_set_line_width(cr, 2.0);

    // Move to the starting point of the line
    cairo_move_to(cr, x1, y1);
    // Draw a line to the end point
    cairo_line_to(cr, x2, y2);

    // Stroke the line to actually draw it
    cairo_stroke(cr);
}

static void on_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    // Draw the trails
    for (int i = 0; i < TRAIL_COUNT; i++) {
        if (trails[i].opacity > 0) {
            draw_line(cr, trails[i].x1, trails[i].y1, trails[i].x2, trails[i].y2, trails[i].opacity);
        }
    }
    // Draw the current line
    draw_line(cr, line_x1, line_y1, line_x2, line_y2, 1.0);
}

static gboolean on_timeout(gpointer user_data) {
    GtkWidget *drawing_area = GTK_WIDGET(user_data);

    // Update line position for first point
    line_x1 += dx1;
    line_y1 += dy1;

    // Bounce off the walls for first point
    if (line_x1 <= 0 || line_x1 >= width) dx1 = -dx1;
    if (line_y1 <= 0 || line_y1 >= height) dy1 = -dy1;

    // Update line position for second point
    line_x2 += dx2;
    line_y2 += dy2;

    // Bounce off the walls for second point
    if (line_x2 <= 0 || line_x2 >= width) dx2 = -dx2;
    if (line_y2 <= 0 || line_y2 >= height) dy2 = -dy2;

    // Update trails
    for (int i = TRAIL_COUNT - 1; i > 0; i--) {
        trails[i] = trails[i - 1];
        trails[i].opacity -= 0.2; // Decrease opacity for fading effect
    }
    trails[0].x1 = line_x1;
    trails[0].y1 = line_y1;
    trails[0].x2 = line_x2;
    trails[0].y2 = line_y2;
    trails[0].opacity = 1.0;

    // Queue redraw of the drawing area
    gtk_widget_queue_draw(drawing_area);

    // Continue calling this function
    return G_SOURCE_CONTINUE;
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *drawing_area;

    // Initialize random positions and directions
    initialize_positions_and_directions();

    // Create a new window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "QIX Style Line Drawing with Trails");
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);

    // Create a drawing area widget
    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, width, height);
    gtk_window_set_child(GTK_WINDOW(window), drawing_area);

    // Connect the draw event to the callback function
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), on_draw, NULL, NULL);

    // Add a timeout to update the line position regularly
    g_timeout_add(16, on_timeout, drawing_area);

    // Present the window
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Create a new GTK application
    app = gtk_application_new("com.example.qixline", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Run the application
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Clean up
    g_object_unref(app);

    return status;
}
