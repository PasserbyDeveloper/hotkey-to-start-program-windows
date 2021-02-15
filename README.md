# Program to listen to a hotkey and start other programs

This program starts a process that watches for a specific hotkey to start other programs

## How to use

Open terminal and input:

```bash
git clone https://github.com/PasserbyDeveloper/hotkey-to-start-program-windows-10.git
cd hotkey-to-start-program-windows-10
node index.js
```

Then press spacebar to test if it executes `echo Hello world` correctly.

Then change `index.js` to your needs

## Usage example

Suppose you are a windows user and you want `calc.exe` (windows default calculator) to run when you press `CTRL+P`.

Open `index.js` file in an editor and change `key_to_start_program` variable at line 12 to:

```diff
const key_to_start_program = {
+   code: getKeyTable().P,
+   needs_ctrl: true,
    needs_shift: false,
    needs_capslock: false
}
```

Then change the variables `program_to_execute` and `parameters_to_program` on lines 30 and 31:

```diff
+const program_to_execute = "calc.exe";
+const parameters_to_program = [];
```

Then run `node index.js` and you should see this:

```
 - Initializing hotkey hook for calc.exe
 - Ready
```

If you press `Ctrl + P` the calculator should be launched and the terminal should contain:

```
 - Initializing hotkey hook for calc.exe
 - Ready
 - Starting program because of hotkey press
 - Program exited successfully
```

As we can see, the `calc.exe` program has exited, even when the calculator is still open, this means the process is just a wrapper that runs the calculator and detached itself from it so the calculator keeps running and `calc.exe` exits.

That's it. As long as you keep `node index.js` running your `calc.exe` will be executed when you press the `CTRL+P` combination.

# How to close processes

By default, `key_to_close_program` is the same as `key_to_start_program`.

If you change the variable to something else, such as:

```diff
-const key_to_close_program = key_to_start_program;
+const key_to_close_program = {
+    code: getKeyTable().Q,
+    needs_ctrl: true,
+    needs_shift: false,
+    needs_capslock: false
+}
```

The above example will allow you to close the process (if it has been started) with the `Ctrl + Q` hotkey.

Note that `getKeyTable()` returns a `Record<string, number>` with the number of every known virtual key code known, but you might use any number if you know which virtual keys there are, such as the ones given in [this guide](https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes).

## Do not allow process to be closed by hotkey

Change the `key_to_close_program` variable to null:

```diff
-const key_to_close_program = key_to_start_program;
+const key_to_close_program = null;
```

# NPM Dependencies

None.

# Dependencies

Open source program I wrote available at the `` folder

# Linux Support

I don't have time to make linux support. If you want to help just send a PR request with the implementation. As long as its not overly complex I will accept it.