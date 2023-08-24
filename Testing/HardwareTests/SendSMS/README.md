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

- Connect to the appropriate COM port with a baud rate (115200 for the A9G).
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

You should receive an `OK` response.

### 5. Send the SMS:

To send an SMS to a specific number, use the `AT+CMGS` command. Replace `PHONENUMBER` with the recipient's phone number and `YOUR_MESSAGE` with your message content:
```bash
AT+CMGS="PHONENUMBER"
> YOUR_MESSAGE
```

After typing your message, send the message by pressing `Ctrl+Z` (or sending the ASCII code for `Ctrl+Z`, which is `0x1A`).

### 6. Confirmation:

Upon successful dispatch, the module should respond with:
```bash
+CMGS: <message reference number>
OK
```


## Troubleshooting:

1. Ensure the module is registered on the network. Use the `AT+CREG?` command. A response of `+CREG: 0,1` or `+CREG: 1,1` indicates registration.
2. Check signal strength with `AT+CSQ`. Values near 31 indicate strong signal.
3. Ensure the SIM card has SMS capabilities and sufficient balance/credit.

