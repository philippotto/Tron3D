function move() {
	var straightDistance, leftDistance, rightDistance, leftDirection, rightDirection;

	leftDirection = 1;
	rightDirection = -1;
	straightDirection = 0;

	straightDistance = player.getFreeDistanceInDirection(0);
	leftDistance = player.getFreeDistanceInDirection(3.14/4);
	rightDistance = player.getFreeDistanceInDirection(-3.14/4);

	player.getID();
	player.getHealth();
	var isCurrentDirectionSafe = straightDistance > 500;
	player.setTurbo(false);
	if (isCurrentDirectionSafe) {
		player.angle = straightDirection;
		player.acceleration = 1;

		var isCurrentDirectionVerySafe = straightDistance > 800;
		if (isCurrentDirectionVerySafe)
		{
			if (Math.random() > 0.2) {
				player.setTurbo(true);
			} else {
				if (leftDistance > 500) {
					player.angle = leftDirection;
				} else if (rightDistance > 500) {
					player.angle = rightDirection;
				}
			}
		}

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