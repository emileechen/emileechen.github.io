// document.getElementById()


// var currLocation = 0;
// var currPosition = document.documentElement.scrollTop || document.body.scrollTop;


var pages = ['splash', 'about', 'projects', 'contact'];

var pageLocations = {};
function refreshLocations() {
	for (p = 0; p < pages.length; p++) {
		pageLocations[pages[p]] = document.getElementById(pages[p]).offsetTop;
	}
}


// $(window).on("scroll", function(e) {
//  	var scrollTop = $(window).scrollTop();
//  	refreshLocations();
// 	for (p = 1; p < pages.length; p++) {
// 		console.log(pages[p]);
//  		if (scrollTop > pageLocations[pages[p]]) {
// 			$('#' + pages[p] + '-title').addClass("box-content-top");
//  		} else {
// 			$('#' + pages[p] + '-title').removeClass("box-content-top");
//  		}
// 	}

// 	// if (scrollTop > pageLocations.projects) {
// 	// 	$('#project-title').addClass("box-content-top");
// 	// }
// 	//  else {
// 	// 	$('#project-title').removeClass("box-content-top");
// 	// }
// });