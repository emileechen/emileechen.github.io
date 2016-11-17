
// Unhide #go-to-top when past the splash page.

$(window).scroll(function() {
	var scroll = $(window).scrollTop();

	if (scroll > $(window).height()/2) {
	    $(".side-nav").removeClass("hidden");
	}
	else {
	    $(".side-nav").addClass("hidden");
	}
});
