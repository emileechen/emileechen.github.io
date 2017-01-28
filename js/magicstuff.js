
// Unhide #go-to-top when past the splash page.

$(window).scroll(function() {
	var scroll = $(window).scrollTop();

	if (scroll > $(window).height()/2) {
	    $(".side-nav-top").removeClass("hidden");
	}
	else {
	    $(".side-nav-top").addClass("hidden");
	}
});
