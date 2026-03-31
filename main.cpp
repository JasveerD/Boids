#include "raylib.h"
#include "raymath.h"
#include <vector>

#define MAX_SPEED 300
#define WIDTH 1500 
#define HEIGHT 1000
#define RADIUS_OF_PRECEP 60
#define RADIUS_OF_SEP 50
#define SEP_WEIGHT 1000.0f
#define MAX_FORCE 150.0f

struct Boid{

	Boid(Vector2 pos, Vector2 vel) : pos(pos), vel(vel), acc({0,0}) {}

	Vector2 pos;
	Vector2 vel;
	Vector2 acc;
	
	void update(float dt){
		vel = Vector2Add(vel, Vector2Scale(acc, dt));
		vel = Vector2ClampValue(vel, 0, MAX_SPEED);
		pos = Vector2Add(pos, Vector2Scale(vel, dt));
		if(pos.x < 0) pos.x += WIDTH;
		if(pos.x > WIDTH) pos.x -= WIDTH;
		if(pos.y < 0) pos.y += HEIGHT;
		if(pos.y > HEIGHT) pos.y -= HEIGHT;
		acc = {0,0};
	}

	void draw() const {
		float angle = atan2f(vel.y, vel.x);
		float size = 8.0f;
		Vector2 tip = { pos.x + size*cosf(angle), pos.y + size*sinf(angle) };

		Vector2 lP = { pos.x + (-size)*cosf(angle) - (size*0.5f)*sinf(angle),
               pos.y + (-size)*sinf(angle) + (size*0.5f)*cosf(angle) };

		Vector2 rP = { pos.x + (-size)*cosf(angle) - (-size*0.5f)*sinf(angle),
               pos.y + (-size)*sinf(angle) + (-size*0.5f)*cosf(angle) };

		DrawTriangle(tip, rP, lP, WHITE);
	}

};

struct Flock{

	Flock(int width, int height, int numBoid){
		
		for(int i=0; i<numBoid; i++){
			Vector2 rPos = {(float)GetRandomValue(0, width), (float)GetRandomValue(0, height)};
			Vector2 rVel = {(float)GetRandomValue(-100, 100), (float)GetRandomValue(-100, 100)};

			boids.push_back(Boid(rPos, rVel));
		}
	}

	std::vector<Boid> boids;

	void update(float dt){

		for(int i=0; i<boids.size(); i++){
			
			// cohession variables
			Vector2 cohSum = {0,0};			
			int cohCount = 0;

			// alignment variables
			Vector2 alignSum = {0,0}; 
			int alignCount = 0;

			// sepration variables
			Vector2 sepSum = {0,0}; 
			int sepCount = 0;

			for(int j=0; j<boids.size(); j++){

				if(i==j){continue;}
				else{
					if(Vector2Distance(boids[i].pos, boids[j].pos) < RADIUS_OF_PRECEP){
						cohSum = Vector2Add(cohSum, boids[j].pos); cohCount++;
						alignSum = Vector2Add(alignSum, boids[j].vel); alignCount++;
					}
					if(Vector2Distance(boids[i].pos, boids[j].pos) < RADIUS_OF_SEP){
						sepCount++;
						Vector2 away = Vector2Subtract(boids[i].pos, boids[j].pos);
						float dist = Vector2Distance(boids[i].pos, boids[j].pos);
						away = Vector2Scale(away, 1.0f / dist);  // weight by inverse distance
						sepSum = Vector2Add(sepSum, away);
					}
				}
			}
			if(cohCount > 0){
				// for cohession forces
				Vector2 avgPos = Vector2Scale(cohSum, 1.0f/cohCount);
				Vector2 forceDir = Vector2Normalize(Vector2Subtract(avgPos, boids[i].pos));
				Vector2 desiredVel = Vector2Scale(forceDir, MAX_SPEED);
				Vector2 steeringForce = Vector2Subtract(desiredVel, boids[i].vel);
				steeringForce = Vector2ClampValue(steeringForce, 0, MAX_FORCE);
				boids[i].acc = Vector2Add(boids[i].acc, steeringForce);
			}

			if(alignCount>0){
				Vector2 avgVel = Vector2Scale(alignSum, 1.0f/alignCount);
				Vector2 desiredVel = Vector2Scale(Vector2Normalize(avgVel), MAX_SPEED);
				Vector2 steeringForce = Vector2Subtract(desiredVel, boids[i].vel);
				steeringForce = Vector2ClampValue(steeringForce, 0, MAX_FORCE);
				boids[i].acc = Vector2Add(boids[i].acc, steeringForce);
			}
			
			if(sepCount>0){
				Vector2 avgSep = Vector2Scale(sepSum, 1.0f/sepCount);
				Vector2 desiredVel = Vector2Scale(Vector2Normalize(avgSep), MAX_SPEED);
				Vector2 steeringForce = Vector2Subtract(desiredVel, boids[i].vel);
				steeringForce = Vector2Scale(steeringForce, SEP_WEIGHT);
				steeringForce = Vector2ClampValue(steeringForce, 0, MAX_FORCE);
				boids[i].acc = Vector2Add(boids[i].acc, steeringForce);
			}

			boids[i].update(dt);
		}
	}

	void drawBoids(){
		for(const auto& b : boids){
			b.draw();
		}
	}
};


int main() {

    InitWindow(WIDTH, HEIGHT, "boids");
    SetTargetFPS(60);	
	Flock flock = Flock(WIDTH, HEIGHT, 500);

    while (!WindowShouldClose()) {
        BeginDrawing();
		DrawFPS(10, 10);
        ClearBackground(BLACK);
		float dt = GetFrameTime();

		flock.update(dt);
		flock.drawBoids();


        EndDrawing();
    }

    CloseWindow();
    return 0;
}
