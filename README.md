# Mosquitto JS auth
Mosquitto JS auth is a lightweight plugin for the Mosquitto MQTT broker, written in C, designed to pass user credentials (username and password) to a custom JS script for user authentication. 

## Features
- Simple integration
- Minimal and lightweight
- ACL check support

## Installation

1. **Clone builds folder**
In the `builds` folder, you will find all builds for every system. 

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