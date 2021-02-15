const cp = require("child_process"); // Used to launch win32-interface

console.log("Starting 'main.exe'");
console.log("press Spacebar to check if it works");

const child = cp.exec("main.exe", []);

child.on("error", function(err) {
    console.log("Child failed:");
    console.log(err);
    process.exit(1);
});

child.stderr.on("data", function(data) {
    process.stdout.write("stderr: " + data.toString("utf8"));
});

child.stdout.on("data", function(data) {
    process.stdout.write("stdout: " + data.toString("utf8"));
});

child.on("exit", function(code) {
    console.log("Watcher exited with", code);
})