

window.onload = function() {
	initializeImages();
};

function pad(n) {
    return n > 9 ? "" + n : "0" + n;
}

function initializeImages() {
	var n = 23;
	var container = document.getElementById("grid");

	for (i = 1; i <= n; i++) {
		piccontain = document.createElement("div");
		piccontain.setAttribute('class', 'pic-contain');

		s = document.createElement('img');
		s.setAttribute('class', 'pic');
		s.setAttribute('src', "s/" + pad(i) + ".jpg");

		p = document.createElement('img');
		p.setAttribute('class', 'pic hide');
		p.setAttribute('src', "p/" + pad(i) + ".jpg");

		piccontain.appendChild(s);
		piccontain.appendChild(p);
		container.appendChild(piccontain);
	}

	piccontain = document.createElement("div");
	piccontain.setAttribute('class', 'pic-contain');
	blank = document.createElement('img');
	blank.setAttribute('class', 'pic');
	blank.setAttribute('src', "blank.jpg");
	piccontain.appendChild(blank);
	container.appendChild(piccontain);
}


