/** looker: yet another video player which can remember
  * the position where you stop looking video to reopen it later.
  * Copyright (C) 2017 Brüggemann Eddie
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  ***********************************************************************/

#include "./headers/includes.h"

#include "./headers/defines.h"

#include "./GtkSmartMenuItem.h"

#include "./video_callbacks.h"
#include "./global_vars.h"
#include "./player_callbacks.h"
#include "./error_message.h"

#include "./configuration.h"

/** ************************ Defines local variable ************************ **/

#ifdef MAC_INTEGRATION
gint attention = -1 ;
#endif

/** ************************************************************************ **/

/** GtkApplication open callback function: for cmdline file(s) or folder opening. **/
static void open_files(GApplication  *application, GFile **files, gint n_files, const gchar *hint) ;

/** GtkApplication activate callback function. **/
static void activate(GApplication *application) ;

/** GtkApplication startup callback function. **/
static void startup(GApplication *application, gpointer user_data) ;

/** GtkApplication shutdown callback function. **/
static void shutdown_app(GApplication *application, gpointer user_data) ;

#if 0
/** GtkApplication (application menu) open file callback function. **/
/** Cannot get controls and window at the same time which are twice needed by opening a file. **/
static void open_file_activated(GSimpleAction *action, GVariant *parameter, gpointer app) ;
#endif

/** ************************************************************************ **/

/** GtkApplication (application menu) About callback function. **/
static void preferences_activated(GSimpleAction *action, GVariant *parameter, gpointer app) ;

static void preferences_callback(GtkWidget *widget, GtkApplication *app) ;

static void about_activated(GSimpleAction *action, GVariant *parameter, gpointer app) ;

static void about_callback(GtkWidget *widget, GtkApplication *app) ;

static void display_about_dialog(GtkWidget *widget, GtkApplication *app) ;

static void quit_activated(GSimpleAction *action, GVariant *parameter, gpointer app) ;

/** ************************************************************************ **/


static void set_as_default_movie_player(GtkWidget *widget, GtkApplication *app) ;

static void reset_to_default(GtkWidget *widget) ;


static void compute_controls_size(GtkWidget *window, GtkWidget *controls) ;


static void player_widget_on_realize(GtkWidget *widget) ;

static gboolean draw_window_bg(GtkWidget *widget, cairo_t *cr) ;

static void on_size_allocated(GtkWidget *window, GdkRectangle *allocation, GtkWidget *controls) ;

static gboolean on_key_press(GtkWidget *window, GdkEventButton  *event, GtkWidget *controls) ;

static gboolean on_configure_window(GtkWidget *window, GdkEvent  *event, GtkWidget *controls) ;


static gchar* timeline_format_value(GtkScale *scale, gdouble value) ;

static gboolean timeline_change_value(GtkRange *range, GtkScrollType scroll, gdouble value) ;

static gboolean timeout_timeline(GtkWidget *widget) ;


static void flush_bookmark_file(GtkWidget *widget) ;


static gboolean hide_controls_on_iconify(GtkWidget *window, GdkEventWindowState *event,GtkWidget *controls) ;


static void destroy(GtkWidget *window, GtkWidget *controls) ;

static gboolean delete_event(GtkWidget *widget,GdkEvent *event) ;


int main(int argc, char *argv[]) {

  g_rec_mutex_init(&mainloop_ctrl_rec_mutex) ;

  bookmark = g_bookmark_file_new();

  if (! g_file_test(PATH_TO_BOOKMARK, G_FILE_TEST_EXISTS)) {

      gchar *bookmark_directory = g_path_get_dirname(PATH_TO_BOOKMARK) ;

      g_mkdir_with_parents(bookmark_directory, 0777);

      g_creat(PATH_TO_BOOKMARK, 0777);

  }

  snapshot_location = g_strdup(g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP)) ;



  #ifdef MAC_INTEGRATION /** The library gtk-mac-integration-gtk3 define this. **/

  attention = gtkosx_application_attention_request(gtkosx_application_get(), CRITICAL_REQUEST);

  gtkosx_application_ready(gtkosx_application_get()) ;

  /** I don't know if it is a good idea to set it on @TRUE, because the radio and check buttons doesn't work with value @TRUE. **/
  gtkosx_application_set_use_quartz_accelerators(gtkosx_application_get(), TRUE) ;

  #ifdef INFO
  fprintf(stdout,"Mac app use quartz accels: %d\n", gtkosx_application_use_quartz_accelerators(gtkosx_application_get()) ;
  #endif

  g_set_prgname(PRGNAME) ; /** Not automatically set from the GtkApplication. **/

  #endif

  const char *app_id = "looker.mrcyberfighter" ; /** @Hint: For using Gtk notification you will have create a *.desktop file named has the id (prgname.org in this case) and getting a dbus connection. ; **/

  if ( ! g_application_id_is_valid(app_id) ) {

    fprintf(stderr, "Wrong app id\n") ;
    exit(EXIT_FAILURE) ;

  }

  int app_flags = G_APPLICATION_NON_UNIQUE | G_APPLICATION_SEND_ENVIRONMENT | G_APPLICATION_HANDLES_OPEN ;

  GtkApplication *app = gtk_application_new(app_id, app_flags) ;

  bool registered = g_application_register(G_APPLICATION(app), NULL, &error) ;

  if (error != NULL || (! registered)) {

    fprintf(stderr,"Cannot register app: %s\n", error->message) ;

    g_clear_error(&error) ;

    exit(EXIT_FAILURE) ;

  }






  #ifdef G_OS_UNIX

  g_signal_connect( G_APPLICATION(app),  "activate",              G_CALLBACK(activate),       NULL) ;
  g_signal_connect( G_APPLICATION(app),  "open",                  G_CALLBACK(open_files),     NULL) ;
  g_signal_connect( G_APPLICATION(app),  "startup",               G_CALLBACK(startup),        NULL) ;
  g_signal_connect( G_APPLICATION(app),  "shutdown",              G_CALLBACK(shutdown_app),   NULL) ;

  if (gtk_application_prefers_app_menu(app)) {

    /** normally if (gtk_application_prefers_app_menu(app)) { ... }
      * but this can be ignore what is better for environments which does not support
      * application menus as said into the documentation.
      *************************************************************************************/

    GtkBuilder *builder = NULL  ;
    GMenuModel *app_menu = NULL ;

    static GActionEntry app_entries[] = {
      #if 0
      { "openfile",     open_file_activated,    NULL, NULL, NULL },
      #endif
      { "preferences",  preferences_activated,  NULL, NULL, NULL },
      { "about",        about_activated,        NULL, NULL, NULL },
      { "quit",         quit_activated,         NULL, NULL, NULL }

    };

    g_action_map_add_action_entries(G_ACTION_MAP(app), app_entries, G_N_ELEMENTS(app_entries), app);

    builder = gtk_builder_new_from_string("<?xml version=\"1.0\"?>"
                                            "<interface>"
                                              "<menu id=\"appmenu\">"
                                              #if 0
                                                "<section>"
                                                  "<item>"
                                                    "<attribute name=\"label\" translatable=\"no\">_Open file</attribute>"
                                                    "<attribute name=\"action\">app.openfile</attribute>"
                                                  "</item>"
                                               "</section>"
                                               #endif
                                               "<section>"
                                                  "<item>"
                                                    "<attribute name=\"label\" translatable=\"no\">_Preferences</attribute>"
                                                    "<attribute name=\"action\">app.preferences</attribute>"
                                                  "</item>"
                                               "</section>"
                                                "<section>"
                                                  "<item>"
                                                    "<attribute name=\"label\" translatable=\"no\">About</attribute>"
                                                    "<attribute name=\"action\">app.about</attribute>"
                                                  "</item>"
                                                "</section>"
                                                "<section>"
                                                  "<item>"
                                                    "<attribute name=\"label\" translatable=\"no\">_Quit</attribute>"
                                                    "<attribute name=\"action\">app.quit</attribute>"
                                                 "</item>"
                                               "</section>"
                                              "</menu>"
                                            "</interface>",

                                            -1) ;

    app_menu = G_MENU_MODEL(gtk_builder_get_object(builder, "appmenu"));

    gtk_application_set_app_menu(GTK_APPLICATION(app), app_menu);

    g_object_unref(builder) ;

  }

  #endif


  g_bookmark_file_load_from_file(bookmark, PATH_TO_BOOKMARK, NULL) ;

  GtkWidget *window = gtk_application_window_new(app)  ;

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS) ;

  get_configuration(GTK_WIDGET(window)) ;


  GtkSettings *gtk_settings_for_screen = gtk_settings_get_for_screen(gtk_window_get_screen(GTK_WINDOW(window))) ;

  g_object_set(G_OBJECT(gtk_settings_for_screen), "gtk-cursor-theme-name", "default", NULL) ;

  GtkCssProvider *provider = NULL ;

  char *theme_name = NULL ;

  g_object_get(G_OBJECT(gtk_settings_for_screen), "gtk-theme-name", &theme_name, NULL) ;

  g_object_unref(gtk_settings_for_screen) ;

  if (theme_name != NULL) {

    provider = gtk_css_provider_get_named(theme_name, "dark") ;

  }
  else {

    provider = gtk_css_provider_get_default() ;

  }

  g_free(theme_name) ;


  if (provider != NULL) {

    GdkDisplay *display = gdk_display_get_default() ;

    GdkScreen *screen = gdk_display_get_default_screen(display) ;

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION) ;

    g_object_unref(provider);

  }



  gtk_window_set_default_icon_from_file(PATH_TO_ICON, NULL) ;


  gtk_window_set_resizable(GTK_WINDOW(window), TRUE) ;





  #ifdef MAC_INTEGRATION

  GdkPixbuf *window_pixbuf = gdk_pixbuf_new_from_file(PATH_TO_ICON, &error) ;

  if (error != NULL) {

    fprintf(stderr,"%s\n", error->message) ;

    g_clear_error(&error) ;

  }

  gtkosx_application_set_dock_icon_pixbuf(gtkosx_application_get(), window_pixbuf) ;
  #endif



  GtkWidget *volume_button = gtk_volume_button_new() ;

  gtk_scale_button_set_value(GTK_SCALE_BUTTON(volume_button), 0.50);

  g_signal_connect(G_OBJECT(volume_button), "value-changed", G_CALLBACK(set_volume), NULL) ;






  GtkWidget *volume_plus_button   = gtk_scale_button_get_plus_button(GTK_SCALE_BUTTON(volume_button));
  GtkWidget *volume_minus_button  = gtk_scale_button_get_minus_button(GTK_SCALE_BUTTON(volume_button));


  GtkWidget *volume_plus_image    = gtk_image_new_from_file(PATH_TO_PLUS_VOLUME) ;

  gtk_button_set_image(GTK_BUTTON(volume_plus_button), volume_plus_image) ;

  GtkWidget *volume_minus_image    = gtk_image_new_from_file(PATH_TO_MINUS_VOLUME) ;

  gtk_button_set_image(GTK_BUTTON(volume_minus_button), volume_minus_image) ;

  GtkWidget *volume_button_image = gtk_image_new_from_file(PATH_TO_VOLUME_BUTTON) ;

  gtk_button_set_image(GTK_BUTTON(volume_button), volume_button_image) ;





  GtkWidget *header_bar = gtk_header_bar_new();

  gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), volume_button);

  gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);

  gtk_window_set_titlebar(GTK_WINDOW(window), header_bar) ;

  //setup vlc
  const char *const args[] = {"--no-xlib", NULL} ;
  vlc_inst = libvlc_new(1, args) ;
  media_player = libvlc_media_player_new(vlc_inst);

  #if 1
  // Gtk set this settings.
  libvlc_video_set_key_input(media_player, 1) ;

  libvlc_video_set_mouse_input(media_player, 1) ;
  #endif

  libvlc_audio_set_volume(media_player, 50) ;



  GtkWidget *controls_bar = gtk_window_new(GTK_WINDOW_TOPLEVEL)  ;

  gtk_window_set_decorated(GTK_WINDOW(controls_bar), FALSE) ;

  gtk_container_set_border_width(GTK_CONTAINER(controls_bar), 8) ;

  gtk_window_set_skip_taskbar_hint(GTK_WINDOW(controls_bar), TRUE) ;




  GtkAccelGroup *accel_group = gtk_accel_group_new() ;


  GtkWidget *open_file = gtk_smart_menu_item_new_all("  Open file", PATH_TO_LOAD_VIDEO, accel_group, GDK_CONTROL_MASK, GDK_KEY_o) ;

  GtkWidget *open_file_menu_item = gtk_smart_menu_item_get_menuitem(open_file) ;

  g_signal_connect(open_file_menu_item, "activate", G_CALLBACK(choose_video), controls_bar) ;


  GtkWidget *make_snapshot  = gtk_smart_menu_item_new_all("  Make a screenshot", PATH_TO_SNAPSHOT, accel_group, GDK_CONTROL_MASK, GDK_KEY_s) ;

  GtkWidget *snapshot_menu_item = gtk_smart_menu_item_get_menuitem(make_snapshot) ;

  g_signal_connect(snapshot_menu_item, "activate", G_CALLBACK(take_snapshot), NULL) ;


  GtkWidget *incr_volume_menu  = gtk_smart_menu_item_new_all("  Increment volume", PATH_TO_PLUS_VOLUME, accel_group, GDK_CONTROL_MASK, GDK_KEY_KP_Add) ;

  GtkWidget *incr_volume_menu_item = gtk_smart_menu_item_get_menuitem(incr_volume_menu) ;

  g_signal_connect(incr_volume_menu_item, "activate", G_CALLBACK(incr_volume), volume_button) ;


  GtkWidget *decr_volume_menu  = gtk_smart_menu_item_new_all("  Decrement volume", PATH_TO_MINUS_VOLUME, accel_group, GDK_CONTROL_MASK, GDK_KEY_KP_Subtract) ;

  GtkWidget *decr_volume_menu_item = gtk_smart_menu_item_get_menuitem(decr_volume_menu) ;

  g_signal_connect(decr_volume_menu_item, "activate", G_CALLBACK(decr_volume), volume_button) ;


  GtkWidget *preferences  = gtk_smart_menu_item_new_all("  Preferences", PATH_TO_PREFERENCES, accel_group, GDK_CONTROL_MASK, GDK_KEY_p) ;

  GtkWidget *preferences_menu_item = gtk_smart_menu_item_get_menuitem(preferences) ;

  g_signal_connect(preferences_menu_item, "activate", G_CALLBACK(preferences_callback), app) ;


  GtkWidget *memory_flush  = gtk_smart_menu_item_new_all("  Clear position storing", PATH_TO_FLUSH_BOOKMARK, accel_group, GDK_CONTROL_MASK, GDK_KEY_c) ;

  GtkWidget *memory_flush_menu_item = gtk_smart_menu_item_get_menuitem(memory_flush) ;

  g_signal_connect(memory_flush_menu_item, "activate", G_CALLBACK(flush_bookmark_file), NULL) ;


  GtkWidget *about  = gtk_smart_menu_item_new_all("  About", PATH_TO_ABOUT, accel_group, GDK_CONTROL_MASK, GDK_KEY_a) ;

  GtkWidget *about_menu_item = gtk_smart_menu_item_get_menuitem(about) ;

  g_signal_connect(about_menu_item, "activate", G_CALLBACK(about_callback), app) ;



  GtkWidget *menu = gtk_menu_new() ;

  gtk_menu_shell_append(GTK_MENU_SHELL(menu), open_file_menu_item) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new ()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), snapshot_menu_item) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new ()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), incr_volume_menu_item) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), decr_volume_menu_item) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), memory_flush_menu_item) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), preferences_menu_item) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), about_menu_item) ;

  gtk_widget_show_all(menu) ;


  GtkWidget *menu_button = gtk_menu_button_new() ;

  GtkWidget *image_menubutton = gtk_image_new_from_file(PATH_TO_MENU_BUTTON) ;

  gtk_button_set_image(GTK_BUTTON(menu_button),image_menubutton);

  gtk_button_set_always_show_image(GTK_BUTTON(menu_button), TRUE) ;

  gtk_menu_button_set_popup(GTK_MENU_BUTTON(menu_button), menu) ;



  gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), menu_button);



  GtkWidget *prev_chapter_button = gtk_button_new() ;

  gtk_button_set_relief(GTK_BUTTON(prev_chapter_button), GTK_RELIEF_NONE) ;

  g_signal_connect(G_OBJECT(prev_chapter_button), "clicked", G_CALLBACK(decr_change_chapter), NULL) ;

  GtkWidget *prev_chapter_image = gtk_image_new_from_file(PATH_TO_PREV_CHAPTER) ;

  gtk_button_set_always_show_image(GTK_BUTTON(prev_chapter_button), TRUE) ;

  gtk_button_set_image(GTK_BUTTON(prev_chapter_button), prev_chapter_image) ;




  GtkWidget *play_pause_button = gtk_button_new() ;

  gtk_button_set_relief(GTK_BUTTON(play_pause_button), GTK_RELIEF_NONE) ;

  g_signal_connect(G_OBJECT(play_pause_button), "clicked", G_CALLBACK(on_click_play_pause), NULL) ;

  GtkWidget *play_pause_image = gtk_image_new_from_file(PATH_TO_ICON_PLAY) ;

  gtk_button_set_always_show_image(GTK_BUTTON(play_pause_button), TRUE) ;

  gtk_button_set_image(GTK_BUTTON(play_pause_button), play_pause_image) ;


  GtkWidget *next_chapter_button = gtk_button_new() ;

  gtk_button_set_relief(GTK_BUTTON(next_chapter_button), GTK_RELIEF_NONE) ;

  g_signal_connect(G_OBJECT(next_chapter_button), "clicked", G_CALLBACK(incr_change_chapter), NULL) ;

  GtkWidget *next_chapter_image = gtk_image_new_from_file(PATH_TO_NEXT_CHAPTER) ;

  gtk_button_set_always_show_image(GTK_BUTTON(next_chapter_button), TRUE) ;

  gtk_button_set_image(GTK_BUTTON(next_chapter_button), next_chapter_image) ;


  timeline_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.0, 1.0, 0.01)  ;

  gtk_scale_set_draw_value(GTK_SCALE(timeline_scale), TRUE) ;

  gtk_range_set_slider_size_fixed(GTK_RANGE(timeline_scale), TRUE) ;

  gtk_scale_set_value_pos(GTK_SCALE(timeline_scale), GTK_POS_RIGHT) ;

  g_object_set(timeline_scale, "lower-stepper-sensitivity", GTK_SENSITIVITY_OFF, NULL) ;
  g_object_set(timeline_scale, "upper-stepper-sensitivity", GTK_SENSITIVITY_OFF, NULL) ;

  g_signal_connect(G_OBJECT(timeline_scale), "change-value", G_CALLBACK(timeline_change_value), NULL) ;
  g_signal_connect(G_OBJECT(timeline_scale), "format-value", G_CALLBACK(timeline_format_value), NULL) ;

  // Launching first application lifetime timeout, as long delayed from the second we can.
  g_timeout_add_full(G_PRIORITY_DEFAULT, 256+128+64, (GSourceFunc) timeout_timeline, timeline_scale, NULL) ;

  GtkWidget *fullscreen_button = gtk_button_new() ;

  gtk_button_set_relief(GTK_BUTTON(fullscreen_button), GTK_RELIEF_NONE) ;

  gtk_button_set_always_show_image(GTK_BUTTON(fullscreen_button), TRUE) ;

  GtkWidget *fullscreen_image = gtk_image_new_from_file(PATH_TO_ICON_UNFULLSCREEN) ;

  gtk_button_set_image(GTK_BUTTON(fullscreen_button), fullscreen_image) ;

  g_signal_connect(G_OBJECT(fullscreen_button), "clicked", G_CALLBACK(set_fullscreen), window) ;


  GtkWidget *hbox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;

  gtk_box_pack_start(GTK_BOX(hbox), prev_chapter_button,  FALSE,  FALSE,  0) ;
  gtk_box_pack_start(GTK_BOX(hbox), play_pause_button,    FALSE,  FALSE,  0) ;
  gtk_box_pack_start(GTK_BOX(hbox), next_chapter_button,  FALSE,  FALSE,  0) ;
  gtk_box_pack_start(GTK_BOX(hbox), timeline_scale ,      TRUE,   TRUE,   0) ;
  gtk_box_pack_start(GTK_BOX(hbox), fullscreen_button,    FALSE,  FALSE,  0) ;


  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(hbox), prev_chapter_button, TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(hbox), play_pause_button, TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(hbox), next_chapter_button, TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(hbox), fullscreen_button, TRUE) ;

  gtk_button_box_set_layout(GTK_BUTTON_BOX(hbox), GTK_BUTTONBOX_EDGE) ;

  gtk_container_add(GTK_CONTAINER(controls_bar), hbox) ;

  GtkWidget *screen = gtk_drawing_area_new() ;

  gtk_widget_add_accelerator(window,
                                 "activate-default",
                                 accel_group,
                                 GDK_KEY_Escape,
                                 0,
                                 GTK_ACCEL_VISIBLE);

  gtk_window_add_accel_group(GTK_WINDOW(window), accel_group)   ;

  g_signal_connect(window, "activate-default", G_CALLBACK(gtk_widget_destroy), window) ;

  g_object_set(G_OBJECT(screen), "events",   GDK_BUTTON_PRESS_MASK,  NULL) ;

  gtk_widget_set_events(window, GDK_BUTTON_PRESS_MASK | GDK_STRUCTURE_MASK) ;



  GtkWidget *scrollwin = gtk_scrolled_window_new(NULL, NULL);

  double ratio = 16.0 / 9.0 ;

  gtk_widget_set_size_request(scrollwin, 400 * ratio, 300) ;

  gtk_container_add(GTK_CONTAINER(scrollwin), screen);

  g_signal_connect(G_OBJECT(screen), "realize", G_CALLBACK(player_widget_on_realize), NULL);

  g_signal_connect(G_OBJECT(screen), "draw", G_CALLBACK(draw_window_bg), NULL) ;

  g_signal_connect(G_OBJECT(window), "button-press-event", G_CALLBACK(on_key_press), controls_bar) ;

  g_signal_connect(G_OBJECT(window), "size-allocate", G_CALLBACK(on_size_allocated), controls_bar) ;

  g_signal_connect(G_OBJECT(window), "configure-event", G_CALLBACK(on_configure_window), controls_bar) ;

  g_signal_connect(G_OBJECT(window), "window-state-event", G_CALLBACK(hide_controls_on_iconify), controls_bar) ;

  g_signal_connect_after(G_OBJECT(window), "delete-event", G_CALLBACK(delete_event), NULL) ;

  g_signal_connect(G_OBJECT(window), "destroy",      G_CALLBACK(destroy),      controls_bar) ;

  gtk_container_add(GTK_CONTAINER(window), scrollwin) ;






  gtk_widget_show_all(window) ;

  if (argc > 1) {

    g_application_activate(G_APPLICATION(app)) ;

  }

  while (g_main_context_pending(NULL)) { // gtk_events_pending()

    g_main_context_iteration(NULL, FALSE) ;

  }

  gtk_widget_hide(controls_bar) ;

  /** Enter GUI mainloop. **/
  int status = g_application_run(G_APPLICATION(app), argc, argv) ;

  g_object_unref(app) ;

  return status ;


}

static void player_widget_on_realize(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GdkWindow *window = gtk_widget_get_window(widget);

  guintptr window_handle = (gulong) NULL ;

  if (! gdk_window_ensure_native(window)) {

    g_error("Couldn't create native window needed !");

  }


  /* Retrieve window handler from GDK */
  #if defined (GDK_WINDOWING_WIN32)
  window_handle = (guintptr)GDK_WINDOW_HWND (window);
  #elif defined (GDK_WINDOWING_QUARTZ)
  window_handle = gdk_quartz_window_get_nsview (window);
  #elif defined (GDK_WINDOWING_X11)
  window_handle = GDK_WINDOW_XID (window);
  #endif

  libvlc_media_player_set_xwindow(media_player, window_handle) ;




}

static inline gboolean draw_window_bg(GtkWidget *widget, cairo_t *cr) {

  cairo_set_source_rgb(cr, 0, 0, 0);

  cairo_paint(cr) ;

  return FALSE ;
}

static void on_size_allocated(GtkWidget *window, GdkRectangle *allocation, GtkWidget *controls) {


  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (window == NULL) {

    return ;

  }

  if (controls != NULL && gtk_widget_is_visible(controls)) {

    compute_controls_size(window, controls) ;

  }


  return ;

}

static void compute_controls_size(GtkWidget *window, GtkWidget *controls) {

  gint root_x, root_y ;

  gtk_window_get_position(GTK_WINDOW(window), &root_x, &root_y);

  gint width, height ;

  gtk_window_get_size(GTK_WINDOW(window), &width, &height);

  gtk_window_resize(GTK_WINDOW(controls), width, 40);

  gtk_widget_set_size_request(timeline_scale , width-128-64-32 , -1) ;

  gtk_window_move(GTK_WINDOW(controls), root_x, height+root_y) ;

  is_controls_on = TRUE ;

  gtk_widget_show_all(controls) ;

  gtk_window_set_keep_above(GTK_WINDOW(controls), TRUE) ;

  return ;

}

static gboolean on_key_press(GtkWidget *window, GdkEventButton  *event, GtkWidget *controls) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (event->button == 1 && ! is_controls_on && event->type == GDK_BUTTON_PRESS) {

    compute_controls_size(window, controls) ;

    is_controls_key_pressed = TRUE ;

  }
  else {

    gtk_widget_hide(controls) ;

    gtk_window_set_keep_above(GTK_WINDOW(controls), FALSE) ;

    is_controls_on = FALSE ;

    is_controls_key_pressed = FALSE ;

  }

  return FALSE ;

}

static gboolean on_configure_window(GtkWidget *window, GdkEvent  *event, GtkWidget *controls) {

  if (gtk_widget_is_visible(controls)) {

    compute_controls_size(window, controls) ;

  }

  return FALSE ;

}


static gchar* timeline_format_value(GtkScale *scale, gdouble value) {

  if (datetime == NULL) {

    return g_strdup_printf("00:00:00/00:00:00") ;

  }

  long int hours = (long int) (factor * value) / 3600.0 ;

  long int min = (long int) (factor * value) / 60.0 - (hours * 60.0) ;

  long int sec = (long int) (value *  factor) % 60 ;

  return g_strdup_printf("%02li:%02li:%02li/%02i:%02i:%02i", hours, min, sec, g_date_time_get_hour(datetime), g_date_time_get_minute(datetime), g_date_time_get_second(datetime) ) ;

}

static gboolean timeline_change_value(GtkRange *range, GtkScrollType scroll, gdouble value) {

  if ( (value <= 0.0) || (value >= 1.0)) {

    return TRUE ;

  }

  libvlc_media_player_set_position(media_player, (float) value) ;

  return TRUE ;

}

static gboolean timeout_timeline(GtkWidget *widget) {

  /** Program lifetime timeout, which have for purpose to:
    *
    *  to update the display time-line value.
    *
    *  @NOTE: see the function @timeout_update_media(...) comments in the same file underhand,
    *         for explanations about the GRecMutex using.
    *
  **/

  // Used for avoiding main-loop timeouts collisions: with a GLib Recursive GMutex locking @Try (Else it can block the interface without the "try").
  gboolean ret = g_rec_mutex_trylock(&mainloop_ctrl_rec_mutex) ;

  if (! ret || widget == NULL) {

    return TRUE ;
  }

  if (libvlc_media_player_is_playing(media_player)) {

    float delay = libvlc_media_player_get_position(media_player) ;

    gtk_range_set_value(GTK_RANGE(widget), (double) delay) ;

  }

  g_rec_mutex_unlock(&mainloop_ctrl_rec_mutex) ;

  return TRUE ;

}





static gboolean hide_controls_on_iconify(GtkWidget *window, GdkEventWindowState *event,GtkWidget *controls) {

  if (event->type == GDK_WINDOW_STATE) {

    if (event->changed_mask == GDK_WINDOW_STATE_ICONIFIED) {

      if (gtk_widget_get_visible(controls)) {

        /** Hide the controls if the window is iconify. **/

        gtk_widget_hide(controls) ;

        is_controls_on = FALSE ;

      }
      else {

        if (! is_controls_on) {

          /** Restore the controls if any was present before. **/

          gtk_widget_show_all(controls) ;

          is_controls_on = TRUE ;

        }


      }

      if (! is_controls_key_pressed) {

        /** Don't show controls. **/

        gtk_widget_hide(controls) ;

      }

    }

  }





  return FALSE ;

}

static void activate(GApplication *application) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  #ifdef G_OS_UNIX
  GDBusConnection *dbus_connection = g_application_get_dbus_connection(G_APPLICATION(application)) ;

  if (dbus_connection != NULL && G_IS_DBUS_CONNECTION(dbus_connection) ) {

    g_dbus_connection_start_message_processing(dbus_connection) ;

  }

  g_application_set_default(G_APPLICATION(application)) ;

  /** 1 seconds before becomes idle. **/
  g_application_set_inactivity_timeout(G_APPLICATION(application), 5000) ;



  #ifdef DEBUG
  fprintf(stdout,"GtkApplication DBUS path: %s\n", g_application_get_dbus_object_path(G_APPLICATION(application)) ) ;
  #endif

  #endif

  #ifdef MAC_INTEGRATION
  gtkosx_application_cancel_attention_request(gtkosx_application_get(), attention) ;
  #endif



  return ;

}









static void open_files(GApplication  *application, GFile **files, gint n_files, const gchar *hint) {


  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif



  /** GtkApplication command line open folder or files argument treatment function:
    * This will permit the g_win32_get_command_line() function to be automatically called internally.
    * And many others things.
    *************************************************************************************************/

  if  (n_files > 0) {

    GtkWindow *window = gtk_application_get_active_window(GTK_APPLICATION(application)) ;

    open_movie(window, g_file_dup(files[0])) ;

  }



  return ;

}

static void startup(GApplication *application, gpointer user_data) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  while (! g_application_get_is_registered(application)) {

    while (g_main_context_pending(NULL)) {

      g_main_context_iteration(NULL, FALSE) ;

    }
  }

  return ;
}

#ifdef G_OS_UNIX
static void about_activated(GSimpleAction *action, GVariant *parameter, gpointer app) {

  display_about_dialog(NULL, app)  ;

  return ;

}

#if 0
static void open_file_activated(GSimpleAction *action, GVariant *parameter, gpointer app) {

  /** FIXME: Cannnot pass controls and window at the same time. **/

  GtkWidget *window = gtk_application_get_active_window(app) ;

  choose_video(window, controls) ;

  return ;

}
#endif

static void set_as_default_movie_player(GtkWidget *widget, GtkApplication *app) {

  GAppInfo *app_info = g_app_info_create_from_commandline(PRGPATH, NULL, G_APP_INFO_CREATE_SUPPORTS_URIS, NULL) ;

  int c = 0 ;

  while (mimetype[c] != NULL) {

    g_app_info_set_as_default_for_type(app_info, mimetype[c], NULL) ;

    ++c ;
  }



  return ;

}

static void reset_to_default(GtkWidget *widget) {

  int c = 0 ;

  while (mimetype[c] != NULL) {

    g_app_info_reset_type_associations(mimetype[c]) ;

    ++c ;
  }

  return ;

}

static void preferences_activated(GSimpleAction *action, GVariant *parameter, gpointer app) {

  GtkWidget *preferences_dialog = gtk_dialog_new() ;

  gtk_window_set_transient_for(GTK_WINDOW(preferences_dialog),  gtk_application_get_active_window(GTK_APPLICATION(app))) ;

  GtkWidget *button_use = gtk_button_new_with_label("Set " PRGNAME " as default movie player !") ;

  g_signal_connect(button_use, "clicked", G_CALLBACK(set_as_default_movie_player), app) ;

  GtkWidget *button_reset = gtk_button_new_with_label("Reset to default") ;

  g_signal_connect(button_reset, "clicked", G_CALLBACK(reset_to_default), app) ;

  GdkPixbuf *pixbuf_icon = gdk_pixbuf_new_from_file_at_size(PATH_TO_ICON, 64+32, 64+32, NULL) ;

  GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf_icon) ;

  g_object_unref(pixbuf_icon) ;

  GtkWidget *toggle_button_enable_registering = gtk_toggle_button_new_with_label("Ask for registering at exit.") ;

  gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(toggle_button_enable_registering), FALSE);

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle_button_enable_registering), register_at_exit);

  GtkWidget *listbox = gtk_list_box_new() ;

  GtkWidget *radiobutton_desktop = gtk_radio_button_new_with_label(NULL,"Desktop") ;
  GtkWidget *radiobutton_document = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_desktop), "Documents") ;
  GtkWidget *radiobutton_download = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_desktop),"Download") ;
  GtkWidget *radiobutton_music = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_desktop),"Music") ;
  GtkWidget *radiobutton_picture = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_desktop),"Picture") ;
  GtkWidget *radiobutton_video = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radiobutton_desktop),"Videos") ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton_desktop), (! g_strcmp0(snapshot_location, g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP))));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton_document), (! g_strcmp0(snapshot_location, g_get_user_special_dir(G_USER_DIRECTORY_DOCUMENTS))));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton_download), (! g_strcmp0(snapshot_location, g_get_user_special_dir(G_USER_DIRECTORY_DOWNLOAD))));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton_music), (! g_strcmp0(snapshot_location, g_get_user_special_dir(G_USER_DIRECTORY_MUSIC))));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton_picture), (! g_strcmp0(snapshot_location, g_get_user_special_dir(G_USER_DIRECTORY_PICTURES))));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton_video), (! g_strcmp0(snapshot_location, g_get_user_special_dir(G_USER_DIRECTORY_VIDEOS))));

  gtk_list_box_insert(GTK_LIST_BOX(listbox), radiobutton_desktop, 0) ;
  gtk_list_box_insert(GTK_LIST_BOX(listbox), radiobutton_document, 1) ;
  gtk_list_box_insert(GTK_LIST_BOX(listbox), radiobutton_download, 2) ;
  gtk_list_box_insert(GTK_LIST_BOX(listbox), radiobutton_music, 3) ;
  gtk_list_box_insert(GTK_LIST_BOX(listbox), radiobutton_picture, 4) ;
  gtk_list_box_insert(GTK_LIST_BOX(listbox), radiobutton_video, 5) ;


  GtkWidget *expander = gtk_expander_new("Where to store the snapshots (folder) ?") ;

  gtk_container_add(GTK_CONTAINER(expander), listbox) ;

  gtk_expander_set_resize_toplevel(GTK_EXPANDER(expander), TRUE);

  GtkWidget *grid = gtk_grid_new() ;

  gtk_grid_attach(GTK_GRID(grid), button_use,   0, 0, 1, 1) ;
  gtk_grid_attach(GTK_GRID(grid), button_reset, 0, 1, 1, 1) ;

  gtk_grid_set_row_homogeneous(GTK_GRID(grid), FALSE);

  gtk_grid_attach(GTK_GRID(grid), image, 1,0,2,2) ;
  gtk_grid_attach(GTK_GRID(grid), toggle_button_enable_registering, 0,2,3,1) ;
  gtk_grid_attach(GTK_GRID(grid), expander, 0,3,3,1) ;

  g_object_set(grid, "column-spacing",  16, NULL) ;
  g_object_set(grid, "row-spacing",     16, NULL) ;

  gtk_container_set_border_width(GTK_CONTAINER(grid), 16) ;

  GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(preferences_dialog));

  gtk_box_pack_start(GTK_BOX(content_area), grid, FALSE, FALSE, 0) ;

  gtk_widget_show_all(preferences_dialog) ;

  gtk_dialog_run(GTK_DIALOG(preferences_dialog)) ;

  register_at_exit = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle_button_enable_registering)) ;

  GSList *gs_list = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radiobutton_desktop));

  gs_list = g_slist_nth(gs_list, 0);

  while (gs_list != NULL) {
 
    if (gtk_toggle_button_get_active(gs_list->data)) {
   
      const gchar *label = gtk_button_get_label(GTK_BUTTON(gs_list->data)) ;
   
      g_free(snapshot_location) ;
    
      snapshot_location = NULL ;
    
      if (! g_strcmp0(label, "Desktop")) {
     
        snapshot_location = g_strdup(g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP)) ;
      }
      else if (! g_strcmp0(label, "Documents")) {
     
        snapshot_location = g_strdup(g_get_user_special_dir(G_USER_DIRECTORY_DOCUMENTS)) ;
      }
      else if (! g_strcmp0(label, "Download")) {
     
        snapshot_location = g_strdup(g_get_user_special_dir(G_USER_DIRECTORY_DOWNLOAD)) ;
      }
      else if (! g_strcmp0(label, "Music")) {
     
        snapshot_location = g_strdup(g_get_user_special_dir(G_USER_DIRECTORY_MUSIC)) ;
      }
      else if (! g_strcmp0(label, "Picture")) {
     
        snapshot_location = g_strdup(g_get_user_special_dir(G_USER_DIRECTORY_PICTURES)) ;
      }
      else if (! g_strcmp0(label, "Videos")) {
     
        snapshot_location = g_strdup(g_get_user_special_dir(G_USER_DIRECTORY_VIDEOS)) ;
      }
  
      break ;
    }
 
    if (gs_list->next != NULL) {
   
      gs_list = gs_list->next ;
    }
    else {
   
      break ;
    } 
  }   

  if (snapshot_location == NULL) {
  
    snapshot_location = g_strdup(g_get_home_dir()) ;
  
  } 

  set_configuration() ;

  gtk_widget_destroy(preferences_dialog) ;

  return ;

}

static void display_about_dialog(GtkWidget *widget, GtkApplication *app) {

  GtkWidget *about_dialog = gtk_about_dialog_new() ;

  gtk_window_set_transient_for(GTK_WINDOW(about_dialog),  gtk_application_get_active_window(GTK_APPLICATION(app))) ;

  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(about_dialog), PRGNAME) ;

  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about_dialog),      VERSION)   ;

  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(about_dialog),    COPYRIGHT)   ;

  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(about_dialog),     "Yet another movie player which\ncan remember the position wher you stop the movie.") ;

  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(about_dialog), PRGWEBURL) ;

  gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(about_dialog), PRGNAME " web page") ;

  gchar *file_content ;

  g_file_get_contents(PATH_TO_GPL_TEXT, &file_content, NULL, NULL) ;

  gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(about_dialog), file_content) ;

  g_free(file_content) ;

  gtk_about_dialog_set_wrap_license(GTK_ABOUT_DIALOG(about_dialog), FALSE);

  const gchar *author[2] = {AUTHOR " " MAIL, NULL} ;

  gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(about_dialog), author);

  gtk_about_dialog_set_documenters(GTK_ABOUT_DIALOG(about_dialog), author);

  gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(about_dialog), NULL) ;

  gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(about_dialog), NULL);

  const gchar *thanks[2] = { "Thanks to my beloved mother, my family and the doctors.", NULL} ;

  gtk_about_dialog_add_credit_section(GTK_ABOUT_DIALOG(about_dialog), "Thank's", thanks);

  const gchar *advice[2] = { "Stay away from drugs: drugs destroy your brain and your life.", NULL} ;

  gtk_about_dialog_add_credit_section(GTK_ABOUT_DIALOG(about_dialog), "Advice:", advice);

  gtk_dialog_run(GTK_DIALOG(about_dialog)) ;

  gtk_widget_destroy(about_dialog) ;

  return ;

}

static void quit_activated(GSimpleAction *action, GVariant *parameter, gpointer app) {

  destroy(GTK_WIDGET(gtk_application_get_active_window(GTK_APPLICATION(app))), app) ;

  return ;

}
#endif

static void shutdown_app(GApplication *application, gpointer user_data) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  delete_event(NULL, NULL) ;

}









static void destroy(GtkWidget *window, GtkWidget *controls) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (register_at_exit) {

    if (media != NULL) {

      ask_for_registering(window, controls) ;

    }

    g_bookmark_file_to_file(bookmark, PATH_TO_BOOKMARK, &error);

    if (error != NULL) {

      display_error_message(window, error->message) ;

      g_clear_error(&error) ;

    }

    g_bookmark_file_free(bookmark) ;

  }

  libvlc_media_release(media) ;

  libvlc_media_player_stop(media_player) ;

  libvlc_media_player_release(media_player) ;

  gtk_widget_destroy(window) ;

  GtkApplication *app = gtk_window_get_application(GTK_WINDOW(window)) ;

  g_application_quit(G_APPLICATION(app)) ;

  return ;

}



static gboolean delete_event(GtkWidget *widget,GdkEvent *event) {

  return FALSE ;
}

static void preferences_callback(GtkWidget *widget, GtkApplication *app) {

  preferences_activated(NULL, NULL, app) ;

  return ;

}

static void about_callback(GtkWidget *widget, GtkApplication *app) {

  display_about_dialog(NULL, app)  ;

  return ;

}

static void flush_bookmark_file(GtkWidget *widget) {

  GtkWidget *window = gtk_widget_get_toplevel(widget) ;

  GtkWidget *msg_widget = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_YES_NO, "%s", "Clearing the position of all video current stored ?") ;

  gint res = gtk_dialog_run(GTK_DIALOG(msg_widget)) ;

  if (res == GTK_RESPONSE_YES) {

    g_remove(PATH_TO_BOOKMARK) ;

    g_creat(PATH_TO_BOOKMARK, 0777) ;

  }

  gtk_widget_destroy(msg_widget) ;

}





