# Mosquitto JS auth

Mosquitto JS auth is a lightweight and straightforward plugin for the Mosquitto MQTT broker, designed to pass user credentials (username and password) to a custom script for authentication. 

## Features

- Simple integration
- Minimal and lightweight
- ACL check support

## Installation

1. **Clone builds folder**
	In the **builds** folder, you will find all builds for every system. 

2. **Configure plugin**
    Add the following to your `mosquitto.conf`, and make sure to replace `/path/to/` to a place where the plugin is located, and `arm64.so` with the right build for your system:
    ```ini
    auth_plugin /path/to/arm64.so
    ```

4. **Write your authentication script**
    In the same folder as the plugin, create a `auth.js`, and configure your auth script in here.

    ```javascript
    // auth.js

    const username = process.argv[2];
    const password = process.argv[3];

    if (username === 'admin' && password === 'secret') {
        process.exit(0); // Authentication successful
    } else {
        process.exit(1); // Authentication failed
    }
    ```



### Building the plugin

	Compile the plugin using the following command:
	```bash
	gcc -fPIC -shared -o mosquitto-js-auth.so mosquitto-js-auth.c -I/usr/include/mosquitto
	```
	- **`-fPIC`**: Generates position-independent code, which is required for shared libraries.
	- **`-shared`**: Creates a shared object (`.so`) file.
	- **`-I/usr/include/mosquitto`**: Points to the directory where Mosquitto header files are located.

	Or, if that doesn't work, use:
	```bash
	gcc -fPIC -shared -o mosquitto-js-auth.so mosquitto-js-auth.c -I/opt/homebrew/include -L/opt/homebrew/lib -lmosquitto
	```
	- **`-fPIC`**: Generates position-independent code, which is required for shared libraries.
	- **`-shared`**: Creates a shared object (`.so`) file.
	- **`-I/opt/homebrew/include`**: Specifies the include directory for Mosquitto headers.
	- **`-L/opt/homebrew/lib`**: Specifies the library directory where Mosquitto’s libraries are located.
	- **`-lmosquitto`**: Links against the Mosquitto library to resolve necessary symbols.

	If you are using an ARM-based Mac (e.g., M1 or M2 chip):
	```bash
	gcc -arch arm64 -fPIC -shared -o builds/arm64.so mosquitto-js-auth.c -I/opt/homebrew/include -L/opt/homebrew/lib -lmosquitto -Wl,-undefined,dynamic_lookup
	```
	- **`-arch arm64`**: Compiles the plugin for ARM64 architecture, suitable for Apple Silicon.
	- **`-fPIC`**: Generates position-independent code, which is required for shared libraries.
	- **`-shared`**: Creates a shared object (`.so`) file.
	- **`-I/opt/homebrew/include`**: Specifies the include directory for Mosquitto headers.
	- **`-L/opt/homebrew/lib`**: Specifies the library directory where Mosquitto’s libraries are located.
	- **`-lmosquitto`**: Links against the Mosquitto library to resolve necessary symbols.
	- **`-Wl,-undefined,dynamic_lookup`**: Instructs the linker to resolve symbols dynamically, which is useful when the symbols are resolved at runtime.