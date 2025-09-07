#include <iostream>
#include <dbus/dbus.h>
#include <string>
#include <limits>
#include <unistd.h>

const char* const SERVICE_NAME = "com.example.Calculator";
const char* const OBJECT_PATH = "/com/example/Calculator";
const char* const INTERFACE_NAME = "com.example.Calculator";
const char* const METHOD_NAME = "Add";

int main() {
    DBusError err;
    dbus_error_init(&err);

    // Connect to the session bus
    DBusConnection* conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        std::cerr << "Connection Error: " << err.message << std::endl;
        dbus_error_free(&err);
        return 1;
    }

    // Get user input for the two integers
    int32_t a, b;

    std::cout << "Enter the first integer: ";
    while (!(std::cin >> a)) {
        std::cout << "Invalid input. Please enter a valid integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Enter the second integer: ";
    while (!(std::cin >> b)) {
        std::cout << "Invalid input. Please enter a valid integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    // Create a new method call message
    DBusMessage* msg = dbus_message_new_method_call(
        SERVICE_NAME,
        OBJECT_PATH,
        INTERFACE_NAME,
        METHOD_NAME
    );
    if (msg == NULL) {
        std::cerr << "Message Null" << std::endl;
        dbus_connection_unref(conn);
        return 1;
    }

    // Append the arguments for the method call
    dbus_message_append_args(msg, DBUS_TYPE_INT32, &a, DBUS_TYPE_INT32, &b, DBUS_TYPE_INVALID);

    // Send the message and get a reply
    std::cout << "Client sending method call: Add(" << a << ", " << b << ")" << std::endl;
    DBusMessage* reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);
    if (dbus_error_is_set(&err)) {
        std::cerr << "Reply Error: " << err.message << std::endl;
        dbus_error_free(&err);
        dbus_message_unref(msg);
        dbus_connection_unref(conn);
        return 1;
    }

    // Get the reply arguments
    DBusMessageIter iter;
    dbus_message_iter_init(reply, &iter);

    // The reply should be a single struct
    if (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_STRUCT) {
        std::cerr << "Received unexpected reply type" << std::endl;
        dbus_message_unref(msg);
        dbus_message_unref(reply);
        dbus_connection_unref(conn);
        return 1;
    }

    // Now get the contents of the struct
    DBusMessageIter struct_iter;
    dbus_message_iter_recurse(&iter, &struct_iter);
    
    int32_t arg1, arg2, result;
    dbus_message_iter_get_basic(&struct_iter, &arg1);
    dbus_message_iter_next(&struct_iter);
    dbus_message_iter_get_basic(&struct_iter, &arg2);
    dbus_message_iter_next(&struct_iter);
    dbus_message_iter_get_basic(&struct_iter, &result);
    
    std::cout << "Client received structured reply: " << arg1 << " + " << arg2 << " = " << result << std::endl;
    
    // Clean up
    dbus_message_unref(msg);
    dbus_message_unref(reply);
    dbus_connection_unref(conn);
    return 0;
}

