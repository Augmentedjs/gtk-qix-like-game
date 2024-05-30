#include <gtk/gtk.h>
#include <cairo.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TRAIL_COUNT 5
#define COLOR_COUNT 15

// Line properties
static double line_x1, line_y1, line_x2, line_y2;
static double dx1, dy1, dx2, dy2;
static int width = 640, height = 480;
static double offset = 5.0;

// Color properties
static int color_index = 0;
static double colors[COLOR_COUNT][3] = {
  {0.0, 0.0, 0.5},    // Blue
  {0.0, 0.5, 0.0},    // Green
  {0.0, 0.5, 0.5},    // Cyan
  {0.5, 0.0, 0.0},    // Red
  {0.5, 0.0, 0.5},    // Magenta
  {0.5, 0.5, 0.0},    // Brown
  {0.75, 0.75, 0.75}, // Light Gray
  {0.5, 0.5, 0.5},    // Dark Gray
  {0.5, 0.5, 1.0},    // Light Blue
  {0.5, 1.0, 0.5},    // Light Green
  {0.5, 1.0, 1.0},    // Light Cyan
  {1.0, 0.5, 0.5},    // Light Red
  {1.0, 0.5, 1.0},    // Light Magenta
  {1.0, 1.0, 0.5},    // Yellow
  {1.0, 1.0, 1.0}     // White
};

// Flag to indicate if the application is running
static gboolean app_running = TRUE;

// Timeout ID
static guint timeout_id;

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
  size_t i = 0;
  for (i = 0; i < TRAIL_COUNT; i++) {
    trails[i].x1 = line_x1;
    trails[i].y1 = line_y1;
    trails[i].x2 = line_x2;
    trails[i].y2 = line_y2;
    trails[i].opacity = 0.0;
  }
}

static void draw_background(cairo_t *cr) {
  // Set the background color to black
  cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
  cairo_paint(cr);
}

static void draw_text(cairo_t *cr) {
  // Set the color for the text (dark gray)
  cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
  cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, 48);

  // Get the text extents to center it
  cairo_text_extents_t extents;
  cairo_text_extents(cr, "QIX", &extents);

  // Calculate the position to center the text
  double x = (width - extents.width) / 2 - extents.x_bearing;
  double y = (height - extents.height) / 2 - extents.y_bearing;

  // Move to the position and show the text
  cairo_move_to(cr, x, y);
  cairo_show_text(cr, "QIX");
}

static void draw_line(cairo_t *cr, double x1, double y1, double x2, double y2, double opacity, double width, int color_index) {
  // Set the color for the line
  cairo_set_source_rgba(cr, colors[color_index][0], colors[color_index][1], colors[color_index][2], opacity);
  // Set the line width
  cairo_set_line_width(cr, width);

  // Move to the starting point of the line
  cairo_move_to(cr, x1, y1);
  // Draw a line to the end point
  cairo_line_to(cr, x2, y2);

  // Stroke the line to actually draw it
  cairo_stroke(cr);
}

static void on_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
  // Draw the background
  draw_background(cr);

  // Draw the text
  draw_text(cr);

  // Draw the trails first
  size_t i = 0;
  for (i = 0; i < TRAIL_COUNT; i++) {
    if (trails[i].opacity > 0) {
      draw_line(cr, trails[i].x1, trails[i].y1, trails[i].x2, trails[i].y2, trails[i].opacity, 1.0, color_index);
    }
  }
  // Draw the current line
  draw_line(cr, line_x1, line_y1, line_x2, line_y2, 1.0, 2.0, color_index);
}

static gboolean on_timeout(gpointer user_data) {
  GtkWidget *drawing_area = GTK_WIDGET(user_data);

  if (!GTK_IS_WIDGET(drawing_area) || !app_running) {
    return G_SOURCE_REMOVE;
  }

  // Update line position for first point
  line_x1 += dx1;
  line_y1 += dy1;

  // Bounce off the walls for first point
  if (line_x1 <= 0 || line_x1 >= width) {
    dx1 = -dx1;
    color_index = (color_index + 1) % COLOR_COUNT;
  }
  if (line_y1 <= 0 || line_y1 >= height) {
    dy1 = -dy1;
    color_index = (color_index + 1) % COLOR_COUNT;
  }

  // Update line position for second point
  line_x2 += dx2;
  line_y2 += dy2;

  // Bounce off the walls for second point
  if (line_x2 <= 0 || line_x2 >= width) {
    dx2 = -dx2;
    color_index = (color_index + 1) % COLOR_COUNT;
  }
  if (line_y2 <= 0 || line_y2 >= height) {
    dy2 = -dy2;
    color_index = (color_index + 1) % COLOR_COUNT;
  }

  // Update trails
  size_t i = 0;
  for (i = TRAIL_COUNT - 1; i > 0; i--) {
    trails[i] = trails[i - 1];
    trails[i].opacity -= 1.0 / TRAIL_COUNT; // Decrease opacity for fading effect
  }

  // Set the position for the first trail
  trails[0].x1 = line_x1;
  trails[0].y1 = line_y1;
  trails[0].x2 = line_x2;
  trails[0].y2 = line_y2;
  trails[0].opacity = 1.0;

  // Apply offset to the remaining trails
  for (i = 1; i < TRAIL_COUNT; i++) {
    trails[i].x1 = trails[i - 1].x1 - offset * dx1 / sqrt(dx1 * dx1 + dy1 * dy1);
    trails[i].y1 = trails[i - 1].y1 - offset * dy1 / sqrt(dx1 * dx1 + dy1 * dy1);
    trails[i].x2 = trails[i - 1].x2 - offset * dx2 / sqrt(dx2 * dx2 + dy2 * dy2);
    trails[i].y2 = trails[i - 1].y2 - offset * dy2 / sqrt(dx2 * dx2 + dy2 * dy2);
  }

  // Queue redraw of the drawing area
  gtk_widget_queue_draw(drawing_area);

  // Continue calling this function
  return G_SOURCE_CONTINUE;
}

static void on_window_destroy(GtkWidget *widget, gpointer user_data) {
  app_running = FALSE;
  if (timeout_id > 0) {
    g_source_remove(timeout_id);
    timeout_id = 0;
  }
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

  // Disable window resizing
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

  // Create a drawing area widget
  drawing_area = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawing_area, width, height);
  gtk_window_set_child(GTK_WINDOW(window), drawing_area);

  // Connect the draw event to the callback function
  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), on_draw, NULL, NULL);

  // Add a timeout to update the line position regularly
  timeout_id = g_timeout_add_full(G_PRIORITY_DEFAULT, 16, on_timeout, drawing_area, NULL);

  // Connect the destroy signal to stop the timeout when the window is closed
  g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

  // Present the window
  gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  // Create a new GTK application
  app = gtk_application_new("com.augmented.qixline", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  // Run the application
  status = g_application_run(G_APPLICATION(app), argc, argv);

  // Clean up
  g_object_unref(app);

  return status;
}
