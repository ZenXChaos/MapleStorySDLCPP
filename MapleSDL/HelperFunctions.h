void IsHit(Entity *e) {
	if (GameUtils::RandomIntegerRange(0, 10) <= 5) {
		e->TakeHit();
	}
}