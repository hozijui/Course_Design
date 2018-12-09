var stat = [0, 0, 0, 0];
var png = ["pic/off.png", "pic/on.png"];
var left = ["0", "20%"];

function SetLed(status) {
    stat = status;
    for (var i = 0; i < 4; i++) {
        $(".point").eq(i).attr("src", png[stat[i]]);
        $('.slider').eq(i).css("left", left[stat[i]]);
    }
}

function GetLed() {
    $.post("Led.json", JSON.stringify({action: "get"}),
        function (data) {
            SetLed(data.status);
        }, "json");
}

function SwitchLed(i) {
    $.post("Led.json", JSON.stringify({action: "set", id: i}),
        function (data) {
            SetLed(data.status);
        }, "json");
}

$(function () {
    GetLed();
    setInterval(GetLed, 2500);
});