var hover = false;

var projnum = 4;
var width = (getWidth() - 354) * 0.8 - 50;
var height = window.innerHeight * 0.55;

var proj1 = document.getElementById("proj1");
var proj2 = document.getElementById("proj2");
var proj3 = document.getElementById("proj3");
var proj4 = document.getElementById("proj4");

setWidth();
setHeight();


function setWidth() {
	proj1.style.width = (width/projnum) + "px";
	proj2.style.width = (width/projnum) + "px";
	proj3.style.width = (width/projnum) + "px";
	proj4.style.width = (width/projnum) + "px";
}

function setHeight() {
	proj1.style.height = height + "px";
	proj2.style.height = height + "px";
	proj3.style.height = height + "px";
	proj4.style.height = height + "px";
}

function hoverr(id) {
	proj1.style.width = (width/projnum) * 0.4 + "px";
	proj2.style.width = (width/projnum) * 0.4 + "px";
	proj3.style.width = (width/projnum) * 0.4 + "px";
	proj4.style.width = (width/projnum) * 0.4 + "px";
	id.style.width = (width/projnum) * (1 + (0.6 * (projnum - 1))) + "px";
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