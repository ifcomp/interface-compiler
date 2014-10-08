var uuid = function () {
	var uuid = [];
	for (var i = 0; i < 16; i++) {
		uuid.push(Math.floor((Math.random()) * 256));
	};
	return uuid;
};