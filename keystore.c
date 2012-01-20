/*
 * Copyright (c) 2012 Daniel Kirchner
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#include <gtk/gtk.h>
#include <libconfig.h>
#include <stdio.h>
#include <string.h>

#define CONFIGFILE "keystore.conf"

void activate (GtkFileChooser *filechooser, gpointer user_data)
{
	config_t config;
	gchar *filename;
	const char *password;
	config_init (&config);

	filename = gtk_file_chooser_get_filename (filechooser);
	if (config_read_file (&config, filename) != CONFIG_TRUE)
	{
		fprintf (stderr, "Can't open file \"%s\".\n", filename);
		g_free (filename);
		config_destroy (&config);
		gtk_main_quit ();
		return;
	}
	g_free (filename);

	if (config_lookup_string (&config, "password", &password) != CONFIG_TRUE)
	{
		fprintf (stderr, "No password defined.\n");
		config_destroy (&config);
		gtk_main_quit ();
		return;
	}

	printf ("%s", password);

	gtk_dialog_response (GTK_DIALOG (filechooser), 0);

	return;
}

int main (int argc, char *argv[])
{
	GtkWidget *filechooser;
	config_t config;
	const char *path;
	gchar *configfile;

	gtk_init (&argc, &argv);

	config_init (&config);

	configfile = g_build_filename (g_get_user_config_dir (), CONFIGFILE, NULL);
	if (config_read_file (&config, configfile) != CONFIG_TRUE)
	{
		fprintf (stderr, "Cannot read config file \"%s\".\n", configfile);
		g_free (configfile);
		config_destroy (&config);
		return -1;
	}
	g_free (configfile);
	if (config_lookup_string (&config, "path", &path) != CONFIG_TRUE)
	{
		fprintf (stderr, "Cannot parse the configuration file.\n");
		config_destroy (&config);
		return -1;
	}

	filechooser = gtk_file_chooser_dialog_new ("Load password", NULL,
																						 GTK_FILE_CHOOSER_ACTION_OPEN,
																						 GTK_STOCK_OPEN, GTK_STOCK_CANCEL,
																						 NULL);
	gtk_file_chooser_set_local_only (GTK_FILE_CHOOSER (filechooser), TRUE);
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (filechooser), path);
	g_signal_connect (filechooser, "file-activated",
										G_CALLBACK (activate), NULL);

	config_destroy (&config);

	gtk_dialog_run (GTK_DIALOG (filechooser));

	return 0;
}
