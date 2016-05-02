


window.onload = function() {
	initializeImages();
};



function unhover(element) {
	var url = image.getAttribute('src');
	var new_url = "p" + url.slice(1);
    image.setAttribute('src', new_url);
    console.log(image.getAttribute('src'));
}

function hover(image) {
	var url = (image.getAttribute('src'));
	var new_url = "s" + url.slice(1);
    image.setAttribute('src', new_url);
}

function pad(n) {
    return n > 9 ? "" + n: "0" + n;
}

function initializeImages() {
	var n = 22;
	var container = document.getElementById("grid");

	for (i = 1; i <= n; i++) {
		image = document.createElement("img");
		image.setAttribute('class', 'pic');
		image.setAttribute('id', pad(i));
		image.setAttribute('src', "p/" + pad(i) + ".jpg");
		image.setAttribute('onmouseout', "unhover(this)");
		image.setAttribute('onmouseover', "hover(this)");
		container.appendChild(image);
	}
}