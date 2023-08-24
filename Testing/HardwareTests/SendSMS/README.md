# A9G GSM Module - Sending SMS

This guide provides a step-by-step procedure for sending an SMS using the A9G GSM module.

## Prerequisites:

- A9G GSM module with an antenna.
- SIM card with SMS capability and no PIN lock.
- Serial interface for communication (like a USB-to-Serial converter or microcontroller with a serial interface).
- Terminal software (like PuTTY, Tera Term, or the Arduino Serial Monitor).

## Steps:

### 1. Set Up Hardware:

- Insert the SIM card into the A9G module.
- Connect the A9G module to the serial interface.
- Ensure the module is powered on and you have a good signal strength.

### 2. Open Terminal Software:

- Connect to the appropriate COM port with a baud rate (commonly 115200 for the A9G).
- Ensure settings are 8-N-1: 8 data bits, no parity, 1 stop bit.

### 3. Test Basic Communication:

Send the command:
```bash
AT
```

The module should respond with:
```bash
OK
```


### 4. Set SMS Mode:

To send an SMS in text mode, send the command:
```bash
AT+CMGF=1
```