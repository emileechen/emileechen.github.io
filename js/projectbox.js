var hover = false;

var projnum = 4;
var width = (getWidth() - 354) * 0.8 - 50;

var proj1 = document.getElementById("proj1");
var proj2 = document.getElementById("proj2");
var proj3 = document.getElementById("proj3");
var proj4 = document.getElementById("proj4");

setWidth();


function setWidth() {
	proj1.style.width = (width/projnum) + "px";
	proj2.style.width = (width/projnum) + "px";
	proj3.style.width = (width/projnum) + "px";
	proj4.style.width = (width/projnum) + "px";
}

function skinny(id) {
	proj1.style.width = (width/projnum) * 0.7 + "px";
	proj2.style.width = (width/projnum) * 0.7 + "px";
	proj3.style.width = (width/projnum) * 0.7 + "px";
	proj4.style.width = (width/projnum) * 0.7 + "px";
	id.style.width = (width/projnum) * (1 + (0.3 * (projnum - 1))) + "px";
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