function move() {
	var straightDistance, leftDistance, rightDistance, leftDirection, rightDirection;

	leftDirection = 1;
	rightDirection = -1;
	straightDirection = 0;

	straightDistance = player.getFreeDistanceInDirection(0);
	leftDistance = player.getFreeDistanceInDirection(3.14/2);
	rightDistance = player.getFreeDistanceInDirection(-3.14/2);


	var isCurrentDirectionSafe = straightDistance > 500;
	player.setTurbo(false);
	if (isCurrentDirectionSafe) {
		player.angle = straightDirection;
		player.acceleration = 1;

		var isCurrentDirectionVerySafe = straightDistance > 700;
		if (isCurrentDirectionVerySafe)
			player.setTurbo(true);

	} else {
		if (straightDistance > leftDistance && straightDistance > rightDistance) {
			player.angle = straightDirection;
		} else {
			if (leftDistance > rightDistance)
				player.angle = leftDirection;
			else
				player.angle = rightDirection;
		}

		if (straightDistance < 200)
			player.acceleration = -10;
		else
			player.acceleration = 0;

	}
	return;
}