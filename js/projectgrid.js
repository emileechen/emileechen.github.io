var hover = false;

var projnum = 3;
var width = (getWidth() - 354) * 0.8 - 50;
var height = window.innerHeight * 0.55;

var proj1 = document.getElementById("proj1");
var proj2 = document.getElementById("proj2");
var proj3 = document.getElementById("proj3");
var proj4 = document.getElementById("proj4");
var proj5 = document.getElementById("proj5");
var proj6 = document.getElementById("proj6");

setHeight();
setWidth();

function setWidth() {
	// var w = ((width/projnum) - (10 * projnum * 2));
	var w = (width/projnum);
	proj1.style.width = w + "px";
	proj2.style.width = w + "px";
	proj3.style.width = w + "px";
	proj4.style.width = w + "px";
	proj5.style.width = w + "px";
	proj6.style.width = w + "px";
}

function setHeight() {
	var w = (width/projnum);
	proj1.style.height = w + "px";
	// proj1.style.height = (width/projnum) - 10 * (projnum * 2) + "px";
	proj2.style.height = w + "px";
	proj3.style.height = w + "px";
	proj4.style.height = w + "px";
	proj5.style.height = w + "px";
	proj6.style.height = w + "px";
}


function getWidth() {
  if (self.innerHeight) {
    return self.innerWidth;
  }

  if (document.documentElement && document.documentElement.clientHeight) {
    return document.documentElement.clientWidth;
  }

  if (document.body) {
    return document.body.clientWidth;
  }
}