var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);
var filename = "";
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
        files.forEach(
            (f) => {
                fileListHTML += generateListItem(f);
            });
        document.getElementById("file_list").innerHTML = fileListHTML;
    } else if(msg.startsWith(":load")) {
        filename = "";
        var status_str = "No file loaded";
        var loaded_file = msg.split(/\r?\n/);
        loaded_file.shift(); // remove the first item
        if(loaded_file.length && loaded_file[0]) {
            filename = loaded_file[0];
            status_str = filename + " loaded"+
                "<button class=\"button\" onclick=\"runCar();\">Run</button>";
        }
        document.getElementById("loaded_file").innerHTML = status_str;
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


