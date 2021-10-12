/* Author: Fuat Bölük <mektup@fuatboluk.com.tr> */


#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <vte/vte.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

#include <gdk/gdkkeysyms.h>
#include <glib/gi18n.h>
#include <gio/gio.h>
#include <glib.h>

#include "helper.c"
#include "shortcuts.c"
#include "dragdrop.c"
#include "functions.c"


#define WIDTH  960
#define HEIGHT 472


/* CallBack */
static void on_temrinal_command_spawned(VteTerminal *terminal G_GNUC_UNUSED, GPid pid G_GNUC_UNUSED,
                                        GError *error, gpointer user_data)
{
    if (error) {
        g_critical("%s", error->message);
        g_error_free(error);
        exit(EXIT_FAILURE);
    }

    gtk_widget_show_all(user_data);
}


/* Change Title */
static void on_title_changed(GtkWidget *terminal, gpointer user_data)
{
    GtkWindow *window = user_data;
    gtk_window_set_title(GTK_WINDOW(window), vte_terminal_get_window_title(VTE_TERMINAL(terminal)));
}


/* Main Function */
int main(int argc, char *argv[])
{
    GtkWidget *window, *terminal, *scrolled;
    GtkWidget *header, *new_button, *copy_button, *paste_button;
    GdkRGBA back_rgba = { 0, 0, 0, 0.9 };
    GdkRGBA front_rgba = { 1, 1, 1, 1.0 };
    PangoFontDescription *font = NULL;
    GdkVisual *visual;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_icon_name(GTK_WINDOW(window), "utilities-terminal");

    header = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
    gtk_header_bar_set_title(GTK_HEADER_BAR(header), "Terminal");
    gtk_header_bar_set_has_subtitle(GTK_HEADER_BAR(header), FALSE);

    new_button = gtk_button_new_from_icon_name("utilities-terminal-symbolic", GTK_ICON_SIZE_MENU);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header), GTK_WIDGET(new_button));

    copy_button = gtk_button_new_from_icon_name("edit-copy-symbolic", GTK_ICON_SIZE_MENU);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header), GTK_WIDGET(copy_button));

    paste_button = gtk_button_new_from_icon_name("edit-paste-symbolic", GTK_ICON_SIZE_MENU);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header), GTK_WIDGET(paste_button));

    gtk_window_set_titlebar(GTK_WINDOW(window), GTK_WIDGET(header));
    gtk_widget_set_app_paintable(GTK_WIDGET(window), TRUE);

    visual = gdk_screen_get_rgba_visual(gtk_widget_get_screen(window));
    gtk_widget_set_visual(GTK_WIDGET(window), visual);

    scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_overlay_scrolling(GTK_SCROLLED_WINDOW(scrolled), 5);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrolled), WIDTH);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled), HEIGHT);

    terminal = vte_terminal_new();
    vte_terminal_set_color_background(VTE_TERMINAL(terminal), &back_rgba);
    vte_terminal_set_color_foreground(VTE_TERMINAL(terminal), &front_rgba);

    font = pango_font_description_from_string("Monospace Regular 11");
    vte_terminal_set_font(VTE_TERMINAL(terminal), font);

    VtePtyFlags pty_flags = VTE_PTY_DEFAULT;

    gchar **envp = g_get_environ();
    gchar **command = (gchar *[]){g_strdup(g_environ_getenv(envp, "SHELL")), NULL };
    g_strfreev(envp);

    vte_terminal_spawn_async(VTE_TERMINAL(terminal),
                             pty_flags,
                             g_get_home_dir(),             /* working directory */
                             command,                      /* command           */
                             NULL,                         /* environment       */
                             G_SPAWN_SEARCH_PATH,          /* spawn flags       */
                             NULL,                         /* child setup       */
                             NULL,                         /* data              */
                             NULL,                         /* data destroy      */
                             -1,                           /* timeout           */
                             NULL,                         /* cancelable        */
                             on_temrinal_command_spawned,  /* callback          */
                             window);                      /* user data         */

    vte_terminal_set_scrollback_lines(VTE_TERMINAL(terminal), -1);
    vte_terminal_set_scroll_on_output(VTE_TERMINAL(terminal), TRUE);
    vte_terminal_set_scroll_on_keystroke(VTE_TERMINAL(terminal), TRUE);
    vte_terminal_set_rewrap_on_resize(VTE_TERMINAL(terminal), TRUE);
    vte_terminal_set_mouse_autohide(VTE_TERMINAL(terminal), TRUE);

    g_signal_connect(window, "delete-event", gtk_main_quit, NULL);

    gtk_drag_dest_set(terminal, GTK_DEST_DEFAULT_ALL, target_list, 2, GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK);

    g_signal_connect(terminal, "drag_drop", G_CALLBACK(on_drag_drop), GTK_WINDOW(window));
    g_signal_connect(terminal, "drag_data_received", G_CALLBACK(on_drag_data_received), GTK_WINDOW(window));

    g_signal_connect(terminal, "child-exited", gtk_main_quit, NULL);
    g_signal_connect(terminal, "key_press_event", G_CALLBACK(on_key_press), GTK_WINDOW(window));
    g_signal_connect(terminal, "window-title-changed", G_CALLBACK(on_title_changed), GTK_WINDOW(window));

    g_signal_connect(new_button, "clicked", G_CALLBACK(open), terminal);
    g_signal_connect(copy_button, "clicked", G_CALLBACK(copy), terminal);
    g_signal_connect(paste_button, "clicked", G_CALLBACK(paste), terminal);

    gtk_container_add(GTK_CONTAINER(scrolled), terminal);
    gtk_widget_show_all(terminal);

    gtk_container_add(GTK_CONTAINER(window), scrolled);
    gtk_widget_show_all(scrolled);

    gtk_widget_set_size_request(window, WIDTH, HEIGHT);
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}


/* End */
