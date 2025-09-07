# ipc

UNIX inter-process communication examples

# D-Bus Calculator Demo

## 1. Prerequisites

This project demonstrates inter-process communication (IPC) using D-Bus on macOS. It consists of a `calculator_service` that runs as a daemon and a `calculator_client` that makes a remote method call to the service.

Before building the project, ensure you have the necessary dependencies installed using Homebrew.

- **CMake:** For building the project.

- **D-Bus:** The D-Bus library and tools.

You can install them with the following command:

```bash
brew install cmake dbus
```

## 2. Building the Project

Navigate to the project root directory and follow these steps to build the executables.

1. Create a `build` directory:

```bash
mkdir build
cd build
```

2. Run CMake to configure the build:

```bash
cmake ..
```

3. Build the executables:

```bash
make
```

After a successful build, you will have two executable files in the `build` directory: `calculator_service` and `calculator_client`.

## 3. How to Run the Demo

Running D-Bus applications on macOS requires a running D-Bus daemon. Due to how `launchd` and Homebrew interact on Apple Silicon (M-series) Macs, a manual setup is the most reliable way to get the demo working.

You will need to use three separate terminal windows for this demo.

### Terminal 1: Start the D-Bus Daemon

This terminal will run the D-Bus daemon itself. It will remain active as long as the service and client are running.

Set a temporary socket path for the daemon to use

```bash
mkdir /tmp/dbus_socket
export DBUS_SESSION_BUS_ADDRESS=unix:path=/tmp/dbus_socket/bus
```

Start the D-Bus daemon. This terminal will be occupied.

```bash
/opt/homebrew/bin/dbus-daemon --session --address=$DBUS_SESSION_BUS_ADDRESS
```

### Terminal 2: Start the Calculator Service

This terminal will run the D-Bus service. It must be able to find the daemon you started in Terminal 1.

Set the same session bus address as the daemon is using

```bash
export DBUS_SESSION_BUS_ADDRESS=unix:path=/tmp/dbus_socket/bus
```

### Terminal 3: Run the Calculator Client

This terminal will run the client, which prompts for user input and sends a message to the service.

- Set the same session bus address as the daemon is using:

```bash
export DBUS_SESSION_BUS_ADDRESS=unix:path=/tmp/dbus_socket/bus
```

- run the client executable:

```bash
./calculator_client
```

## 4. What to Expect

- **Terminal 1:** The `dbus-daemon` will start and remain quiet.

- **Terminal 2:** The service will print `"Service is running..."` and will then print the arguments it receives when the client sends a message.

- **Terminal 3:** The client will prompt you to enter two integers. After you provide them, it will send the message and print the structured reply it receives from the service.

Once the client successfully receives the reply, the process will exit cleanly.
