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
