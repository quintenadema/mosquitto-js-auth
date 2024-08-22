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

1. **Clone the Repository**

    ```bash
    git clone https://github.com/quintenadema/mosquitt-js-auth.git
    cd mosquitt-js-auth
    ```

2. **Compile the Plugin**

    ```bash
    gcc -fPIC -shared -o mosquitt-js-auth.so mosquitt-js-auth.c -I/usr/include/mosquitto
    ```

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