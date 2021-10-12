/* Author: Fuat Bölük <mektup@fuatboluk.com.tr> */


/* Key Press */
static gboolean on_key_press(GtkWidget *terminal, GdkEventKey *event, gpointer user_data)
{
    switch (event->keyval)
    {
      case GDK_KEY_c:
      case GDK_KEY_C:
        if (event->state & GDK_CONTROL_MASK){
            if (event->state & GDK_SHIFT_MASK){
                vte_terminal_copy_clipboard_format(VTE_TERMINAL(terminal), VTE_FORMAT_TEXT);
                return TRUE;
            }
            return FALSE;
        }
        break;
      case GDK_KEY_v:
      case GDK_KEY_V:
        if (event->state & GDK_CONTROL_MASK){
            if (event->state & GDK_SHIFT_MASK){
                vte_terminal_paste_clipboard(VTE_TERMINAL(terminal));
                return TRUE;
            } 
            return FALSE;
        }
        break;
      case GDK_KEY_t:
      case GDK_KEY_T:
        if (event->state & GDK_CONTROL_MASK){
            if (event->state & GDK_SHIFT_MASK){
                system(" terminal & disown ");
                return TRUE;
            } 
            return FALSE;
        }
        break;
      case GDK_KEY_q:
      case GDK_KEY_Q:
        if (event->state & GDK_CONTROL_MASK){
            if (event->state & GDK_SHIFT_MASK){
                gtk_main_quit();
                return TRUE;
            }
            return FALSE;
        }
        break;
      default:
        return FALSE; 
    }
    return FALSE; 
}


/* End */
