#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exchange-rates.h"

#ifndef ASSETS_DIR
#define ASSETS_DIR "../assets/"
#endif

#define GLADE_FILE ASSETS_DIR "main.glade"
#define ICON_FILE ASSETS_DIR "icon.png"
#define STYLE_FILE ASSETS_DIR "style.css"


static void exchange_currency(gpointer builderPtr)
{
  GtkBuilder* builder = GTK_BUILDER(builderPtr);
  GObject* spin = gtk_builder_get_object(builder, "spinValue");
  GObject* resultLabel = gtk_builder_get_object(builder, "resultValue");

  GObject* valueCombo = gtk_builder_get_object(builder, "valueCurrency");  
  GObject* resultCombo = gtk_builder_get_object(builder, "resultCurrency");  
  char* from = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(valueCombo));
  char* to = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(resultCombo));

  double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin));
  double result = convert_currency(from, to, value);
  int length = snprintf(NULL, 0, "%.2lf", result) + 1;

  char output[length];
  snprintf(output, length, "%.2lf", result);
  gtk_label_set_text(GTK_LABEL(resultLabel), output);
}

static void on_currency_changed(GtkComboBoxText* combo, gpointer data) { exchange_currency(data); }
static void on_spin_changed(GtkSpinButton* spin, gpointer data) { exchange_currency(data); }
static void update_rates_button_clicked(GtkButton* button, gpointer data) { update_currency_rates(); }
static void on_window_destroyed(GtkWindow* window, gpointer data) { cleanUp();}

static void copy_result_button_clicked(GtkButton* button, gpointer data) 
{
  GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
  GObject* resultLabel = gtk_builder_get_object(data, "resultValue");
  const gchar* text = gtk_label_get_text(GTK_LABEL(resultLabel));
  gtk_clipboard_set_text(clipboard, text, -1);
}

static void activate (GtkApplication* app, gpointer data)
{
  update_currency_rates();
  GtkBuilder* builder = gtk_builder_new_from_file(GLADE_FILE);

  // window and style
  GObject* windowOb = gtk_builder_get_object(builder,"main");
  GtkCssProvider* cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssProvider, STYLE_FILE, NULL);
  gtk_application_add_window(app, GTK_WINDOW(windowOb));
  GtkWidget* windowWidget = GTK_WIDGET(windowOb);
  
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  // callbacks
  GObject* spinOb = gtk_builder_get_object(builder, "spinValue");
  GObject* valueCombo = gtk_builder_get_object(builder, "valueCurrency");  
  GObject* resultCombo = gtk_builder_get_object(builder, "resultCurrency");
  append_currencies_in_combos(GTK_COMBO_BOX_TEXT(valueCombo), GTK_COMBO_BOX_TEXT(resultCombo));

      
  GObject* updateButton = gtk_builder_get_object(builder, "updateButton");
  GObject* copyResultButton = gtk_builder_get_object(builder, "copyResult");

  g_signal_connect(valueCombo, "changed", G_CALLBACK(on_currency_changed), builder);
  g_signal_connect(resultCombo, "changed", G_CALLBACK(on_currency_changed), builder);
  g_signal_connect(spinOb, "value-changed", G_CALLBACK(on_spin_changed), builder);
  g_signal_connect(updateButton, "clicked", G_CALLBACK(update_rates_button_clicked), NULL);
  g_signal_connect(copyResultButton, "clicked", G_CALLBACK(copy_result_button_clicked), builder);
  g_signal_connect(windowOb, "destroy", G_CALLBACK(on_window_destroyed), NULL);

  gtk_widget_show_all(GTK_WIDGET(windowOb));
  
  exchange_currency(builder);
}

int main(int argc, char **argv)
{
  g_print(ASSETS_DIR);
  GtkApplication* app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  int status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  return status;
}