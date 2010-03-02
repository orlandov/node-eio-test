var sys = require("sys");
var fs = require("fs");

var weirdy = require("./weirdy");

var w = new weirdy.Weirdy();

w.connect();
sys.puts("done");
