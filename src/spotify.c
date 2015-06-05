/*
  Spotify Now Playing plugin
  Original X-Chat source by S3NSA: http://forum.cheatengine.org/viewtopic.php?t=405073&sid=83516683cf425324e4596a993fcec09b
  Rewritten for HexChat by Freek
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "hexchat-plugin.h"


static hexchat_plugin *ph;   /* plugin handle */
static char name[] = "Spotify Now Playing";
static char desc[] = "Sends currently playing song in Spotify to the current channel.";
static char version[] = "1.0";
static const char helpmsg[] = "Sends currently playing song in Spotify to the current channel. USAGE: /spotify";

static int spotify_cb(char *word[], char *word_eol[], void *userdata)
{
	HWND hWnd = FindWindowW (L"SpotifyMainWindow", NULL);
	wchar_t window_text[1024];

	if (hWnd == NULL)
	{
		hexchat_print (ph, "Unable to find Spotify window.");
		return HEXCHAT_EAT_ALL;
	}

	if (GetWindowTextW (hWnd, window_text, 1024))
	{
		char utf8_title[2048], *title = utf8_title;

		if (wcscmp (window_text, L"Spotify") == 0)
		{
			hexchat_print (ph, "Spotify is not playing anything right now.");
			return HEXCHAT_EAT_ALL;
		}

		/* UTF-16 to UTF-8 */
		if (!WideCharToMultiByte (CP_UTF8, 0, window_text, -1, &utf8_title, sizeof(utf8_title), NULL, NULL))
		{
			hexchat_print (ph, "Failed to convert song title to utf8");
			return HEXCHAT_EAT_ALL;
		}

		/* Older versions have a prefix */
		if (strncmp (title, "Spotify - ", 10) == 0)
			title += 10;

		hexchat_commandf (ph, "me is now listening to: %s", title);
	}
	return HEXCHAT_EAT_ALL;
}

int hexchat_plugin_init(hexchat_plugin *plugin_handle,
                      char **plugin_name,
                      char **plugin_desc,
                      char **plugin_version,
                      char *arg)
{
	/* we need to save this for use with any hexchat_* functions */
	ph = plugin_handle;

	/* tell HexChat our info */
	*plugin_name = name;
	*plugin_desc = desc;
	*plugin_version = version;

	hexchat_hook_command (ph, "SPOTIFY", HEXCHAT_PRI_NORM, spotify_cb, helpmsg, 0);

	hexchat_printf (ph, "%s plugin loaded\n", name);

	return 1;	/* return 1 for success */
}

int hexchat_plugin_deinit(hexchat_plugin *plugin_handle)
{
	hexchat_printf (plugin_handle, "%s plugin unloaded\n", name);
	return 1;
}
