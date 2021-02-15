#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <inttypes.h>
#include <stdint.h>
#include <windows.h>

int virtual_key_to_watch = VK_SPACE; // can be overwritten by numeric parameter

#pragma comment(lib, "User32.lib")

int buffer_index = 0;
char json_output_buffer[1024];

HHOOK hHook = { NULL };

int is_capslock_active() {
	return (GetKeyState(VK_CAPITAL) & 1) == 1;
}
int is_shift_active() {
	return GetKeyState(VK_LSHIFT) < 0 || GetKeyState(VK_RSHIFT) < 0;
}
int is_ctrl_pressed() {
	return GetKeyState(VK_LCONTROL) < 0 || GetKeyState(VK_RCONTROL) < 0;
}

LRESULT CALLBACK on_keyboard_update(const int code, const WPARAM wParam, const LPARAM lParam) {
    if (wParam != WM_KEYDOWN) {
        // not key down: exit early
	    return CallNextHookEx(hHook, code, wParam, lParam);
    }

    KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
    DWORD wVirtKey = kbdStruct->vkCode;
    DWORD wScanCode = kbdStruct->scanCode;

    if (wVirtKey != virtual_key_to_watch) {
        // printf("Virtual: %" PRId64 "\n", (int64_t) wVirtKey);
        // not key we want: exit early
	    return CallNextHookEx(hHook, code, wParam, lParam);
    }

    int64_t stateCode = (int64_t) GetAsyncKeyState(virtual_key_to_watch);

    LRESULT result = CallNextHookEx(hHook, code, wParam, lParam);

    buffer_index = 0;
    buffer_index += snprintf(
        &json_output_buffer[buffer_index],
        sizeof(json_output_buffer) - 1 - buffer_index,
        "{"
        "\"pressed\": %" PRId64
        ", \"ctrl\": %s"
        ", \"shift\": %s"
        ", \"capslock\": %s"
        ", \"state\": %" PRId64
        ", \"scan\": %" PRId64
        "}",
        (int64_t) wVirtKey,
        is_ctrl_pressed() ? "true" : "false",
        is_shift_active() ? "true" : "false",
        is_capslock_active() ? "true" : "false",
        (int64_t) stateCode,
        (int64_t) wScanCode
    );
    if (buffer_index < sizeof(json_output_buffer) - 8) {
        json_output_buffer[buffer_index++] = '\0';
        puts(json_output_buffer);
        fflush(stdout);
    } else {
        printf("{\"error\": true, \"message\": \"Output overflow\", \"index\": %d}\n", buffer_index);
    }
    return result;
}

const char * discoverAliasFromVirtualKey(int virtual_key) {
    if (virtual_key >= 0x30 && virtual_key <= 0x39) {
        return "digit";
    }
    if (virtual_key >= 0x41 && virtual_key <= 0x5A) {
        return "letter";
    }
    switch (virtual_key) {
        case VK_LBUTTON: return "VK_LBUTTON";
        case VK_RBUTTON: return "VK_RBUTTON";
        case VK_CANCEL: return "VK_CANCEL";
        case VK_MBUTTON: return "VK_MBUTTON";
        case VK_XBUTTON1: return "VK_XBUTTON1";
        case VK_XBUTTON2: return "VK_XBUTTON2";
        case VK_BACK: return "VK_BACK";
        case VK_TAB: return "VK_TAB";
        case VK_CLEAR: return "VK_CLEAR";
        case VK_RETURN: return "VK_RETURN";
        case VK_SHIFT: return "VK_SHIFT";
        case VK_CONTROL: return "VK_CONTROL";
        case VK_MENU: return "VK_MENU";
        case VK_PAUSE: return "VK_PAUSE";
        case VK_CAPITAL: return "VK_CAPITAL";
        case VK_KANA: return "VK_KANA";
        case VK_JUNJA: return "VK_JUNJA";
        case VK_FINAL: return "VK_FINAL";
        case VK_HANJA: return "VK_HANJA";
        case VK_ESCAPE: return "VK_ESCAPE";
        case VK_CONVERT: return "VK_CONVERT";
        case VK_NONCONVERT: return "VK_NONCONVERT";
        case VK_ACCEPT: return "VK_ACCEPT";
        case VK_MODECHANGE: return "VK_MODECHANGE";
        case VK_SPACE: return "VK_SPACE";
        case VK_PRIOR: return "VK_PRIOR";
        case VK_NEXT: return "VK_NEXT";
        case VK_END: return "VK_END";
        case VK_HOME: return "VK_HOME";
        case VK_LEFT: return "VK_LEFT";
        case VK_UP: return "VK_UP";
        case VK_RIGHT: return "VK_RIGHT";
        case VK_DOWN: return "VK_DOWN";
        case VK_SELECT: return "VK_SELECT";
        case VK_PRINT: return "VK_PRINT";
        case VK_EXECUTE: return "VK_EXECUTE";
        case VK_SNAPSHOT: return "VK_SNAPSHOT";
        case VK_INSERT: return "VK_INSERT";
        case VK_DELETE: return "VK_DELETE";
        case VK_HELP: return "VK_HELP";
        case VK_LWIN: return "VK_LWIN";
        case VK_RWIN: return "VK_RWIN";
        case VK_APPS: return "VK_APPS";
        case VK_SLEEP: return "VK_SLEEP";
        case VK_NUMPAD0: return "VK_NUMPAD0";
        case VK_NUMPAD1: return "VK_NUMPAD1";
        case VK_NUMPAD2: return "VK_NUMPAD2";
        case VK_NUMPAD3: return "VK_NUMPAD3";
        case VK_NUMPAD4: return "VK_NUMPAD4";
        case VK_NUMPAD5: return "VK_NUMPAD5";
        case VK_NUMPAD6: return "VK_NUMPAD6";
        case VK_NUMPAD7: return "VK_NUMPAD7";
        case VK_NUMPAD8: return "VK_NUMPAD8";
        case VK_NUMPAD9: return "VK_NUMPAD9";
        case VK_MULTIPLY: return "VK_MULTIPLY";
        case VK_ADD: return "VK_ADD";
        case VK_SEPARATOR: return "VK_SEPARATOR";
        case VK_SUBTRACT: return "VK_SUBTRACT";
        case VK_DECIMAL: return "VK_DECIMAL";
        case VK_DIVIDE: return "VK_DIVIDE";
        case VK_F1: return "VK_F1";
        case VK_F2: return "VK_F2";
        case VK_F3: return "VK_F3";
        case VK_F4: return "VK_F4";
        case VK_F5: return "VK_F5";
        case VK_F6: return "VK_F6";
        case VK_F7: return "VK_F7";
        case VK_F8: return "VK_F8";
        case VK_F9: return "VK_F9";
        case VK_F10: return "VK_F10";
        case VK_F11: return "VK_F11";
        case VK_F12: return "VK_F12";
        case VK_F13: return "VK_F13";
        case VK_F14: return "VK_F14";
        case VK_F15: return "VK_F15";
        case VK_F16: return "VK_F16";
        case VK_F17: return "VK_F17";
        case VK_F18: return "VK_F18";
        case VK_F19: return "VK_F19";
        case VK_F20: return "VK_F20";
        case VK_F21: return "VK_F21";
        case VK_F22: return "VK_F22";
        case VK_F23: return "VK_F23";
        case VK_F24: return "VK_F24";
        case VK_NAVIGATION_VIEW: return "VK_NAVIGATION_VIEW";
        case VK_NAVIGATION_MENU: return "VK_NAVIGATION_MENU";
        case VK_NAVIGATION_UP: return "VK_NAVIGATION_UP";
        case VK_NAVIGATION_DOWN: return "VK_NAVIGATION_DOWN";
        case VK_NAVIGATION_LEFT: return "VK_NAVIGATION_LEFT";
        case VK_NAVIGATION_RIGHT: return "VK_NAVIGATION_RIGHT";
        case VK_NAVIGATION_ACCEPT: return "VK_NAVIGATION_ACCEPT";
        case VK_NAVIGATION_CANCEL: return "VK_NAVIGATION_CANCEL";
        case VK_NUMLOCK: return "VK_NUMLOCK";
        case VK_SCROLL: return "VK_SCROLL";
        case VK_LSHIFT: return "VK_LSHIFT";
        case VK_RSHIFT: return "VK_RSHIFT";
        case VK_LCONTROL: return "VK_LCONTROL";
        case VK_RCONTROL: return "VK_RCONTROL";
        case VK_LMENU: return "VK_LMENU";
        case VK_RMENU: return "VK_RMENU";
        case VK_BROWSER_BACK: return "VK_BROWSER_BACK";
        case VK_BROWSER_FORWARD: return "VK_BROWSER_FORWARD";
        case VK_BROWSER_REFRESH: return "VK_BROWSER_REFRESH";
        case VK_BROWSER_STOP: return "VK_BROWSER_STOP";
        case VK_BROWSER_SEARCH: return "VK_BROWSER_SEARCH";
        case VK_BROWSER_FAVORITES: return "VK_BROWSER_FAVORITES";
        case VK_BROWSER_HOME: return "VK_BROWSER_HOME";
        case VK_VOLUME_MUTE: return "VK_VOLUME_MUTE";
        case VK_VOLUME_DOWN: return "VK_VOLUME_DOWN";
        case VK_VOLUME_UP: return "VK_VOLUME_UP";
        case VK_MEDIA_NEXT_TRACK: return "VK_MEDIA_NEXT_TRACK";
        case VK_MEDIA_PREV_TRACK: return "VK_MEDIA_PREV_TRACK";
        case VK_MEDIA_STOP: return "VK_MEDIA_STOP";
        case VK_MEDIA_PLAY_PAUSE: return "VK_MEDIA_PLAY_PAUSE";
        case VK_LAUNCH_MAIL: return "VK_LAUNCH_MAIL";
        case VK_LAUNCH_MEDIA_SELECT: return "VK_LAUNCH_MEDIA_SELECT";
        case VK_LAUNCH_APP1: return "VK_LAUNCH_APP1";
        case VK_LAUNCH_APP2: return "VK_LAUNCH_APP2";
        case VK_OEM_1: return "VK_OEM_1";
        case VK_OEM_PLUS: return "VK_OEM_PLUS";
        case VK_OEM_COMMA: return "VK_OEM_COMMA";
        case VK_OEM_MINUS: return "VK_OEM_MINUS";
        case VK_OEM_PERIOD: return "VK_OEM_PERIOD";
        case VK_OEM_2: return "VK_OEM_2";
        case VK_OEM_3: return "VK_OEM_3";
        case VK_GAMEPAD_A: return "VK_GAMEPAD_A";
        case VK_GAMEPAD_B: return "VK_GAMEPAD_B";
        case VK_GAMEPAD_X: return "VK_GAMEPAD_X";
        case VK_GAMEPAD_Y: return "VK_GAMEPAD_Y";
        case VK_GAMEPAD_RIGHT_SHOULDER: return "VK_GAMEPAD_RIGHT_SHOULDER";
        case VK_GAMEPAD_LEFT_SHOULDER: return "VK_GAMEPAD_LEFT_SHOULDER";
        case VK_GAMEPAD_LEFT_TRIGGER: return "VK_GAMEPAD_LEFT_TRIGGER";
        case VK_GAMEPAD_RIGHT_TRIGGER: return "VK_GAMEPAD_RIGHT_TRIGGER";
        case VK_GAMEPAD_DPAD_UP: return "VK_GAMEPAD_DPAD_UP";
        case VK_GAMEPAD_DPAD_DOWN: return "VK_GAMEPAD_DPAD_DOWN";
        case VK_GAMEPAD_DPAD_LEFT: return "VK_GAMEPAD_DPAD_LEFT";
        case VK_GAMEPAD_DPAD_RIGHT: return "VK_GAMEPAD_DPAD_RIGHT";
        case VK_GAMEPAD_MENU: return "VK_GAMEPAD_MENU";
        case VK_GAMEPAD_VIEW: return "VK_GAMEPAD_VIEW";
        case VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON: return "VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON";
        case VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON: return "VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON";
        case VK_GAMEPAD_LEFT_THUMBSTICK_UP: return "VK_GAMEPAD_LEFT_THUMBSTICK_UP";
        case VK_GAMEPAD_LEFT_THUMBSTICK_DOWN: return "VK_GAMEPAD_LEFT_THUMBSTICK_DOWN";
        case VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT: return "VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT";
        case VK_GAMEPAD_LEFT_THUMBSTICK_LEFT: return "VK_GAMEPAD_LEFT_THUMBSTICK_LEFT";
        case VK_GAMEPAD_RIGHT_THUMBSTICK_UP: return "VK_GAMEPAD_RIGHT_THUMBSTICK_UP";
        case VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN: return "VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN";
        case VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT: return "VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT";
        case VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT: return "VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT";;
        default: return "unknown";
    }
}

int main(int argn, char ** argc) {
    if (argn == 2) {
        virtual_key_to_watch = atoi(argc[1]);
    }

    buffer_index = 0;
    buffer_index += snprintf(
        &json_output_buffer[buffer_index],
        1024 - 1 - buffer_index,
        "{\"watching\": %d, \"alias\": \"%s\"}",
        virtual_key_to_watch,
        discoverAliasFromVirtualKey(virtual_key_to_watch)
    );
    puts(json_output_buffer);
    fflush(stdout);

    char * hotkey = argc[1];

	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, on_keyboard_update, NULL, 0);
	if (hHook == NULL) {
        buffer_index = 0;
        buffer_index += snprintf(
            &json_output_buffer[buffer_index],
            1024 - 1 - buffer_index,
            "{\"error\": true, \"message\": \"Could not attach keyboard hook\", \"problem\": \"SetWindowsHookEx returned NULL\"}",
            virtual_key_to_watch
        );
        puts(json_output_buffer);
        fflush(stdout);
        return 1;
    }

	while (1) {
        // should loop forever
        GetMessage(NULL, NULL, 0, 0);
        // loop just in case
        // don't worry this is not busy loop
    }

    return 0;
}