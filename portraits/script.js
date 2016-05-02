function hover(element) {
    element.setAttribute('src', 'http://dummyimage.com/100x100/eb00eb/fff');
}
function unhover(element) {
    element.setAttribute('src', 'http://dummyimage.com/100x100/000/fff');
}


window.onload = function() {
	initializeImages();
};



function pad(n) {
    return n > 9 ? "" + n: "0" + n;
}

function initializeImages() {
	var n = 22;
	var container = document.getElementById("grid");

	for (i = 1; i <= n; i++) {
		image = document.createElement("img");
		image.setAttribute('class', 'pic');
		image.setAttribute('src', "p/" + pad(i) + ".jpg");
		container.appendChild(image);
	}
}
