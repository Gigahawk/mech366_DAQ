function load(url, callback) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", url, true);
    xhr.responseType = "arraybuffer";

    xhr.onload = function (evt) {
        callback(xhr.response)
    }
    xhr.send(null);
}
function downloadCsv(link) {
    load(link, function(contents) {
        var filename = link + ".csv";
        var values = new Int16Array(contents);
        var t,x,y,z;
        var csv = "time, x, y, z\n";
        var cur_line = "";
        for (var i = 0; i < values.length/4; i++) {
            t = values[4*i + 0];
            x = values[4*i + 1];
            y = values[4*i + 2];
            z = values[4*i + 3];
            cur_line = `${t}, ${x}, ${y}, ${z}`;
            //console.log(cur_line);
            csv += cur_line + "\n";
        }
        var element = document.createElement("a");
        element.setAttribute("href", "data:text/plain;charset=utf-8," + encodeURIComponent(csv));
        element.setAttribute("download", filename);
        element.style.display = "none";
        document.body.appendChild(element);
        element.click();
        document.body.removeChild(element);
    });
}
