#ifndef H_FUNC_H
#define H_FUNC_H
void IsHit(Entity *e) {
	if ((SDL_GetTicks() / 1000) - e->lastHit > 0.3 && e->alive == true && e->Life.Health > 0) {
		//if (GameUtils::RandomIntegerRange(0, 10) <= 5) {
		e->TakeHit();

		//}

		e->Life.Health -= 5;

		if (e->Life.Health <= 0) {
			e->PrepKill();
		}

		e->lastHit = (SDL_GetTicks() / 1000);
		printf("mob hit at %i\n", static_cast<int>(e->lastHit));
	}
}
#endif