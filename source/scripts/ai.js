function move() {
	var straight, left, right;
	straight = player.getFreeDistanceInDirection(0);
	if (straight < 200 ) {

		// 1 is left


		left = player.getFreeDistanceInDirection(3.14/4);
		right = player.getFreeDistanceInDirection(-3.14/4);


		// player.log("left   " + left);
		// player.log("right  " + right);
		if (left < right) {
			player.log("going right");
			play.angle = -1;
		} else {
			player.log("going left");
		 	player.angle = 1;
		}

		player.acceleration = -10;
		player.setTurbo(false);
	} else {
		player.angle = 0;
		player.acceleration = 1;
		player.setTurbo(true);

		player.log("resetting");
	}
	 player.angle *= 1.4;
}




