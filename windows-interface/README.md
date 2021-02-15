# What is this

This folder is used to compile the native windows application to attach a key hook to the keyboard.

# How to compile

Run `compile-windows-loop.bat`.

You must have Microsoft Visual Studio 2019 installed and then run ``.

# How does it work

The executable start by printing which virtual key code it is listening to, for example:

```
{"watching": 32, "alias": "VK_SPACE"}
```

This can be changed by sending a virtual key number (in this case the default was selected, which is 32)

Then, when you press the key it prints information regarding the key down event:

```
{"pressed": 32, "ctrl": false, "shift": false, "capslock": false, "state": 0, "scan": 57}
```

This JSON string is guaranteed to be sent as a full stdin block and there will always be a new line after a string block in case you need to separate it manually.
