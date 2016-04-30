#ifndef BOX_H
#define BOX_H
class Box {
	b2Body* body = nullptr;
	b2Fixture* fixture = nullptr;
	b2Vec2 dimensions;
public:
	void Init(b2World* world, const b2Vec2 & pos, const b2Vec2 & dim);

	const b2Body* getBody() const {
		return this->body;
	}

	const b2Fixture* getFixture() const {
		return this->fixture;
	}

	const b2Vec2 & getDimensions() const {
		return this->dimensions;
	}
};
#endif