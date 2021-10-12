/* Author: Fuat Bölük <mektup@fuatboluk.com.tr> */


/* Open */
static void open(GtkButton *new_button, GtkWidget *terminal)
{
    system(" terminal & disown ");
}


/* Copy */
static void copy(GtkButton *copy_button, GtkWidget *terminal)
{
    vte_terminal_copy_clipboard_format(VTE_TERMINAL(terminal), VTE_FORMAT_TEXT);
}


/* Paste */
static void paste(GtkButton *paste_button, GtkWidget *terminal)
{
    vte_terminal_paste_clipboard(VTE_TERMINAL(terminal));
}


/* End */
