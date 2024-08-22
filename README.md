# Mosquitto JS auth

Mosquitto JS auth is a lightweight and straightforward plugin for the Mosquitto MQTT broker, designed to pass user credentials (username and password) to a custom script for authentication. 

## Features

- Simple integration
- Minimal and lightweight
- ACL check support

## Getting Started

### Prerequisites

- **Mosquitto**
- **GCC**
- **Javascript**

### Installation

1. **Clone the repository**

    ```bash
    git clone https://github.com/quintenadema/mosquitt-js-auth.git
    cd mosquitt-js-auth
    ```


2. **Compile the plugin**
   
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
   - **`-I/opt/homebrew/include`**: Specifies the include directory for Mosquitto headers.
   - **`-L/opt/homebrew/lib`**: Specifies the library directory where Mosquitto’s libraries are located.
   - **`-lmosquitto`**: Links against the Mosquitto library to resolve necessary symbols.




	If you are using an ARM-based Mac (e.g., M1 or M2 chip):
	```bash
	gcc -arch arm64 -fPIC -shared -o mosquitto-js-auth.so mosquitto-js-auth.c -I/opt/homebrew/include -L/opt/homebrew/lib -lmosquitto -Wl,-undefined,dynamic_lookup
	```
	- **`-arch arm64`**: Compiles the plugin for ARM64 architecture, suitable for Apple Silicon.
	- **`-Wl,-undefined,dynamic_lookup`**: Instructs the linker to resolve symbols dynamically, which is useful when the symbols are resolved at runtime.

3. **Setup Mosquitto**

    Add the following to your `mosquitto.conf`:

    ```ini
    auth_plugin /path/to/mosquitt-js-auth.so
    ```

4. **Write your authentication script**

    Create a file in your Mosquitto folder at `mosquitto-js-auth/validate.js`:

    ```javascript
    // mosquitto-js-auth/validate.js

    const username = process.argv[2];
    const password = process.argv[3];

    if (username === 'admin' && password === 'secret') {
        process.exit(0); // Authentication successful
    } else {
        process.exit(1); // Authentication failed
    }
    ```

5. **Run Mosquitto**

    ```bash
    mosquitto -c /path/to/mosquitto.conf
    ```