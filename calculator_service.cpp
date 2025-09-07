#include <iostream>
#include <dbus/dbus.h>
#include <unistd.h>

const char* const SERVICE_NAME = "com.example.Calculator";
const char* const OBJECT_PATH = "/com/example/Calculator";
const char* const INTERFACE_NAME = "com.example.Calculator";

// a handler for message calls
DBusHandlerResult message_handler(DBusConnection* connection, DBusMessage* message, void* user_data) {
    if (dbus_message_is_method_call(message, INTERFACE_NAME, "Add")) {
        DBusMessage* reply = NULL;
        int32_t a = 0,b = 0;
        DBusError error;
        dbus_error_init(&error);
        
        // get args from the method call
        dbus_message_get_args(message, &error, DBUS_TYPE_INT32, &a, DBUS_TYPE_INT32, &b, DBUS_TYPE_INVALID);
        if (dbus_error_is_set(&error)) {
            std::cerr << "Error getting message args: " << error.message << std::endl;
            dbus_error_free(&error);
            return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
        }

        int32_t sum = a + b;
        std::cout << "Service received method call: Add(" << a << ", " << b << ')' << std::endl;

        // create a method return message
        reply = dbus_message_new_method_return(message);
        if (reply == NULL) {
            std::cerr << "Error creating method return message" << std::endl;
            return DBUS_HANDLER_RESULT_NEED_MEMORY;
        }

        DBusMessageIter iter;
        dbus_message_iter_init_append(reply, &iter);
        
        // We need a separate iterator for the struct's contents
        DBusMessageIter struct_iter;

        // The contained_signature argument MUST be a NULL string for a struct
        // The signature is implicitly built from the appended types.
        dbus_message_iter_open_container(&iter, DBUS_TYPE_STRUCT, NULL, &struct_iter);
        
        // Append the basic types using the struct_iter
        dbus_message_iter_append_basic(&struct_iter, DBUS_TYPE_INT32, &a);
        dbus_message_iter_append_basic(&struct_iter, DBUS_TYPE_INT32, &b);
        dbus_message_iter_append_basic(&struct_iter, DBUS_TYPE_INT32, &sum);

        // Close the container using both the parent and child iterators
        dbus_message_iter_close_container(&iter, &struct_iter);

        dbus_connection_send(connection, reply, NULL);
        dbus_message_unref(reply);
        return DBUS_HANDLER_RESULT_HANDLED;
    };
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

int main() {
    DBusError error;
    dbus_error_init(&error);

    // connect to the session bus
    DBusConnection* connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
    if (dbus_error_is_set(&error)) {
        std::cerr << "Connection Error (" << error.message << ")" << std::endl;
        dbus_error_free(&error);
        return 1;
    }


    // request a unique bus name
    dbus_bus_request_name(connection, SERVICE_NAME, DBUS_NAME_FLAG_REPLACE_EXISTING, &error);
    if (dbus_error_is_set(&error)) {
        std::cerr << "Name Request Error (" << error.message << ")" << std::endl;
        dbus_error_free(&error);
        return 1;
    }

    // add a message handler
    dbus_connection_add_filter(connection, message_handler, NULL, NULL);

    std::cout << "Service is running..." << std::endl;
    
    // enter the event loop
    while (dbus_connection_read_write_dispatch(connection, -1)) {
        // loop forever
    }

    // clean up
    dbus_connection_close(connection);
    return 0;
}

