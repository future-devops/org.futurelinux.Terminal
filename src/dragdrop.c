/* Author: Fuat Bölük <mektup@fuatboluk.com.tr> */


/* Drag Defines */
enum { TARGET_STRING };
static GtkTargetEntry target_list[] = {
     { "STRING",        0, TARGET_STRING },
     { "text/plain",    0, TARGET_STRING }
};


/* Drag Drop */
static gboolean on_drag_drop(GtkWidget *terminal, GdkDragContext *context, gint x, gint y, guint time, gpointer user_data)
{
    gboolean valid = TRUE;
    static GdkAtom target = GDK_NONE;
    const gchar *name = gtk_widget_get_name(terminal);

    if (gdk_drag_context_list_targets(context)) {
        target = GDK_POINTER_TO_ATOM(gdk_drag_context_list_targets(context));
        gtk_drag_get_data(terminal, context, target, time);
    } else {
        valid = FALSE;
    }

    return valid;
}


/* Drag Data Received */
static gboolean on_drag_data_received(GtkWidget *terminal, GdkDragContext *context, int x, int y,
                                      GtkSelectionData *selection_data, guint info, guint time)
{
    char *data;
    char *uris;
    char url[1000];
    gboolean success = FALSE;
    gboolean success_data = FALSE;

    const gchar *name = gtk_widget_get_name(terminal);

    if((selection_data != NULL) && (gtk_selection_data_get_length(selection_data) >= 0)) {

        if (gdk_drag_context_get_suggested_action(context) == GDK_ACTION_COPY) {
            success_data = TRUE;
        }

        if (gdk_drag_context_get_suggested_action(context) == GDK_ACTION_MOVE) {
            success_data = TRUE;
        }

        if (gdk_drag_context_get_suggested_action(context) == GDK_ACTION_LINK) {
            success_data = TRUE;
        }

        switch (info)
        {
            case TARGET_STRING:
                data = (char *) gtk_selection_data_get_text(selection_data);
                uris = strtok(data, "\n");

                while (uris != NULL)
                {
                    if (!strncmp(uris, "file://", 7)) {
                        sprintf(url, " %s", uris + 7);
                    } else if (!strncmp(uris, "x-nautilus-desktop://", 21)) {
                        if (!strncmp(uris, "x-nautilus-desktop:///home", 26)) {
                           sprintf(url, " %s ", getenv("HOME"));
                        } else if (!strncmp(uris, "x-nautilus-desktop:///trash", 27)) {
                           sprintf(url, " %s/.local/share/Trash/files ", getenv("HOME"));
                        } else {
                           sprintf(url, " %s", uris);
                        }
                    }

                    utf8_decode(url, url);
                    vte_terminal_feed_child(VTE_TERMINAL(terminal), url, strlen(url) - 1);
                    uris = strtok(NULL, "\n");
                }
                
                success = TRUE;
                break;

            default:
                return FALSE;
        }
    }
    gtk_drag_finish(context, success, success_data, time);
    g_free(uris);
}


/* End */
