void IsHit(Entity *e) {
	if (GameUtils::RandomIntegerRange(0, 10) <= 5) {
		e->TakeHit();

	}

	e->Life.Health -= 15;

	if (e->Life.Health <= 0) {
		e->PrepKill();
	}
}