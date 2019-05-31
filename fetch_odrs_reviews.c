#include <libsoup/soup.h>
#include <json-glib/json-glib.h>
#include <string.h>

gchar *gs_utils_get_user_hash (GError **error)
{
	g_autofree gchar *data = NULL;
	g_autofree gchar *salted = NULL;

	if (!g_file_get_contents ("/etc/machine-id", &data, NULL, error))
		return NULL;

	salted = g_strdup_printf ("gnome-software[%s:%s]",
				  g_get_user_name (), data);
	return g_compute_checksum_for_string (G_CHECKSUM_SHA1, salted, -1);
}

int main (int argc, char **argv)
{
    SoupSession *session;
    SoupMessage *message;
    guint status_code;
    JsonParser *parser;
    gboolean result;
    JsonNode *root;
    JsonArray *array;
    gint i;
    gchar *uri = NULL;
    JsonBuilder *builder = NULL;	
    JsonGenerator *json_generator = NULL;
    JsonNode *json_root = NULL;
    gchar *data = NULL;
    GError *error = NULL;

    gchar *user_hash = gs_utils_get_user_hash (&error);
    if (user_hash == NULL) {
	g_warning ("Failed to get machine+user hash: %s", error->message);
    }

    uri = g_strdup_printf ("%s/fetch", "https://odrs.gnome.org/1.0/reviews/api");
    builder = json_builder_new ();
    json_builder_begin_object (builder);
    json_builder_set_member_name (builder, "user_hash");
    json_builder_add_string_value (builder, user_hash);

    json_builder_set_member_name (builder, "app_id");
    json_builder_add_string_value (builder, "org.gnome.gedit.desktop");

    json_builder_set_member_name (builder, "locale");
    json_builder_add_string_value (builder, "en_DE");

    json_builder_set_member_name (builder, "distro");
    json_builder_add_string_value (builder, "Debian GNU/Linux");

    json_builder_set_member_name (builder, "version");
    json_builder_add_string_value (builder, "unknown");

    json_builder_set_member_name (builder, "limit");
    json_builder_add_int_value (builder, 10);

    json_builder_end_object (builder);

    /* export as a string */
    json_root = json_builder_get_root (builder);
    json_generator = json_generator_new ();
    json_generator_set_pretty (json_generator, TRUE);
    json_generator_set_root (json_generator, json_root);
    data = json_generator_to_data (json_generator, NULL);

    if (data == NULL)
    {
	g_print ("data = %s\n", data);
        return 0;
    }

    session = soup_session_new_with_options (SOUP_SESSION_USER_AGENT, "gnome-software/3.22.1", NULL);
    message = soup_message_new (SOUP_METHOD_POST, uri);
    g_assert (message != NULL);

    soup_message_set_request (message, "application/json; charset=utf-8",
  				  SOUP_MEMORY_COPY, data, strlen (data));

    status_code = soup_session_send_message (session, message);
    if (status_code != SOUP_STATUS_OK) {
	g_print("Got invalid data, return code: %d", status_code);

    } 
    g_assert (status_code == SOUP_STATUS_OK);

    g_print("Returned %s", message->response_body->data);

    /* Parse the data in JSON format */
    parser = json_parser_new ();
    result = json_parser_load_from_data (parser, message->response_body->data, -1, NULL);
    g_assert (result);

    /* The data should contain an array of JSON objects */
    root = json_parser_get_root (parser);
    g_assert (JSON_NODE_HOLDS_ARRAY (root));
    array = json_node_get_array (root);
 
    										    for (i = 0; i < json_array_get_length (array); i++)
										    {
           JsonNode *node;
           JsonObject *object;
           const gchar *description;
	   const gchar *summary;
	   const gchar *app_id;
	   const gchar *date_created;
	   const gchar *distro;
	   const gdouble karma_down;
	   const gdouble karma_up;
	   const gint rating;
	   const gint score;
	   const gint review_id;
	   const gchar *user_display;
	   const gchar *version;
	   
           node = json_array_get_element (array, i);
           if (!JSON_NODE_HOLDS_OBJECT (node))
               continue;

           object = json_node_get_object (node);
	   if (json_object_has_member (object, "date_created")) {
		gint64 timestamp;
		gchar *str;
		g_autoptr(GDateTime) dt = NULL;
		timestamp = json_object_get_int_member (object, "date_created");
		dt = g_date_time_new_from_unix_utc (timestamp);
		str = g_date_time_format (dt, "%F");
		g_print("Date Created:%s \n",str);
		g_free(str);
	   }

           description = json_object_get_string_member (object, "description");
	   /*Emel: Parse the fields similarly */

           }

										    /* Clean up */
    g_object_unref (session); 
    g_object_unref (message);
    g_object_unref (parser);

    g_free(uri);
    g_free(user_hash);

    return 0;
}

