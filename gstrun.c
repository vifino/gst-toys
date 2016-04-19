#include <gst/gst.h>

int main (int argc, char *argv[]) {
	GstElement *pipeline;
	GstMessage *msg;
	GstBus *bus;
	GError *error = NULL;

	gst_init (&argc, &argv);

	if (argc != 2) {
		g_print("usage: %s 'pipeline as one arg'\n", argv[0]);
		return -1;
	}

	pipeline = gst_parse_launch(argv[1], &error);
	if (!pipeline) {
		g_print("Parse error: %s\n", error->message);
		return 1;
	}

	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	bus = gst_element_get_bus(pipeline);

	msg = gst_bus_poll(bus, GST_MESSAGE_EOS | GST_MESSAGE_ERROR, -1);

	int msg_type = GST_MESSAGE_TYPE(msg);

	switch (msg_type) {
		case GST_MESSAGE_EOS: {
			break;
		}
		case GST_MESSAGE_ERROR: {
			GError *err = NULL; /* error to show to users */
			gchar *dbg = NULL;  /* additional debug string for developers */

			gst_message_parse_error(msg, &err, &dbg);
			if (err) {
				g_printerr("ERROR: %s\n", err->message);
				g_error_free(err);
			}
			if (dbg) {
				g_printerr("[Debug details: %s]\n", dbg);
				g_free(dbg);
			}
		}
		default:
			g_printerr("Unexpected message of type %d", msg_type);
			break;
	}
	gst_message_unref(msg);

	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);
	gst_object_unref(bus);

	return 0;
}
