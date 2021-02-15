// @ts-check
/**
 * Configure the hotkey to start the program
 *
 * Starting the process only works if its not already running
 *
 * By default the key to close the process is the same as the key to open it.
 * Change `key_to_close_program` variable to change this behaviour.
 *
 * @type {HotkeyConfig}
 */
const key_to_start_program = {
    code: getKeyTable().SPACE,
    needs_ctrl: false,
    needs_shift: false,
    needs_capslock: false
};

/**
 * Configure the hotkey to close the program after it has been started.
 *
 * Set to the same value as `key_to_start_program` to close the program with
 * the same key combination that started it.
 *
 * Set to null if you don't want any hotkey to terminate the started program.
 * @type {null | HotkeyConfig}
 */
const key_to_close_program = key_to_start_program;

const program_to_execute = "echo";
const parameters_to_program = ["Hello", "world"];

// Actual program implementation follows

const cp = require("child_process"); // Used to launch windows-interface

/**
 * @typedef {{code: number; needs_ctrl?: boolean; needs_shift?: boolean; needs_capslock?: boolean; }} HotkeyConfig
 */

console.log(" - Initializing hotkey hook for " + program_to_execute);

const watchers = [
    cp.spawn("./windows-interface/main.exe", [key_to_start_program.code.toString()])
];

// Add second watcher if necessary
if (key_to_close_program && key_to_start_program.code !== key_to_close_program.code) {
    watchers.push(
        cp.spawn("./windows-interface/main.exe", [key_to_close_program.code.toString()])
    );
}

// Attach error handlers
watchers.forEach((watcher, i) => watcher.on("error", function(err) {
    // @ts-ignore
    if (err && err.code && err.code === "ENOENT") {
        console.log(" - Error: The native application (executable) was not found (got ENOENT)");
        console.log(" - You either deleted it or forgot to include it from the repository");
        process.exit(1);
        return;
    }
    if (i === 0) {
        console.log(" - The key start watcher failed with error:");
    } else {
        console.log("The key close watcher failed with error:");
    }
    console.log(err);
    process.exit(1);
}));

/**
 * Test whether an event data descriptor matches a specific hotkey configuration
 * @param {{code: number, needs_ctrl?: boolean, needs_shift?: boolean, needs_capslock?: boolean}} desired_hotkey
 * @param {{pressed: number, ctrl: boolean, shift: boolean, capslock: boolean}} event_data
 */
function checkObjectForHotkey(desired_hotkey, event_data) {
    if (desired_hotkey.code !== event_data.pressed) {
        return false;
    }
    if (desired_hotkey.needs_ctrl && !event_data.ctrl) {
        return false;
    }
    if (desired_hotkey.needs_shift && !event_data.shift) {
        return false;
    }
    if (desired_hotkey.needs_capslock && !event_data.capslock) {
        return false;
    }
    return true;
}

watchers.forEach((watcher, i) => watcher.stdout.on("data", function(data) {
    try {
        const event_data = JSON.parse(data.toString("utf8"));
        if (event_data.pressed) {
            if (is_child_running) {
                if (key_to_close_program && checkObjectForHotkey(key_to_close_program, event_data)) {
                    onCloseKeyPressed();
                }
                return;
            }
            if (!is_child_running) {
                if (checkObjectForHotkey(key_to_start_program, event_data)) {
                    onStartKeyPressed();
                }
                return;
            }
        }
    } catch (err) {
        console.error(err);
        process.exit(1);
    }
}));

let child = null;
let is_child_running = false;
let child_exit_signal_index = 0;

function onStartKeyPressed() {
    if (is_child_running) {
        console.warn(" - Program is already running");
        return;
    }

    console.log(" - Starting program because of hotkey press");

    is_child_running = true;
    child_exit_signal_index = 0;
    child = cp.spawn(program_to_execute, parameters_to_program);

    child.on("error", function(err) {
        console.log(" - Execution of program returned an error:");
        console.log(err);
        is_child_running = false;
    });

    child.on("close", function(code) {
        if (code === 0) {
            console.log(" - Program exited successfully");
        } else {
            console.log(" - Program exited with status code", code);
        }
        is_child_running = false;
    });

    child.stdout.on("data", (data) => process.stdout.write(data));
    child.stderr.on("data", (data) => process.stdout.write(data));
}

function onCloseKeyPressed() {
    if (!is_child_running) {
        console.warn(" - Program is not running");
        return;
    }
    if (!child) {
        console.log(" - No child process to kill");
        return;
    }
    switch (child_exit_signal_index) {
        case 0: child.kill('SIGINT'); break; // Send interrupt from keyboard
        case 1: child.kill('SIGHUP'); break; // Hangup detected on controlling terminal
        case 2: child.kill('SIGQUIT'); break; // Quit from keyboard
        case 3: child.kill('SIGTERM'); break; // Termination signal (serious business)
    }
    child_exit_signal_index++;
    if (child_exit_signal_index > 4) {
        child_exit_signal_index = 0;
    }
}

function getKeyTable() {
    return {
        A: 0x41,
        B: 0x42,
        C: 0x43,
        D: 0x44,
        E: 0x45,
        F: 0x46,
        G: 0x47,
        H: 0x48,
        I: 0x49,
        J: 0x4A,
        K: 0x4B,
        L: 0x4C,
        M: 0x4D,
        N: 0x4E,
        O: 0x4F,
        P: 0x50,
        Q: 0x51,
        R: 0x52,
        S: 0x53,
        T: 0x54,
        U: 0x55,
        V: 0x56,
        W: 0x57,
        X: 0x58,
        Y: 0x59,
        Z: 0x5A,
        0: 0x30,
        1: 0x31,
        2: 0x32,
        3: 0x33,
        4: 0x34,
        5: 0x35,
        6: 0x36,
        7: 0x37,
        8: 0x38,
        9: 0x39,
        LBUTTON: 0x01,
        RBUTTON: 0x02,
        CANCEL: 0x03,
        MBUTTON: 0x04,
        XBUTTON1: 0x05,
        XBUTTON2: 0x06,
        BACK: 0x08,
        TAB: 0x09,
        CLEAR: 0x0C,
        RETURN: 0x0D,
        ENTER: 0x0D,
        SHIFT: 0x10,
        CONTROL: 0x11,
        MENU: 0x12,
        PAUSE: 0x13,
        CAPITAL: 0x14,
        KANA: 0x15,
        HANGEUL: 0x15,
        HANGUL: 0x15,
        ESCAPE: 0x1B,
        CONVERT: 0x1C,
        NONCONVERT: 0x1D,
        ACCEPT: 0x1E,
        MODECHANGE: 0x1F,
        SPACE: 0x20,
        PRIOR: 0x21,
        NEXT: 0x22,
        END: 0x23,
        HOME: 0x24,
        LEFT: 0x25,
        UP: 0x26,
        RIGHT: 0x27,
        DOWN: 0x28,
        SELECT: 0x29,
        PRINT: 0x2A, /* PAPER PRINTER, not print screen */
        EXECUTE: 0x2B,
        SNAPSHOT: 0x2C, /* PRINSCREEN / PRTSC */
        INSERT: 0x2D,
        DELETE: 0x2E,
        HELP: 0x2F,
        LWIN: 0x5B,
        RWIN: 0x5C,
        APPS: 0x5D,
        SLEEP: 0x5F,
        NUMPAD0: 0x60,
        NUMPAD1: 0x61,
        NUMPAD2: 0x62,
        NUMPAD3: 0x63,
        NUMPAD4: 0x64,
        NUMPAD5: 0x65,
        NUMPAD6: 0x66,
        NUMPAD7: 0x67,
        NUMPAD8: 0x68,
        NUMPAD9: 0x69,
        MULTIPLY: 0x6A,
        ADD: 0x6B,
        SEPARATOR: 0x6C,
        SUBTRACT: 0x6D,
        DECIMAL: 0x6E,
        DIVIDE: 0x6F,
        F1: 0x70,
        F2: 0x71,
        F3: 0x72,
        F4: 0x73,
        F5: 0x74,
        F6: 0x75,
        F7: 0x76,
        F8: 0x77,
        F9: 0x78,
        F10: 0x79,
        F11: 0x7A,
        F12: 0x7B,
        F13: 0x7C,
        F14: 0x7D,
        F15: 0x7E,
        F16: 0x7F,
        F17: 0x80,
        F18: 0x81,
        F19: 0x82,
        F20: 0x83,
        F21: 0x84,
        F22: 0x85,
        F23: 0x86,
        F24: 0x87,
        NAVIGATION_VIEW: 0x88,
        NAVIGATION_MENU: 0x89,
        NAVIGATION_UP: 0x8A,
        NAVIGATION_DOWN: 0x8B,
        NAVIGATION_LEFT: 0x8C,
        NAVIGATION_RIGHT: 0x8D,
        NAVIGATION_ACCEPT: 0x8E,
        NAVIGATION_CANCEL: 0x8F,
        NUMLOCK: 0x90,
        SCROLL: 0x91,
    }
}

setTimeout(() => console.log(" - Ready"), 100);