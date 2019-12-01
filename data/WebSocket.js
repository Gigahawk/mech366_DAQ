var connection;
window.addEventListener("load", () => {
    connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);
    connection.onopen = function () {
        connection.send("load");
        //connection.send('Connect ' + new Date());
    };
    connection.onerror = function (error) {
        console.log('WebSocket Error ', error);
    };
    connection.onmessage = function (e) {
        console.log('Server: ', e.data);
        handleMessage(e.data)
    };
    connection.onclose = function(){
        console.log('WebSocket connection closed');
    };
});
var filename = "";
var jogging = false;
var jog_forwards = false;

function getFileList() {
    console.log("Getting file list");
    connection.send("ls");
}

function handleMessage(msg) {
    if(msg.startsWith(":ls")){
        // Got a file list
        var files = msg.split(/\r?\n/);
        files.shift(); // remove the first item
        var fileListHTML = "";
        files.sort();
        files.forEach(
            (f) => {
                fileListHTML += generateListItem(f);
            });
        document.getElementById("file_list").innerHTML = fileListHTML;
    } else if(msg.startsWith(":load")) {
        filename = "";
        var status_str = "No file loaded";
        var imu_status_str = "unknown";
        var loaded_file = msg.split(/\r?\n/);
        loaded_file.shift(); // remove the first item
        if(loaded_file.length && loaded_file[0]) {
            filename = loaded_file[0];
            status_str = filename + " loaded"+
                "<button class=\"button\" onclick=\"runCar();\">Run</button>";
        }
        if(loaded_file.length > 1 && loaded_file[1]) {
            imu_status_str = loaded_file[1];
        }
        document.getElementById("loaded_file").innerHTML = status_str;
        document.getElementById("imu_status").innerHTML = imu_status_str;
    } else {
        console.log("Unknown response");
    }
}

function runCar(){
    if(confirm("Are you sure you want to run " + filename)){
        document.getElementById("loaded_file").innerHTML = "Running " + filename;
        connection.send("run");
    }
}

function start_jog_f(obj) {
    obj.style.backgroundColor = "#1EC5E5";
    jogging = true;
    jog_forwards = true;
}

function start_jog_b(obj) {
    obj.style.backgroundColor = "#1EC5E5";
    jogging = true;
    jog_forwards = false;
}

function stop_jog(obj) {
    obj.style.backgroundColor = "#D94A38";
    jogging = false;
    connection.send("jog s");
}

function jogCar() {
    if(jog_forwards)
        connection.send("jog f");
    else
        connection.send("jog b");
}

function loadFile(path) {
    if(confirm("Are you sure you want to load " + path)){
        connection.send("load " + path);
    }
}


function generateListItem(path) {
    return "<li><a href=\"" +
        path +
        "\">" +
        path +
        "</a>" +
        "<form action=\"/delete\" method=\"post\"onsubmit=\"return confirm('Do you really want to delete " + path + "');\">" +
        "<input type=\"hidden\" name=\"path\" value=\"" + path + "\">" +
        "<input type=\"submit\" value=\"Delete\">" +
        "</form>" +
        "<button class=\"button\" onclick=\"loadFile('" + path + "');\">Load</button>" +
        "<button class=\"button\" onclick=\"downloadCsv('" + path + "');\">Download CSV</button>" +
        "</li>";
}

window.setInterval(() => {
    if(connection.readyState != WebSocket.OPEN)
        return;
    if(jogging)
        jogCar();
}, 175);


